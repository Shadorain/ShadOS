# First set magic number of multiboot
.set MAGIC, 0x1BADB002

# Set Flags
.set FLAGS, 0

# Set checksum
.set CHECKSUM, -(MAGIC + FLAGS)

# Dont need any other fields

# Set multiboot section
.section .multiboot

# Set types for MAGIC, FLAGS, CHECKSUM
.long MAGIC
.long FLAGS
.long CHECKSUM


# Need stack for our kernel functions
# (Stack grows from higher to lower memory)
# Define stack bottom
stackBottom: 

# Say we'd need 512mb stack size -- Skip them
.skip 512

# Define stack top
stackTop:

# Set sections
.section .text
.global _start
.type _start, @function

# Start with a starting point
_start:
    # get current stack pointer from ESP
    mov $stackTop, %esp
    # Call main kernel function
    call kernel_main
    # clear interrupts
    cli

# Put system in infinite loop
hltLoop:
    hlt
    jmp hltLoop

.size _start, . - _start
