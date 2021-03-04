import gym
from RL_brain import PolicyGradient
import matplotlib.pyplot as plt

RENDER = False  # 在屏幕上显示模拟窗口会拖慢运行速度, 我们等计算机学得差不多了再显示模拟
DISPLAY_REWARD_THRESHOLD = 400  # 当 回合总 reward 大于 400 时显示模拟窗口

env = gym.make('CartPole-v0')
env.seed(1)     # 普通的 Policy gradient 方法, 使得回合的 variance 比较大, 所以我们选了一个好点的随机种子
env = env.unwrapped  # # 取消限制

print(env.action_space)    # 显示可用 action
print(env.observation_space)   # 显示可用 state 的 observation
print(env.observation_space.high)   # 显示 observation 最高值
print(env.observation_space.low)    # 显示 observation 最低值

RL = PolicyGradient(
    n_actions=env.action_space.n,
    n_features=env.observation_space.shape[0],
    learning_rate=0.02,
    reward_decay=0.99,
    # output_graph=True,    # 输出 tensorboard 文件才使用本句
)


# 主循环如下，与Qleanring 等算法在回合中每一步都可以更新参数不同的是，PolicyGradient是让计算机跑完一整个回合才更新一次
for i_episode in range(3000):
    observation = env.reset()
    while True:
        if RENDER: env.render()
        action = RL.choose_action(observation)
        observation_, reward, done, info = env.step(action)
        # 存储这一回合的 transition
        RL.store_transition(observation, action, reward)
        if done:
            ep_rs_sum = sum(RL.ep_rs)
            if 'running_reward' not in globals():
                running_reward = ep_rs_sum
            else:
                running_reward = running_reward * 0.99 + ep_rs_sum * 0.01
            if running_reward > DISPLAY_REWARD_THRESHOLD: RENDER = True     # 判断是否需要显示模拟
            print("episode:", i_episode, "  reward:", int(running_reward))

            # 学习，输出vt
            # vt 也就是 discounted_ep_rs_norm
            # 观察输出的图像，意思如下：
            # 请重视我这回合开始时的一系列动作, 因为前面一段时间杆子还没有掉下来
            # 请惩罚我之后的一系列动作, 因为后面的动作让杆子掉下来了
            # 或者可以理解为：
            # 我每次都想让这个动作在下一次增加被做的可能性 (grad(log(Policy))), 但是增加可能性的这种做法是好还是坏呢？
            # 这就要由 vt 告诉我了, 所以后段时间的 增加可能性 做法并没有被提倡, 而前段时间的 增加可能性 做法是被提倡的
            vt = RL.learn()

            if i_episode == 0:
                plt.plot(vt)    # plot本次episode的vt
                plt.xlabel('episode steps')
                plt.ylabel('normalized state-action value')
                plt.show()
            break
        observation = observation_