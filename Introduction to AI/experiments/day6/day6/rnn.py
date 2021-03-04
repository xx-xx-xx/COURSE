import tensorflow as tf
import config
import os
from data_preprocess import *


# 文本分类的MLP结构
def RNN(input_text, keep_prob):
    # step 1
    # 将用id表示的文本映射到该单词对应的词向量组成句子的词向量数组
    # 单个新闻shape: len x emb
    # 批数据shape: batch x len x emd
    with tf.name_scope('emd'):
        # 定义词向量的表
        # 利用tf.get_variable
        embedding = tf.get_variable('embedding', [config.vocab_size, config.embedding_dim])
        # 通过文本id在向量表中查询得到单词向量
        # 利用tf.nn.embedding_lookup
        # embedding_input batch x len x emd
        embedding_input = tf.nn.embedding_lookup(embedding, input_text)

    # step 2
    # 将输入送入RNN中得到分类标签的输出
    with tf.name_scope("RNN"):
        # 两种不同rnn循环单元的初始化
        if (config.rnn == 'lstm'):
            # 利用函数tf.nn.rnn_cell.BasicLSTMCell 初始化LSTM
            cell = tf.nn.rnn_cell.BasicLSTMCell(30)
        else:
            # # 利用函数tf.nn.rnn_cell.GRUCell 初始化GRU
            cell = tf.nn.rnn_cell.GRUCell(30)
        # 利用循环单元, 建立循环神经网络
        # 利用函数tf.nn.dynamic_rnn
        # 该函数的返回值有两个输出，一个是循环单元最终结果的输出一个是送入下一个循环单元的隐藏状态输出
        # 我们关心的是循环单元最终结果的输出
        rnn_outputs, _ = tf.nn.dynamic_rnn(cell, embedding_input, dtype=tf.float32)
        # 最终结果的输出包括了每个神经单元的结果，我们仅关心最后一个神经单元的结果，因为它包含了整个文章序列模型
        # 取最后一个时序输出作为结果
        # 直接利用下标操作
        # shape： batch x len x hidden_dim -> batch x hidden_dim(batch x len[-1] x hidden_dim)
        last_output = rnn_outputs[:, -1, :]

    # 将卷积的特征输入两次全连接层也就是MLP中
    # 第二层全连接不进行性非线性变化和dropout
    with tf.name_scope("MLP_output"):
        # 第一层全连接
        # 全连接层，后面接relu激活、dropout
        # 利用函数tf.layers.dense
        fc1 = tf.layers.dense(last_output, 256)
        # 利用函数tf.nn.relu
        fc1 = tf.nn.relu(fc1)
        # 利用函数tf.nn.dropout
        fc1 = tf.nn.dropout(fc1, keep_prob)

        # 第二层全连接也就是最后输出的分类层
        # 利用函数tf.layers.dense
        output_logits = tf.layers.dense(fc1, config.num_classes)

        # 测试的时候
        # 需要手动通过logits概率判断新闻所属类别
        # 首先将logits概率归一化
        # 利用函数tf.nn.softmax
        norm_logits = tf.nn.softmax(output_logits)
        # 通过查找最大的概率值返回其对应下标得到最有可能的类别标签
        # 利用函数tf.argmax
        pred_label = tf.argmax(norm_logits, axis=1)

    # 函数返回logit概率和预测标签
    return output_logits, pred_label


def load_data():
    base_dir = 'cnews'
    train_dir = os.path.join(base_dir, 'cnews.val.txt')
    test_dir = os.path.join(base_dir, 'cnews.test.txt')
    val_dir = os.path.join(base_dir, 'cnews.val.txt')
    vocab_dir = os.path.join(base_dir, 'cnews.vocab.txt')

    print('Start loading train and val data...')
    if not os.path.exists(vocab_dir):  # 如果不存在词汇表，重建
        build_vocab(train_dir, vocab_dir, config.vocab_size)
    categories, cat_to_id = read_category()
    words, word_to_id = read_vocab(vocab_dir)
    config.vocab_size = len(words)
    # x_train, y_train = process_file(train_dir, word_to_id, cat_to_id, config.seq_length)
    x_val, y_val = process_file(val_dir, word_to_id, cat_to_id, config.seq_length)
    # x_test, y_test = process_file(test_dir, word_to_id, cat_to_id, config.seq_length)
    print('End loading')

    # return x_train,y_train,x_val,y_val,x_test,y_test
    return x_val, y_val


