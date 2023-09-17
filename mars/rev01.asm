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
A:	.word 1 , 2 , 3 , 5 , 8
B:	.word 13, 21, 34, 55, 89

.text 
li $t0, 0			# f = 0
li $t1, 1			# g = 1
li $t2, 2			# h = 2
li $t3, 4			# i = 4
li $t4, 5			# j = 5

######################################
# f = ((g+1) * h) - 3

addi $t1, $t1, 1  		# g + 1
mul $t0, $t1, $t2 		# (g + 1) * h
li $t5, 3         		# $t5 = 3
sub $t0, $t0, $t5		  # f = ((g + 1) * h) - 3

######################################
# f = (h*h + 2) / f - g

mul $t5, $t2, $t2		# h * h
addi $t5, $t5, 2		# h * h + 2
sub $t6, $t0, $t1		# f - g
div $t0, $t5, $t6		# f = (h * h + 2) / f - g

######################################
# B[i] = 2 * A[i] 

sll $t6, $t3, 2			# i * 4
la $t5, A			
add $t5, $t5, $t6		# endereço + 4 para acessar A[1]
lw $t5, 0($t5)			# $t5 = A[1]
sll $t5, $t5, 1			# 2 * A[1]
la $t7, B			      # carrega endereço base de B
add $t6, $t6, $t7		# endereço + 4 para acessar B[1]
sw $t5, 0($t6)			# B[1] = 2 * A[1]

######################################
# B[f+g] = A[i] / (A[j] - B[j])

add $t5, $t0, $t1		# f + g
sll $t5, $t5, 2			# 4 * (f + g)
sll $t6, $t3, 2			# i * 4
sll $t7, $t4, 2			# j * 4

la $t8, A			      # endereço base de A
add $t9, $t8, $t7		# endereço + 4 * j para acessar A[j]
lw $t8, 0($t9)			# $t8 = A[j]

la $t9, B			      # endereço base de B
add $t9, $t9, $t7		# endereço + 4 * j para acessar B[j]
lw $t9, 0($t9)			# $t9 = B[j]

sub $t9, $t8, $t9		# A[j] - B[j]

la $t8, A
add $t8, $t8, $t6		# endereço + 4 * i para acessar A[i]
lw $t8, 0($t8)			# $t8 = A[i]

div $t8, $t8, $t9		# A[i] / A[j] - B[j]
mflo $t8			      # $t8 = A[i] / A[j] - B[j]

la $t6, B			      # endereço base de B
add $t6, $t6, $t5		# endereço + 4 * (f + g) para acessar B[f + g]
sw $t8, 0($t6)			# B[f + g] = A[i] / A[j] - B[j]
