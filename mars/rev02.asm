#########################################################
# Realize a conversão das expressões abaixo considerando
# que os valores das variáveis já estão carregados nos
# registradores, conforme o mapeamento indicado abaixo
#
# Mapeamento dos registradores:
# a: $s0, b: $s1, c: $s2, d: $s3
#########################################################

li $s0, 1			# a = 1
li $s1, 2			# b = 2
li $s2, 4			# c = 4
li $s3, 8			# d = 8

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

	li, $t0, 3		# $t0 carregado com 3 para comparar com C ou inicializar o FOR

	beq $s0, $s1, END	# verifica se a!= b, caso igual, encerra o programa
	move $s0, $s1		# realiza a = b;
	blt $s2, 3, IF_menor	# verifica se c < 3, caso menor, pula para IF_menor
FOR: 
	bge $t0, 15, END	# condição para finalizar o loop do for, quando i >= 15
	add $s1, $t0, $s1	# realiza b += i;
	addi $t0, $t0, 2	# incrementa i += 2;
	j FOR			# retorna o loop do FOR
IF_menor:
	addi $s0, $s0, 1	# incrementa a++;
END:
