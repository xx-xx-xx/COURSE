# README

## 编译环境

**OS**

Windows_NT x64 10.0.18363 

**JAVA**

java version "1.8.0_191"
Java(TM) SE Runtime Environment (build 1.8.0_191-b12)
Java HotSpot(TM) 64-Bit Server VM (build 25.191-b12, mixed mode)

**IDE**

VS Code Version: 1.44.1

## 编译方法

- 首先cmd中运行`netstat -ano | find "5507"` 查看该端口是否被占用，若被占用使用`taskkill /f /pid "xxxx"`结束此进程（xxxx表示此进程的id号）

* 使用`java -jar socket.jar`运行`socket.jar`或在VS Code中打开工程F5运行代码或直接运行`socket.exe`
* 访问如下网址
  * http://127.0.0.1:5507/txt/test.txt 
  * http://127.0.0.1:5507/html/test.html
  * http://127.0.0.1:5507/html/noimg.html

## 文件目录结构

```
│  manifest.txt
│  readme.md
│  socket.exe
│  socket.jar
│  实验报告模版_实验8.pdf
│  
├─assets
│  ├─html
│  │  │  noimg.html
│  │  │  test.html
│  │  │  
│  │  └─img
│  │          logo.jpg
│  │          
│  └─txt
│          test.txt
│          
└─code
    │  ClientConnection.java
    │  LogPrinter.java
    │  MySocket.java
    │  RequestParser.java
    │  
    └─bin
            ClientConnection.class
            LogPrinter.class
            MySocket$Exit.class
            MySocket.class
            RequestParser.class     

```

