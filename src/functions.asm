iprint:
	push 	rax ;on preserve rax
	push	rcx ;pareil
	push	rdx ;...
	push	rsi ;...
	mov	rcx, 0 ; compteur du nombre de caracteres a afficher 

divideLoop:
	inc	rcx ; incrémente le compteur de caractère de 1
	mov	rdx, 0 ; place 0 dans rdx ( qui sera le reste de la div ) 
	mov	rsi, 10 ; place 10 dans rsi ( ce par quoi on va div )
	idiv	rsi ; on divise rax par rsi le reste sera dans rdx
	add	rdx, 48 ; on ajoute 48 au reste
	push	rdx ; on conserve la valeur de rdx
	cmp	rax, 0 ; on verifie qu'on ne soit pas à la fin de la chaine
	jnz	divideLoop ; boucle

printLoop:
	dec 	rcx ; on decremente le compteur de caracteres
	mov 	rax, rsp ; on mets dans rax l'adresse du haut de la pile
	call 	sprint ; call notre fonction print
	pop 	rax ; remove le dernier caractere de la pile
	cmp 	rcx, 0 ; verifier qu'il ya toujours des caracteres a print
	jnz	printLoop ; si oui print
	
	pop 	rsi ; sinon on recupere ttes les valeurs stockées sur la pile
	pop 	rdx
	pop	rcx;
	pop	rax;
	ret

iprintLF:
	call iprint
	
	push	rax 
	mov	rax, 0Ah 
	push	rax ; On rajoute le caractere de fin de ligne sur la pile 
	mov	rax, rsp ; Recupere ládresse de la pile
	call	sprint ; print
	pop	rax ; on enleve notre caractere de fin de ligne
	pop	rax ; et notre ancien rax
	ret
	

slen:
	push	rbx
	mov	rbx, rax

nextchar:
	cmp	byte [rax], 0
	jz	finished
	inc	rax
	jmp	nextchar

finished:
	sub 	rax, rbx
	pop 	rbx
	ret

sprint:
	push 	rdx
	push 	rcx
	push 	rbx
	push 	rax
	call 	slen

	mov 	rdx, rax
	pop	rax

	mov 	rcx, rax
	mov 	rbx, 1
	mov 	rax, 4
	int 	80h
	
	pop	rbx
	pop	rcx
	pop	rdx
	ret

sprintLF:
	call 	sprint
	push	rax ; on push le message
	mov	rax, 0Ah
	push	rax ; on push le caractere fin de ligne
	mov 	rax, rsp; rsp pointe sur le dernier element rajouté sur la pile
	call	sprint
	pop 	rax
	pop	rax
	ret

quit:
	mov 	rbx, 0
	mov	rax, 1
	int	80h
	ret
