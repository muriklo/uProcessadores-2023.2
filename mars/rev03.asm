#########################################################
# Qual é o valor do registrador $s0 após a execução das 
# instruções abaixo? O registrador $s1 possui o valor 
# 0x0000FEFE. Apresente a sua resposta em hexadecimal.
#########################################################

#########################################################


li $s1, 65278			# valor de $s1 em decimal, ou 0b1111 1110 1111 1110 em binário

	add  $s0, $0, $0	# valor do contador iniciado em zero
LOOP:	
	beq  $s1, $0,  DONE	# verifica se o número é igual a zero, para finalizar o programa
	andi $t0, $s1, 0x01	# verifica se o bit menos significativo é igual a 1
	beq  $t0, $0, SKIP	# se o bit menos significativo for zero, o programa pula para SKIP
	addi $s0, $s0, 1	# se for igual a 1, adiciona 1 ao contador
SKIP:
	srl	 $s1, $s1, 1	# realiza um shift right no número, que é o mesmo que dividir por 2
	j    LOOP		# retorna para o loop
DONE:

# pode se observar que o contador irá armazenar o número de bits ativos do número em $s1, logo a soma de 1's do
# número irá revelar o valor final de $s0, que será 14 em decimal, ou 0x0000000E em hexadecimal