if __name__ == '__main__':
    x_train, y_train = load_data()

    print('Start training and validating...')
    # 整个模型流图的定义
    # ----------------------定义输入----------------------
    # 一个是处理好的已经表示成id的固定长度文本
    # 一个是实际的分类标签
    input_text = tf.placeholder(tf.int32, shape=(None, config.seq_length))
    input_label = tf.placeholder(tf.float32, shape=(None, config.num_classes))
    # 定义一个剪枝的概率,仅训练需要，验证和测试集的时候设为1
    keep_prob = tf.placeholder(tf.float32)
    # ---------------------------------------------------

    # -------------------定义多层感知机的模型---------------
    # 调用RNN()函数，在该函数中完成输入到输出的定义
    output_logits, pred_label = RNN(input_text, keep_prob)
    # ---------------------------------------------------

    # ------------------定义损失函数进行优化----------------
    with tf.name_scope("optimize"):
        # 这里使用交叉熵
        # 如果标签为经过one hot处理则使用函数tf.nn.sparse_softmax_cross_entropy_with_logits
        # 而我们数据与处理后的标签是独立编码因此使用函数tf.nn.softmax_cross_entropy_with_logits_v2
        cross_entropy = tf.nn.softmax_cross_entropy_with_logits_v2(labels=input_label, logits=output_logits)
        # 将损失值经过均值处理使优化更稳定
        # 利用函数tf.reduce_mean
        loss = tf.reduce_mean(cross_entropy)
        # 选择合适的优化器Gradient or Adam
        # 复杂的数据集推荐使用函数tf.train.AdamOptimizer
        optim = tf.train.AdamOptimizer(config.learning_rate).minimize(loss)
    # ---------------------------------------------------

    # --------------------计算模型的准确率-----------------
    with tf.name_scope("accuracy"):
        # 准确率
        # 正确分类的数据由独热编码转为标签
        # 利用函数tf.argmax
        true_label = tf.argmax(input_label, axis=1)
        # 利用函数tf.equal判断实际和预测值的正确个数
        correct_num = tf.equal(true_label, pred_label)
        # 利用tf.cast将正确的个数值转化为浮点数才能计算平均值tf.reduce_mean
        acc = tf.reduce_mean(tf.cast(correct_num, tf.float32))
    # ---------------------------------------------------

    # 创建session
    sess = tf.Session()
    sess.run(tf.global_variables_initializer())

    # train and val
    total_batch = 0  # 总批次
    best_acc_val = 0.0  # 最佳验证集准确率
    last_improved = 0  # 记录上一次提升批次
    require_improvement = 100  # 如果超过1000轮未提升，提前结束训练
    flag = False
    for epoch in range(config.num_epochs):
        print(f'Epoch: {epoch+1}')
        # 获取train批处理数据
        batch_train = batch_iter(x_train, y_train, config.batch_size)
        for x_batch, y_batch in batch_train:
            # 将训练的批处理数据送入流框架中，运行loss,acc,optim
            loss_train, acc_train, _ = sess.run([loss, acc, optim], feed_dict={input_text:x_batch, input_label:y_batch, keep_prob:0.8})
            if total_batch % config.print_per_batch == 0:
                # 每多少轮次输出在训练集和验证集上的性能
                # 在验证上验证
                if acc_train > best_acc_val:
                    # 保存最好结果
                    best_acc_val = acc_train
                    last_improved = total_batch
                print(f"Iter: {total_batch}, Train Loss: {loss_train}, Train Acc: {acc_train}")
            total_batch += 1
            # 正确率长期不提升，提前结束训练
            if total_batch - last_improved > require_improvement:
                print("No optimization for a long time, auto-stopping...")
                flag = True
                break  # 跳出循环
        if flag:  # 同上
            break
    print('End optimization')
