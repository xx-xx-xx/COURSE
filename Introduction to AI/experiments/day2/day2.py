# -*- coding: utf-8 -*-


#1.Standardization
#scale
from sklearn import preprocessing
import numpy as np
X_train = np.array([[ 1., -1.,  2.],
                    [ 2.,  0.,  0.],
                    [ 0.,  1., -1.]])
X_scaled = preprocessing.scale(X_train)

X_scaled 

X_scaled.mean(axis=0)

X_scaled.std(axis=0)

#StandardScaler class
scaler = preprocessing.StandardScaler().fit(X_train)
scaler
scaler.mean_                                      
scaler.scale_                                       
scaler.transform(X_train) 
X_test=[[-1, 1, 0.]]
scaler.transform(X_test)

#MinMaxScaler
X_train=np.array([[1., -1, 2.],
                 [2., 0., 0.],
                 [0., 1., -1]])

min_max_scaler=preprocessing.MinMaxScaler()
X_train_minmax=min_max_scaler.fit_transform(X_train)
X_train_minmax
X_test = np.array([[-3., -1.,  4.]])
X_test_minmax = min_max_scaler.transform(X_test)
X_test_minmax
min_max_scaler.scale_                             
min_max_scaler.min_
#given an explicit min/max
X_std = (X - X.min(axis=0)) / (X.max(axis=0) - X.min(axis=0))
X_scaled = X_std * (max - min) + min
X_train = np.array([[ 1., -1.,  2.],
                    [ 2.,  0.,  0.],
                    [ 0.,  1., -1.]])

max_abs_scaler = preprocessing.MaxAbsScaler()
X_train_maxabs = max_abs_scaler.fit_transform(X_train)
X_train_maxabs                # doctest +NORMALIZE_WHITESPACE^



X_test = np.array([[ -3., -1.,  4.]])
X_test_maxabs = max_abs_scaler.transform(X_test)
X_test_maxabs                 

max_abs_scaler.scale_      


#MaxAbsScaler
#RobustScaler

#2.Normalizer
X = [[ 1., -1.,  2.],
     [ 2.,  0.,  0.],
     [ 0.,  1., -1.]]
X_normalized = preprocessing.normalize(X, norm='l2')

X_normalized      
normalizer = preprocessing.Normalizer().fit(X)  # fit does nothing
normalizer
normalizer.transform(X)                            
normalizer.transform([[-1.,  1., 0.]])     


#3.Binarizer
X = [[ 1., -1.,  2.],
     [ 2.,  0.,  0.],
     [ 0.,  1., -1.]]

binarizer = preprocessing.Binarizer().fit(X)  # fit does nothing
binarizer
binarizer.transform(X)
binarizer = preprocessing.Binarizer(threshold=1.1)
binarizer.transform(X)

#4.Encoding categorical features
#LabelEncoder()

#OneHotEncoder()
enc = preprocessing.OneHotEncoder()
X = [['male', 'from US', 'uses Safari'], ['female', 'from Europe', 'uses Firefox']]
enc.fit(X)  

enc.transform([['female', 'from US', 'uses Safari'],
               ['male', 'from Europe', 'uses Safari']]).toarray()
enc.categories_
#OrdinalEncoder()
enc = preprocessing.OrdinalEncoder()
X = [['male', 'from US', 'uses Safari'], ['female', 'from Europe', 'uses Firefox']]
enc.fit(X)  

enc.transform([['female', 'from US', 'uses Safari']])
#6.Generating polynomial features
import numpy as np
from sklearn.preprocessing import PolynomialFeatures
X = np.arange(6).reshape(3, 2)
X                                                 
poly = PolynomialFeatures(2)
poly.fit_transform(X) 

#7.custom transformers
import numpy as np
from sklearn.preprocessing import FunctionTransformer
transformer = FunctionTransformer(np.log1p, validate=True)
X = np.array([[0, 1], [2, 3]])
transformer.transform(X)


#8.
sklearn.preprocessing.QuantileTransformer ()
sklearn.preprocessing.PowerTransformer()
#9.Discretization
X = np.array([[ -3., 5., 15 ],
              [  0., 6., 14 ],
              [  6., 3., 11 ]])
est = preprocessing.KBinsDiscretizer(n_bins=[3, 2, 2], encode='ordinal').fit(X)

#LabelEncoder与OneHotEncoder的混合使用方式
a = OneHotEncoder()
b = LabelEncoder()
temp是LabelEncoder对[123,456,789]处理的结果
将LabelEncoder的结果作为OneHotEncoder特征输入
输出特征[123,789]的OneHotEncoder的编码结果

要求：最后进行OneHotEncoder编码的时候
，temp和OneHotEncoder特征[123,789]均做reshape(-1,1)处理

from sklearn.preprocessing import LabelEncoder,OneHotEncoder
a = OneHotEncoder()
b = LabelEncoder()
temp=lb.fit_transform([123,456,789])
print(temp)#LabelEncoder的结果
enc.fit(tmp.reshape(-1,1))#将LabelEncoder的结果作为OneHotEncoder特征输入
x_train = enc.transform(lb.transform([123,789]).reshape(-1, 1))
#输出特征[123,789]的OneHotEncoder的编码结果
print(x_train)

