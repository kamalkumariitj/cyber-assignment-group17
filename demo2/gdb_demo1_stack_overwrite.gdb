# ============================================================
# GDB Demo — Stack Variable Overwrite
#
# what this does:
#   we run a vulnerable C program and use GDB to watch
#   a buffer overflow happen live, byte by byte in memory
#
# build first:
#   gcc -g -gdwarf-4 -fno-stack-protector -O0 -o bin/01_overflow src/01_basic_overflow.c
#
# run it:
#   gdb -x gdb_demo1_stack_overwrite.gdb bin/01_overflow
# ============================================================

set pagination off
# arm64 doesn't use intel syntax, so we skip disassembly-flavor
# also force software watchpoints — docker doesn't support hardware ones
set can-use-hw-watchpoints 0

echo \n======================================================\n
echo  GDB DEMO 1: Live Stack Variable Overwrite\n
echo ======================================================\n

# first, show the raw assembly so the audience can see strcpy has no bounds check
echo \n[STEP 1] look at the assembly — spot the strcpy with no length check\n
disas check_access

# break right when we enter the vulnerable function
# (we need to be inside it so local variables like buffer and is_admin are in scope)
break check_access
echo \n[STEP 2] hit the breakpoint, now running with 13 A's as input (buffer is only 8 bytes)...\n
run AAAAAAAAAAAAA

# we landed at the top of check_access but is_admin = 0 hasn't run yet
# do one step so it initializes, then we set the watchpoint clean
next

echo \n[STEP 3] we are inside check_access now — is_admin should be 0\n

# confirm is_admin is 0 before anything bad happens
echo \n------------------------------ local variables right now ------------------------------\n
info locals

# dump the raw stack bytes so the audience can see where things live in memory
echo \n------------------------------ raw stack memory BEFORE the overflow ------------------------------\n
echo     x/24xb buffer\n
x/24xb buffer

# show that buffer and is_admin are right next to each other
echo \n------------------------------ where exactly are these variables in memory? ------------------------------\n
print &buffer
print &is_admin

# this is the key part — we tell GDB to watch is_admin
# the moment any byte of it changes, GDB will freeze and tell us
echo \n[STEP 4] setting a watchpoint on is_admin — GDB will stop the instant strcpy touches it\n
watch is_admin

# let it run — the printf calls execute, then strcpy overflows, watchpoint fires
continue

# watchpoint just fired — strcpy wrote past the buffer into is_admin
echo \n[STEP 5] watchpoint fired! strcpy just walked past the buffer and hit is_admin\n
echo \n------------------------------ see above: old value vs new value ------------------------------\n

echo \n------------------------------ local variables after the overflow ------------------------------\n
info locals

echo \n------------------------------ raw stack memory after the overflow ------------------------------\n
x/24xb buffer

# show registers so the audience sees where the CPU actually is right now
echo \n------------------------------ CPU registers at this exact moment ------------------------------\n
info registers sp pc lr x0 x1

# now show something extra — GDB itself can be the attacker
# remove the watchpoint so our manual writes don't keep triggering it
delete breakpoints
echo \n DEMO GDB can also play attacker — no overflow needed, just write directly in to variable\n

# reset is_admin so we start clean
echo \n------------------------------ reset is_admin to 0 first ------------------------------\n
set variable is_admin = 0
info locals

# now write an obvious value straight into is_admin using GDB
echo \n------------------------------ now write 0xDEADBEEF directly into is_admin using GDB ------------------------------\n
set variable is_admin = 0xDEADBEEF
info locals

# dump memory so the audience sees the hex representation
echo \n------------------------------ raw memory after the direct write ------------------------------\n
x/24xb buffer

# also show we can write individual bytes into the buffer itself
echo \n------------------------------ write BBBB into the first 4 bytes of buffer directly ------------------------------\n
set {char}(buffer+0) = 0x42
set {char}(buffer+1) = 0x42
set {char}(buffer+2) = 0x42
set {char}(buffer+3) = 0x42
echo \n    buffer[0..3] = 0x42 0x42 0x42 0x42 = 'B' 'B' 'B' 'B' ---\n
x/8xb buffer

# let it finish — program will print ACCESS GRANTED because is_admin = 0xDEADBEEF
continue
