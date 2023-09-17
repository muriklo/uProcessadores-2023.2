#########################################################
# Realize a conversão das expressões abaixo considerando
# que os valores das variáveis já estão carregados nos
# registradores, conforme o mapeamento indicado abaixo
#
# Mapeamento dos registradores:
# f: $t0, g: $t1, h: $t2, i: $t3, j: $t4
# Endereço base A: $s0, Endereço base B: $s1
#########################################################

######################################
# f = ((g+1) * h) - 3
.data 
g: .word
h: .word
lw $t0, g      # Carrega g em $t0
lw $t1, h      # Carrega h em $t1

# f = ((g+1) * h) - 3
addi $t0, $t0, 1  # g+1
mul $t0, $t0, $t1 # (g+1) * h
li $t2, 3         # 3
sub $t0, $t0, $t2 # ((g+1) * h) - 3

######################################
# f = (h*h + 2) / f - g

######################################
# B[i] = 2 * A[i] 

######################################
# B[f+g] = A[i] / (A[j] - B[j])
