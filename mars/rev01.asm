#########################################################
# Realize a conversão das expressões abaixo considerando
# que os valores das variáveis já estão carregados nos
# registradores, conforme o mapeamento indicado abaixo
#
# Mapeamento dos registradores:
# f: $t0, g: $t1, h: $t2, i: $t3, j: $t4
# Endereço base A: $s0, Endereço base B: $s1
#########################################################

.data				# vetores
A:	.word 1, 2, 3, 5, 8
B:	.word 13, 21, 34, 55, 89

li $t0, 0			# x = 0
li $t1, 0			# f = 0
li $t2, 2			# g = 2
li $t3, 4			# h = 4
li $t4, 0			# y = 0

######################################
# f = ((g+1) * h) - 3

addi $t1, $t2, 1  		# g + 1
mul $t0, $t0, $t3 		# (g + 1) * h
li $t5, 3         		# $t5 = 3
sub $t1, $t0, $t5		# f = ((g + 1) * h) - 3

######################################
# x = (h*h + 2) / f - g

mul $t0, $t3, $t3		# h * h
addi $t0, $t1, 2		# h * h + 2
sub $t5, $t1, $t2		# f - g
div $t0, $t0, $t5		# (h * h + 2) / f - g

######################################
# B[i] = 2 * A[i] 

li $t6, 1			# i = 1
lw $t5, A($t6)			# A[1] = 2
sll $t5, $t5, 1			# 2 * A[1]
sw $t5, B($t6)			# B[1] = 2 * A[1] = 2

######################################
# B[f+g] = A[i] / (A[j] - B[j])

add $t5, $t1, $t2		# f + g
li  $t6, 1			# i = 1
li  $t7, 2			# j = 2
lw $t8, A($t7)			# $t8 = A[2]
lw $t9, B($t7)			# $t9 = B[2]
sub $t9, $t8, $t9		# A[2] - B[2]
lw $t8, A($t6)			# $t8 = A[1]
div $t8, $t8, $t9		# A[1] / A[2] - B[2]
mflo $t8			# $t8 = A[1] / A[2] - B[2]
sw $t8, B($t5)			# B[f + g] = A[1] / A[2] - B[2]
