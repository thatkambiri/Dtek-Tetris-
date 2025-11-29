# timetemplate.S
# Written 2015 by F Lundevall
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# Choose the macro syntax for RARS or DTEK-V board.         #
# The syntax for RARS is probably due to its MIPS heritage. #
#############################################################
###################
# Macros for RARS #
###################
#.macro	PUSH (%reg)
#	addi	sp,sp,-4
#	sw	%reg,0(sp) 
#.end_macro

#.macro	POP (%reg)
#	lw	%reg,0(sp)
#	addi	sp,sp,4
#.end_macro
###################
# Macros for DTEK #
###################
.macro	PUSH reg
	addi sp,sp,-4
	sw \reg,0(sp) 
.endm

.macro	POP reg
	lw	\reg,0(sp)
	addi	sp,sp,4
.endm
#############################################################

	.data
	.align 2
mytime:	.word 	0x5959
timstr:	.asciz 	"text more text lots of text\0"
	.text
	.globl timetemplate, tick, time2string, display_string, delay

# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 2		# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la	t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return

#########################################################
# Place for your functions: time2string, hex2asc, delay.#
#########################################################
hex2asc:
	li	a0, 15		# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	
	

hexasc:
	andi 	a0, a0, 0xf		#keep the four least significant bits.
	
	li	t0, 9
	
	ble	a0,t0, digit		#if our test number is from 0-9, we need to print a numerical digit
	
	#then a0 is a letter (10-15)
	addi	a0, a0, 55		#we are trying to skip to A-F ASCII values assuming the 
					#[ 0x41(ASCII value of 'A') - 10(Decimal) = 55(Decimal) ]  
	jr	ra
	
digit:
	addi	a0, a0, 0x30		#ASCII values of 0-9 start from 0x30 so
	jr	ra
	
##############################################################################	
#delay:
#	jr ra
	

delay: 
	PUSH	ra
	
while:
	blez	a0, exit_delay
	addi	a0, a0, -1
	
	li	t0, 0
	li	t1, 3112
for:
	addi	t0, t0, 1
	blt	t0, t1, for
	
	bgtz	a0, while
	
exit_delay:
	POP	ra
	jr	ra

	
##############################################################################
time2string:
	PUSH	s0
	PUSH	s1				 
	PUSH	ra
					
	mv	s1, a1			
	mv	s0, a0
	
	#Minutes tens 
	srli	t0, s1, 12		#shift to the first set of bits from the left to the right  
	andi	a0, t0, 0xf		#mask unecessary bits
	jal	hexasc			#call hexasc subroutine 
	sb	a0, 0(s0)		#store from a0 to s0 with the appropriate offset
	
	#Minutes ones 
	srli	t0, s1, 8
	andi	a0, t0, 0xf
	jal	hexasc
	sb	a0, 1(s0)
	
	#Colon Divider
	li	a0, 0x3a		#load the hex value for ASCII character ':'
	sb	a0, 2(s0)		#store in s0 with the required offset 
		
	
	#Seconds tens 
	srli	t0, s1, 4
	andi	a0, t0, 0xf
	jal	hexasc
	sb	a0, 3(s0)
	
	#Seconds ones 
	srli	t0, s1, 0
	andi	a0, t0, 0xf
	jal	hexasc
	sb	a0, 4(s0)
	
	# Null terminator
	li 	a0, 0x00		#load a null value into a0
	sb	a0, 5(s0)		#store in s0 with the required offset 
	
	POP	ra
    	POP 	s1
    	POP 	s0
    	jr	ra
