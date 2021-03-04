# coding=utf-8

from collections import Counter
import tensorflow as tf
import numpy as np

"""
    新闻数据集来源于THUCNews（根据新浪新闻RSS订阅频道2005~2011年间的历史数据筛选过滤生成）
    节选了14个候选分类类别：财经、彩票、房产、股票、家居、教育、科技、社会、时尚、时政、体育、星座、游戏、娱乐中的8个类别
"""

# 读取文件数据
def read_file(filename):
    contents, labels = [], []
    with open(filename, 'r', encoding='utf-8') as f:
        for line in f:
            try:
                # 每行新闻 类别+内容
                label, content = line.strip().split('\t')
                if content:
                    contents.append(list(content))
                    labels.append(label)
            except:
                pass
    return contents, labels

# 根据训练集构建词汇表，存储
def build_vocab(train_dir, vocab_dir, vocab_size=5000):
    data_train, _ = read_file(train_dir)
    all_data = []
    for content in data_train:
        all_data.extend(content)
    # Counter是统计单词的频数
    counter = Counter(all_data)
    # 统计前n个常见单词
    count_pairs = counter.most_common(vocab_size - 1)
    # 利用 * 号操作符，可以将元组解压为列表
    words, _ = list(zip(*count_pairs))
    # <PAD> 作为未知单词
    words = ['<PAD>'] + list(words)
    # '\n'.join() 单词列表转字符串以换行符连接
    open(vocab_dir, 'w', encoding='utf-8').write('\n'.join(words))

# 读取词汇表
def read_vocab(vocab_dir):
    with open(vocab_dir,'r', encoding='utf-8') as f:
        words = [line.strip() for line in f]
    # 把[('a',3),('b',2)]单词和下标的元组列表转换为字典，得到单词和下标的索引
    # 这样就可以用下标序号来代替单词本身
    word_to_id = dict(zip(words, range(len(words))))
    return words, word_to_id

# 读取分类目录(爬虫新闻固定的类别)
def read_category():
    categories = ['体育', '财经', '房产', '家居', '教育', '科技', '时尚', '时政', '游戏', '娱乐']
    categories = [x for x in categories]
    # 类别和下标的对应
    cat_to_id = dict(zip(categories, range(len(categories))))
    return categories, cat_to_id

# 将id表示的内容转换为文字
def to_words(content, words):
    # ''.join()列表转字符串函数
    return ''.join(words[x] for x in content)

# 读取新闻文件 将新闻的内容和标签都转为数字
def process_file(filename, word_to_id, cat_to_id, max_length=600):
    contents, labels = read_file(filename)

    data_id, label_id = [], []
    for i in range(len(contents)):
        data_id.append([word_to_id[x] for x in contents[i] if x in word_to_id])
        label_id.append(cat_to_id[labels[i]])

    # 使用keras提供的pad_sequences来将所有文本填充为固定长度
    x_pad = tf.keras.preprocessing.sequence.pad_sequences(data_id, max_length)
    # 将标签转换为one-hot独热编码表示
    y_pad = tf.keras.utils.to_categorical(label_id, num_classes=len(cat_to_id))
    return x_pad, y_pad

# 生成批次数据
def batch_iter(x, y, batch_size=64):
    data_len = len(x)
    num_batch = data_len//batch_size + 1
    # 将数据进行打乱排序，避免模型学习到数据的排列分布
    indices = np.random.permutation(np.arange(data_len))
    x_shuffle = x[indices]
    y_shuffle = y[indices]
    for i in range(num_batch):
        s=i*batch_size
        e=(i+1)*batch_size
        # yield是将该函数作为迭代器调用的时候，能够每次产生一批次的数据
        yield x_shuffle[s:e], y_shuffle[s:e]

