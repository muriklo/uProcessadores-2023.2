#define MAX_SIZE 16
.eqv RB_MAX_SIZE 16

#typedef struct ringbuffer {
#	int size;
#	int rd;
#	int wr;
#	char buf[MAX_SIZE];
#} t_ringbuffer;
.macro t_ringbuffer(%name)
.data
%name:
.align 2
	.space 12
	.space RB_MAX_SIZE
.end_macro

.text
# void rb_init(t_ringbuffer * rbuf){
# 	rbuf->size = 0;
# 	rbuf->rd = 0;
# 	rbuf->wr = 0;
# }
rb_init:
	sw $zero, 0($a0)
	sw $zero, 4($a0)
	sw $zero, 8($a0)
	jr $ra

# bool rb_empty(t_ringbuffer * rbuf){
#	if (rbuf->size == 0) {
#		return 1;
#	} else {
#		return 0;
#	}
# }
rb_empty:
	lw $t0, 0($a0)	# Carrega rbuf->size em $t0
    	beqz $t0, if_empty 	# rbuf->size == 0
    	li $v0, 0       # return 0
    	jr $ra          

if_empty:
	li $v0, 1	# return 1
	jr $ra
	
# bool rb_full(t_ringbuffer * rbuf){
#	if (rbuf->size == MAX_SIZE)
#		return 1;
#	else
#		return 0;
# } 
rb_full:
	lw $t0, 0($a0)     	# Carrega rbuf->size em $t0
	li $t1, RB_MAX_SIZE
    	bne $t0, $t1, else_full # rbuf->size == MAX_SIZE
    	li $v0, 1          	# return 1
    	jr $ra             

else_full:
	li $v0, 0		# return 0
	jr $ra
	
#char rb_read(t_ringbuffer * rbuf){
#	char tmp = 0;
#	if(!rbuf_empty(rbuf)){
#		rbuf->size--;
#		tmp = rbuf->buf[rbuf->rd];
#		rbuf->rd = (rbuf->rd + 1) % MAX_SIZE;
#	}
#	return tmp;
#}
rb_read:
	addiu $sp, $sp, -16
	sw    $ra, 8($sp)
	
	li   $t0, 0
	sw   $t0, 12($sp)
	
	jal rb_empty
	lw   $t0, 12($sp)
	bnez $v0, exit_read	# Se size for diferente de zero, pule para exit
	
	lw   $t1, 0($a0)
	addi $t1, $t1, -1
	sw   $t1, 0($a0)
	
	lw   $t1, 4($a0)
	add  $t2, $t1, $a0
	lb   $t0, 12($t2)

	addi $t1, $t1, 1
	li   $t3, RB_MAX_SIZE
	div  $t1, $t3
	mfhi $t1
	sw   $t1, 4($a0)

exit_read:
	move  $v0, $t0
	lw    $ra, 8($sp)
	addiu $sp, $sp, 16
	jr    $ra             

# bool rb_write(t_ringbuffer * rbuf, char byte){
#	if(!rbuf_full(rbuf)){
#		rbuf->size++;
#		rbuf->buf[rbuf->wr] = byte;
#		rbuf->wr = (rbuf->wr + 1) % MAX_SIZE;
#		return 1;
#	}
#	return 0;
# }
rb_write:
	addiu $sp, $sp, -8
	sw    $ra, 4($sp)
	
	jal rb_full
	li $t0, 0
	bnez $v0, exit_write
	
	lw $t1, 0($a0)
	addi $t1, $t1, 1
	sw $t1, 0($a0)
	
	lw $t1, 8($a0)
	add $t2, $t1, $a0
	sb $a1, 12($t2)
	
	addi $t1, $t1, 1
	li $t3, 16
	div $t1, $t3
	mfhi $t1
	sw $t1, 8($a0)
	
	li $t0, 1
	
exit_write:
	move $v0, $t0
	lw $ra 4($sp)
	addiu $sp, $sp, 8
	jr $ra
