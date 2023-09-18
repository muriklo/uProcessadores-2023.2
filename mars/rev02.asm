#########################################################
# Realize a conversão das expressões abaixo considerando
# que os valores das variáveis já estão carregados nos
# registradores, conforme o mapeamento indicado abaixo
#
# Mapeamento dos registradores:
# a: $s0, b: $s1, c: $s2, d: $s3
#########################################################

li $s0, 1		# a = 1
li $s1, 2		# b = 2
li $s2, 4		# c = 4
li $s3, 8		# d = 8

#########################################################
# if (a != b) {
#    a = b;
#    if( c < 3 ){
#      a++;
#    } else {
#      for(int i = 3; i < 15; i += 2) {
#         b += i;
#      }
#    }
# }
#########################################################
li, $t0, 3

	beq $s0, $s1, END
	move $s0, $s1
	blt $s2, 3, IF_menor
FOR: 
	beq $t0, 15, END
	add $s1, $t0, $s1
	addi $t0, $t0, 2
	j FOR
IF_menor:
	addi $s0, $s0, 1
END:
