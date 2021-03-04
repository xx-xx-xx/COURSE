# coding: utf-8

import tensorflow as tf
import config
import os
from data_preprocess import *


# 文本分类的MLP结构
def MLP(input_text,keep_prob):
    # step 1
    # 将用id表示的文本映射到该单词对应的词向量组成句子的词向量数组
    # 单个新闻shape: len x emb
    # 批数据shape: batch x len x emd
    with tf.name_scope('emd'):
        # 定义词向量的表,shape: vocab_size X embedding_dim
        # 利用tf.get_variable
        embedding = tf.get_variable("embedding", shape=[config.vocab_size, config.embedding_dim])
        # 通过文本id在向量表中查询得到单词向量
        # 利用tf.nn.embedding_lookup
        embedding_input = tf.nn.embedding_lookup(embedding, input_text)
        # 改变embedding形状 batch x len x emd-> batch x (len*emd)
        # tf.reshape
        embedding_input = tf.reshape(embedding_input, [-1,(config.seq_length*config.embedding_dim)])

    # step 2
    # 将输入送入多层感知机（也就是多个全连接层）得到最后的输出
    # 输出的每条新闻应该包含n个类别的概率，共有batch条新闻
    # 输出shape: bacth x n_class
    # 我们定义两层感知机（为了防止过拟合加入了dropout层）
    # 首先经过wx+b的线性变化，再经过dropout剪枝，最后经过非线性变化函数relu
    with tf.name_scope("MLP"):
        # 线性变化的全连接层
        # 第一层
        # 可以手写wx+b的线性函数也可以利用函数tf.layers.dense
        fc1 = tf.layers.dense(embedding_input, config.hidden_dim)
        # 非线性变化包括sigmoid、relu等我们选择relu
        # 利用函数tf.nn.relu
        fc1 = tf.nn.relu(fc1)
        # dropout层
        # 利用函数tf.nn.dropout
        fc1 = tf.nn.dropout(fc1, keep_prob)

        # 第二层同理如上
        fc2 = tf.layers.dense(fc1, config.hidden_dim)
        fc2 = tf.nn.relu(fc2)
        fc2 = tf.nn.dropout(fc2, keep_prob)

        # 最后输出层需要转换为logits概率输出
        # 将第二层感知机的输出通过线性变化即可
        # 利用函数tf.layers.dense
        output_logits = tf.layers.dense(fc2, config.num_classes)

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
    train_dir = os.path.join(base_dir, 'cnews.train.txt')
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

    # return x_train, y_train, x_val, y_val, x_test, y_test
    return x_val, y_val


if __name__=='__main__':
    x_train, y_train = load_data()

    print('Start training and validating...')
    # 整个模型流图的定义
    # ----------------------定义输入----------------------
    # 一个是处理好的已经表示成id的固定长度文本 shape: None x seq_length
    # 一个是实际的分类标签 shape: None x num_classes
    input_text = tf.placeholder(tf.int32,[None, config.seq_length])
    input_label = tf.placeholder(tf.float32,[None, config.num_classes])
    # 定义一个剪枝的概率,仅训练需要，验证和测试集的时候设为1
    keep_prob = tf.placeholder(tf.float32)
    # ---------------------------------------------------

    # -------------------定义多层感知机的模型---------------
    # 调用MLP()函数，在该函数中完成输入到输出的定义
    output_logits, pred_label = MLP(input_text, keep_prob)
    # ---------------------------------------------------

    # ------------------定义损失函数进行优化----------------
    with tf.name_scope("optimize"):
        # 这里使用交叉熵
        # 如果标签为经过one hot处理则使用函数tf.nn.sparse_softmax_cross_entropy_with_logits
        # 而我们数据与处理后的标签是独立编码因此使用函数tf.nn.softmax_cross_entropy_with_logits_v2
        cross_entropy = tf.nn.softmax_cross_entropy_with_logits_v2(logits=output_logits, labels=input_label)
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
        correct_num = tf.equal(pred_label, true_label)
        # 利用tf.cast将正确的个数值转化为浮点数才能计算平均值tf.reduce_mean
        correct_num = tf.cast(correct_num, tf.float32)
        acc = tf.reduce_mean(correct_num)
    # ---------------------------------------------------

    # 创建session
    sess = tf.Session()
    sess.run(tf.global_variables_initializer())

    # train and val
    total_batch = 0  # 总批次
    best_acc_val = 0.0  # 最佳验证集准确率
    last_improved = 0  # 记录上一次提升批次
    require_improvement = 100  # 如果超过1000轮未提升，提前结束训练
    flag=False
    for epoch in range(config.num_epochs):
        print(f'Epoch: {epoch+1}')
        # 获取train批处理数据
        batch_train=batch_iter(x_train,y_train,config.batch_size)
        for x_batch,y_batch in batch_train:
            # 将训练的批处理数据送入流框架中，运行loss,acc,optim
            loss_train,acc_train,_ = sess.run([loss, acc, optim], feed_dict={input_text:x_batch, input_label:y_batch, keep_prob:config.dropout_keep_prob})
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
