## typedef

typedef A B就是把类型B作为类型A的一个新名字。但是碰到像typedef string *pstring或者是typedef int int_array[4]

比如定义一个string类型的指针变量，是string *str1;这个时候str1是变量名。

typedef string *str2;这个时候str2就不是变量名，而是类型名，它的类型就是变量str1所具有的类型。也就是string *类型。

所以typedef string *pstring这句话的意思就豁然开朗了，那么我以后可以拿pstring去定义别的变量，不如pstring pstr;这就定义了一个指向string的指针对象pstr。

typedef int int_array[4];如果去掉前面的typedef那么定义的是一个叫做int_array的含有4个元素的数组。前面加上typedef以后，==int_array就变成了含有4个元素的数组类型的替代名==。以后要有==int_array ia;==这句话我们就知道它等同于==int ia[4];==

再比如这一页的指向函数的指针:bool (*pf)(const string&, const string&)



```html
使用typdef以后，定义这一类型（即同样的形参以及同样的返回类型）的函数指针语句都将得到简化。方法如下：
typedef bool (*cmpFcn)(const string&, const string&);
用新得到的类型来定义两个新的函数指针pf1和pf2，它们所指向的函数都有两个const string&形参且返回bool类型值。以下是定义:
cmpFcn pf1;
cmpFcn pf2;
不过实际应用中这种小细节的东西用的应该不会很多。也算一点并不实用的领悟吧。
```