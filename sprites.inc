.text 0x00430000

#  int colors[8];
#  sprite sprites[20];
#  char sprite[49];
#=============================================================
# void draw_sprite(X, Y, sprite_id){
#       char * ptr_sprite =  sprites[sprite_id];
#       for(int i = 0; i < 49; i++){ 
#		color = colors[*ptr_sprite];
#       	sX = i % 7 ;
#		sY = i / 7 ;
#       	sX = X + sX;
#       	sY = Y + sY;
#       	set_pixel(sX, sY, color)
#       	ptr_sprite++;
#       }
# }
## IMP
draw_sprite: # void draw_sprite(X, Y, sprite_id) ($a0, $a1, $a2)
	addiu $sp, $sp, -40
	sw $s0, 12($sp)
	sw $s1, 16($sp)
	sw $s2, 20($sp)
	sw $s3, 24($sp)
	sw $s4, 28($sp)
# <empty space> 32($sp)
	sw $ra, 36($sp)
	
	move $s0, $a0
	move $s1, $a1
	
	la $s2, colors	
	la $s3, sprites
	la $t1, 49
	
	mul $t1, $a2, $t1
	add $s3, $s3, $t1  # $s0 -> * ptr_sprite
	
	li $s4, 0 # int i = 0
	
FOR_draw_sprite:	
	bge $s4, 49, end_FOR_draw_sprite # i >= 49 ? end
	lb  $t2, 0($s3)
	
	sll $t2, $t2, 2		
	add $t2, $t2, $s2	# t1 -> *colors[*ptr_sprite]; 
	lw  $a2, 0($t2)		# $a2 -> color = colors[*ptr_sprite];
	
	li  $t3, 7
	div $s4, $t3	
		
	mfhi $t4		# sX -> i % 7
	mflo $t5		# sY -> i / 7
	add $a0, $s0, $t4	# sX -> X + sX
	add $a1, $s1, $t5	# sY -> Y + sY
	
	jal set_pixel
		
	addiu $s3, $s3, 1	# * ptr_sprite++
	addiu $s4, $s4, 1	# i++
	j FOR_draw_sprite

end_FOR_draw_sprite:
	
	lw $s0, 12($sp)
	lw $s1, 16($sp)
	lw $s2, 20($sp)
	lw $s3, 24($sp)
	lw $s4, 28($sp)
# <empty space> 32($sp)
	lw $ra, 36($sp)
	addiu $sp, $sp, 40
	
	jr   $ra

#=============================================================

#=============================================================
# void draw_animated_sprite(animated_sprite*)
## IMP
draw_animated_sprite:
	# Chama draw_sprite com a posição contida na estrutura
	# animated_sprite
	addiu $sp, $sp, -4
	sw $ra, 0($sp)

	move $t0, $a0
	lw $a0, 4($t0)
	lw $a1, 8($t0)
	lw $a2, 0($t0)
	jal draw_sprite
	
	lw $ra, 0($sp)
	addiu $sp, $sp, 4
	
	jr  $ra
#=============================================================

#=============================================================
# void apply_movement(animated_sprite*)
apply_movement:
	lw   $t0, 4($a0)
	lw   $t1, 12($a0)
	add  $t0, $t0, $t1
	sw   $t0, 4($a0)
	
	lw   $t0, 8($a0)
	lw   $t1, 16($a0)
	add  $t0, $t0, $t1
	sw   $t0, 8($a0)
	jr    $ra
#=============================================================

#=============================================================
# void set_move_animated_sprite(animated_sprite* sp, int vx, int vy)
set_move_animated_sprite:
	# Aplica o vetor de movimento da posicao do sprite
	sw   $a1, 12($a0)
	sw   $a2, 16($a0)
	jr   $ra
#=============================================================

#=============================================================   
# void stop_animated_sprite(animated_sprite* sp)
stop_animated_sprite:
	# zera o vetor de movimento do animated sprite
	sw   $zero, 12($a0)
	sw   $zero, 16($a0)
	jr   $ra
#=============================================================

#=============================================================
# char get_grid_element(row-y, column-x, grid_table* )
get_grid_element:
	## Le o elemento na grid table e retorna
	mul $t0, $a0, GRID_COLS
	add $t0, $t0, $a1
	add $t0, $t0, $a2
	lb  $v0, 0($t0)	
	jr  $ra
#=============================================================

#=============================================================
# draw_grid(width, height, grid_table* )
draw_grid:
	#Percorre a matriz da grid, para cada elemento
	
	## Pegar o id_do_sprite (get_grid_element)
	## converter de ASCII para ID (-64)
	## Chamar draw_sprite(grid pos -> pixel pos - multiplica {X|Y}_SCALE)
	
	addiu $sp, $sp, -40
	sw    $s4, 28($sp)
	sw    $s3, 24($sp)
	sw    $s2, 20($sp)
	sw    $s1, 16($sp)
	sw    $s0, 12($sp)
	sw    $ra, 32($sp)

	move $s0, $a2
	move $s1, $a0
	move $s2, $a1

	li $s3, 0 # ROW
draw_grid_L0:
	beq $s3, $s2, draw_grid_L0_end
	li $s4, 0 # COL
	draw_grid_L1:
		beq  $s4, $s1, draw_grid_L1_end
		
		move $a0, $s3
		move $a1, $s4
		move $a2, $s0
		jal get_grid_element
		move $a2, $v0
		addi $a2, $a2, -64 ## Adjust to idx
		## Convert ROW COL to X, Y
		mul $a0, $s4, X_SCALE
		mul $a1, $s3, Y_SCALE
		jal draw_sprite
		
		addi $s4, $s4, 1
		j draw_grid_L1
	draw_grid_L1_end:
		addi $s3, $s3, 1
		j draw_grid_L0
draw_grid_L0_end:

	lw    $s4, 28($sp)
	lw    $s3, 24($sp)
	lw    $s2, 20($sp)
	lw    $s1, 16($sp)
	lw    $s0, 12($sp)
	lw    $ra, 32($sp)
	addiu $sp, $sp, 40
	
	jr   $ra
#=============================================================

# struct mov_vector {
#    int valid; setar para 1
#    int mov_x;
#    int mov_y;
# }
# W = cima (-1, 0)
# A = esquerda (0, -1)
# S = baixo (1, 0)
# D = direita (0, 1)
#=============================================================
#IMP
# void process_input(mov_vector *)
process_input:
    ## Verificar se key_rb está vazio
    ## Se estiver, retornar
	
	## Senao
	## Le valor do ringbuffer
	
	## Processa valor com teclas associadas ao
	## movimento. Setar mov_vector e tornar
	## válido	
	addiu $sp, $sp, -16
	sw $s0, 4($sp)
	sw $ra, 8($sp)
	move $s0, $a0
	
	la $a0, key_rb
	jal rb_empty
	
	li $t0, 1
	beq $v0, $t0, empty_process_input
	jal rb_read
	
	li $t2, 'w'
	li $t3, 'a'
	li $t4, 's'
	li $t5, 'd'
	bne $v0, $t2, skip_w_process_input
	li $t0, -1 # W = cima (-1, 0)
	li $t1, 0			
	j move_vec

skip_w_process_input:
	bne $v0, $t3, skip_a_process_input
	li $t0, 0 # A = esquerda (0, -1)
	li $t1, -1			
	j move_vec
	
skip_a_process_input:	
	bne $v0, $t4, skip_s_process_input
	li $t0, 1 # S = baixo (1, 0)
	li $t1, 0			
	j move_vec
	
skip_s_process_input:	
	bne $v0, $t5, empty_process_input
	li $t0, 0 # D = direita (0, 1)
	li $t1, 1			
	j move_vec
	
move_vec:
	sw $t0, 4($s0)
	sw $t1, 8($s0)
	li $t0, 1
	sw $t0, 0($s0)

empty_process_input:
	lw $s0, 4($sp)
	lw $ra, 8($sp)
	addiu $sp, $sp, 16
	jr   $ra
#=============================================================

#=============================================================   
# void try_change_movement(animated_sprite* sp, mov_vector* mv)
try_change_movement:
	## Verifica se mov_vector é válido
	## Se não, retorna.

	## Converte posição pixel para posicao grid
	## Lembrete - se resto divisao == 0, está alinhado
	
	## Se não está alinhado, não muda o movimento.
	## Retorna função.
	
	## Se alinhado, verifica se pode se mover para nova
	## direção
	
	## Calculo nova posicao somando vetor movimento
	## com posição do grid e pego elemento do grid
	## para ver se não é parede.
	
	## Se não for parede, aplica vetor de movimento
	## no sprite, caso contrário não faz nada
	## Não esquecer de tornar o mov_vector inválido
	## (valid = 0)
	addi $sp, $sp, -24
	sw   $s0, 12($sp)
	sw   $s1, 16($sp)
	sw   $ra, 20($sp)

	move $s0, $a0
	move $s1, $a1

	lw   $t0, 0($s1)
	beqz $t0, try_change_movement_end

	lw 	 $a1, 4($s0)        # Pixel X
	div  $a1, $a1, X_SCALE  # Column
	mfhi $t0                # If == 0 - root pos
	
	lw 	 $a0, 8($s0)
	div  $a0, $a0, Y_SCALE  # Row
	mfhi $t1				 # If == 0 - root pos
	
	add  $t0, $t0, $t1
	bnez $t0, try_change_movement_end
	
	#Can change direction?
	lw 	 $t0, 4($s1)
	add  $a1, $a1, $t0		# +mov_x
	lw 	 $t0, 8($s1)
	add  $a0, $a0, $t0		# +mov_y
	la   $a2, grid
	jal  get_grid_element
	
	bge  $v0, 69, try_change_movement_end
	
	# change direction
	lw 	 $t0,    4($s1)
	sw   $t0,   12($s0)
	lw 	 $t0,    8($s1)
	sw   $t0,   16($s0)
	sw   $zero,  0($s1)
	
try_change_movement_end:	
	lw   $s0, 12($sp)
	lw   $s1, 16($sp)
	lw   $ra, 20($sp)
	addi $sp, $sp, 24	
			
	jr   $ra
#=============================================================

#=============================================================
# bool check_movement(animated_sprite*)
check_movement:
	## Converte posição pixel para posicao grid
	## Lembrete - se resto divisao == 0, está alinhado
	
	## Se não está alinhado, pode se mover.
	## Retorna verdadeiro
	
	## Calculo nova posicao somando vetor movimento
	## com posição do grid e pego elemento do grid
	## para ver se não é parede.
	
	## Se não é parede retorna verdadeiro
	
	## Se for parede, para o sprite (stop_animated_sprite)
	## e retorna falso.
	addi $sp, $sp, -24
	sw   $s0, 12($sp)
	sw   $ra, 16($sp)

	move $s0, $a0

	lw 	 $a1, 4($s0)        # Pixel X
	div  $a1, $a1, X_SCALE  # Column
	mfhi $t0                # If == 0 - root pos
	
	lw 	 $a0, 8($s0)
	div  $a0, $a0, Y_SCALE  # Row
	mfhi $t1				 # If == 0 - root pos
	
	add  $t0, $t0, $t1
	bnez $t0, check_movement_can_move
	
	lw 	 $t0, 12($s0)
	add  $a1, $a1, $t0		# +mov_x
	lw 	 $t0, 16($s0)
	add  $a0, $a0, $t0		# +mov_y
	la   $a2, grid
	jal  get_grid_element
	
	blt  $v0, 69, check_movement_can_move
	
	move $a0, $s0
	jal  stop_animated_sprite
	li   $v0, 0
	
	j    check_movement_end
		
check_movement_can_move:
	li   $v0, 1

check_movement_end:	
	lw   $s0, 12($sp)
	lw   $ra, 16($sp)
	addi $sp, $sp, 24
	
	jr   $ra
#=============================================================

