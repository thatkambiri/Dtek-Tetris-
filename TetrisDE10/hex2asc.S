  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
	.globl hex2asc

hex2asc:
	li	a0, 4		# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
  #
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
