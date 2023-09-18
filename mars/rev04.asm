#########################################################
# Paridade de uma palavra pode ser definida como par ou 
# ímpar, de acordo com a quantidade de bits “1” presentes 
# na palavra. Por exemplo, a palavra 0b0110 possui 
# paridade par, pois possui 2 bits ativados, enquanto que 
# a palavra 0b1110 possui paridade ímpar uma vez que possui 
# 3 bits ativados. Projete um programa em linguagem assembly 
# do MIPS para calcular a paridade da palavra presente no 
# endereço de memória 0x10018000. Armazene o valor 0x01 
# caso a paridade seja par, ou 0x02 caso seja ímpar no 
# endereço de memória 0x10018004.
#
# Utilize chamadas de sistemas para inicializar o valor 
# inicial da palavra armazenada no endereço 0x10018000.
#
#########################################################
.macro lerInt (%reg)
    li $v0, 5 
    move $a0, %reg
    syscall
    move %reg, $v0 
.end_macro

.macro imprimirInt (%reg)
    li $v0, 1       		# carrega o código do serviço de sistema para imprimir uma string em $v0
    add $a0, $zero, %reg    	# carrega string ao registrador $a0
    syscall            		# chama o serviço de sistema para imprimir a string
.end_macro

.macro imprimirString (%str)
    .data
    string: .asciiz %str
    .text
    li $v0, 4        		# carrega o código do serviço de sistema para imprimir uma string em $v0
    la $a0, string        	# carrega string ao registrador $a0
    syscall            		# chama o serviço de sistema para imprimir a string
.end_macro

.data 0x10018000
palavra: 	.word 0		# palavra a ser analisada
result:		.word 0		# reserva para o resultado (0x10018004)

.text
	la $gp, 0x10018000	# carrega o endereço da palavra no global pointer
	lw $s0, 0($gp)		# carrega palavra no ponteiro global
	la $t0, 0		# contador de bits iniciado em zero
	
	imprimirString("Insira o valor inteiro: ")	#chamada para imprimir o texto de solicitação
	lerInt($s0)		# lê o valor inserido no console pelo usuário
loop:
	andi $t3, $s0, 0x01	# verifica se o bit menos significativo é igual a 0x01
	beqz $t3, pula_incremento	# se for igual a zero, pula incremento do contador
	addi $t0, $t0, 1	# adiciona 1 no contador
pula_incremento:
	srl $s0, $s0, 1		# shift right na palavra (divide por 2)
	beqz $s0, end		# se a palavra foi igual a zero, encerra o programa
	j loop			# retorna para o loop
end:
	imprimirString("\nResultado da paridade: ")
	imprimirInt($t0)
	rem $t0, $t0, 2
	bnez $t0, impar
	li $t1, 0x01
	lw $t1, 4($gp)
	imprimirString("\n\nA paridade de bits ativos é par\n\n")
	j encerra
impar:
	li $t1, 0x02
	lw $t1, 4($gp)
	imprimirString("\n\nA paridade de bits ativos é ímpar\n\n")
encerra:
