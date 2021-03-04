import numpy as np
import tensorflow as tf

# reproducible
np.random.seed(1)
tf.set_random_seed(1)


class PolicyGradient:

    # 定义初始化函数（初始化的时候，需要给出参数并创建一个网络）
    def __init__(self,n_actions,n_features,learning_rate=0.01,reward_decay=0.95,output_graph=False,):
        self.n_actions = n_actions
        self.n_features = n_features
        self.lr = learning_rate  # 学习率
        self.gamma = reward_decay  # reward 递减率
        # 这是我们存储 回合信息的 list
        self.ep_obs, self.ep_as, self.ep_rs = [], [], []
        # 建立 policy 神经网络
        self._build_net()
        self.sess = tf.Session()
        if output_graph:   # 判断是否输出 tensorboard 文件
            # $ tensorboard --logdir=logs
            # http://0.0.0.0:6006/
            # tf.train.SummaryWriter soon be deprecated, use following
            tf.summary.FileWriter("logs/", self.sess.graph)
        self.sess.run(tf.global_variables_initializer())

    # 建立 policy gradient 神经网络
    def _build_net(self):
        with tf.name_scope('inputs'):
            # 接收 observation
            self.tf_obs = tf.placeholder(tf.float32, [None, self.n_features], name="observations")
            # 接收我们在这个回合中选过的 actions
            self.tf_acts = tf.placeholder(tf.int32, [None, ], name="actions_num")
            # 接收每个 state-action 所对应的 value (通过 reward 计算)
            self.tf_vt = tf.placeholder(tf.float32, [None, ], name="actions_value")

        # fc1
        layer = tf.layers.dense(
            inputs=self.tf_obs,
            units=10,   # 输出个数
            activation=tf.nn.tanh,  # tanh activation
            kernel_initializer=tf.random_normal_initializer(mean=0, stddev=0.3),
            bias_initializer=tf.constant_initializer(0.1),
            name='fc1'
        )
        # fc2
        all_act = tf.layers.dense(
            inputs=layer,
            units=self.n_actions,    # 输出个数
            activation=None,   # 之后再加 Softmax
            kernel_initializer=tf.random_normal_initializer(mean=0, stddev=0.3),
            bias_initializer=tf.constant_initializer(0.1),
            name='fc2'
        )

        self.all_act_prob = tf.nn.softmax(all_act, name='act_prob')  # 用softmax算出probability

        with tf.name_scope('loss'):
            # 最大化 总体 reward (log_p * R) 就是在最小化 -(log_p * R), 而 tf 的功能里只有最小化 loss
            neg_log_prob = tf.nn.sparse_softmax_cross_entropy_with_logits(logits=all_act, labels=self.tf_acts)
            # 或者换一种写法：
            # neg_log_prob = tf.reduce_sum(-tf.log(self.all_act_prob)*tf.one_hot(self.tf_acts, self.n_actions), axis=1)
            # 这两种形式是一模一样的, 只是第二个是第一个的展开形式.
            # 都是分类问题中的 cross-entropy，即使用 softmax 和神经网络的最后一层 logits 输出和真实标签 (self.tf_acts) 对比的误差
            # 并将神经网络的参数按照这个真实标签改进

            # 我们能将这个 neg_log_prob 理解成 cross-entropy 的分类误差
            # 分类问题中的标签是真实 x 对应的 y, 而我们 Policy gradient 算法中, x 是 state, y 就是它按照这个 x 所做的动作号码.
            # 所以也可以理解成, 它按照 x 做的动作永远是对的 (出来的动作永远是正确标签), 它也永远会按照这个 “正确标签” 修改自己的参数
            # 可是事实却不是这样, 他的动作不一定都是 “正确标签”, 这就是强化学习(Policy gradient)和监督学习(classification)的不同
            # 为了确保这个动作真的是 “正确标签”, 我们的 loss 需要在原本的 cross-entropy 形式上乘以 vt
            # 用 vt 来告诉这个 cross-entropy 算出来的梯度是不是一个值得信任的梯度
            # 如果 vt 小, 或者是负的, 就说明这个梯度下降是一个错误的方向, 我们应该向着另一个方向更新参数
            # 如果这个 vt 是正的, 或很大, vt 就会称赞 cross-entropy 出来的梯度, 并朝着这个方向梯度下降
            # 实在不理解，看PDF中的参考资料

            # (vt = 本reward + 衰减的未来reward) 引导参数的梯度下降
            loss = tf.reduce_mean(neg_log_prob * self.tf_vt)


        with tf.name_scope('train'):
            self.train_op = tf.train.AdamOptimizer(self.lr).minimize(loss)

    # 根据一定的策略选择行为
    # 这个行为不再是用 Q value 来选定的, 而是用概率来选定. 即使不用 epsilon-greedy, 也具有一定的随机性
    def choose_action(self, observation):
        # 所有 action 的概率
        prob_weights = self.sess.run(self.all_act_prob, feed_dict={self.tf_obs: observation[np.newaxis, :]})
        # 根据概率来选 action
        action = np.random.choice(range(prob_weights.shape[1]), p=prob_weights.ravel())
        return action

    # 存储回合的 transition
    # 将这一步的 observation, action, reward 加到列表中去
    # 因为本回合完毕之后要清空列表, 然后存储下一回合的数据, 所以我们会在 learn() 当中进行清空列表的动作
    def store_transition(self, s, a, r):
        self.ep_obs.append(s)
        self.ep_as.append(a)
        self.ep_rs.append(r)

    # 学习更新参数
    # 我们要对这回合的所有 reward 动动手脚, 使他变得更适合被学习
    # 1.随着时间推进, 用 gamma 衰减未来的 reward
    # 2.为了一定程度上减小 policy gradient 回合 variance, 我们标准化回合的 state-action value
    def learn(self):
        # 衰减, 并标准化这回合的 reward
        discounted_ep_rs_norm = self._discount_and_norm_rewards()

        # train on episode
        self.sess.run(self.train_op, feed_dict={
             self.tf_obs: np.vstack(self.ep_obs),  # shape=[None, n_obs]
             self.tf_acts: np.array(self.ep_as),  # shape=[None, ]
             self.tf_vt: discounted_ep_rs_norm,  # shape=[None, ]
        })
        self.ep_obs, self.ep_as, self.ep_rs = [], [], []    # 清空列表中的回合 data
        # 返回这一回合的 state-action value
        return discounted_ep_rs_norm

    # 定义衰减回合的 reward
    # _discount_and_norm_rewards()用来估算每一个action对应的潜在价值discounted_ep_rs
    # 每次获得的reward都和前边的action有关，我们在衡量每一个action带来的实际价值的时候，不能只看当前reward，也要考虑后边的reward
    # 那些能让杆长时间在空中竖直的action应该拥有较大的期望值，让杆倾倒的action拥有较小的期望值
    # 越靠后的action期望价值越小，因为他们更可能是导致杆倾倒的原因，越靠前的action期望价值大是因为其长时间的保持了杆的竖直
    # 因此我们倒推整个过程，从最后一个action开始计算所有action对应的期望价值
    # 函数中输入的参数r是每一个action实际获得的奖励，即即时性的奖励，在本例中，除了在最后杆倾倒的时候奖励为0，其余时刻奖励均为1
    # 我们定义么一个action除了直接获得的reward外还有的潜在价值为running_add，running_add从后向前累计且经过discount衰减
    # 每一个action的潜在价值为后一个action的潜在价值乘以衰减系数gamma再加上直接获得的奖励，即running_add * self.gamma + self.ep_rs[t]
    # 这样做符合我们的意愿——越靠前的action期望价值大
    def _discount_and_norm_rewards(self):
        # discount episode rewards
        discounted_ep_rs = np.zeros_like(self.ep_rs)
        running_add = 0
        for t in reversed(range(0, len(self.ep_rs))):
            running_add = running_add * self.gamma + self.ep_rs[t]
            discounted_ep_rs[t] = running_add

        # normalize episode rewards
        discounted_ep_rs -= np.mean(discounted_ep_rs)
        discounted_ep_rs /= np.std(discounted_ep_rs)
        return discounted_ep_rs