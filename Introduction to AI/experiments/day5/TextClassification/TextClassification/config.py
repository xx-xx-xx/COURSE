# coding=utf-8

"""
    参数配置
"""

# 模型共有参数
embedding_dim = 64      # 词向量维度
seq_length = 600        # 序列长度
num_classes = 10        # 类别数
vocab_size = 5000       # 词汇表达小

dropout_keep_prob = 0.8  # dropout保留比例
learning_rate = 1e-3  # 学习率

batch_size = 64  # 每批训练大小
num_epochs = 10  # 总迭代轮次

print_per_batch = 5 # 每多少轮输出一次结果

# MLP参数
hidden_dim = 256  # 全连接层神经元

# CNN参数
num_filters = 256  # 卷积核数目
kernel_size = 5  # 卷积核尺寸

# RNN参数
num_layers= 2   # rnn的层数
rnn = 'gru'     # lstm 或 gru
