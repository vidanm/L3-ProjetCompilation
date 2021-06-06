iprint:
	push 	eax ;on preserve eax
	push	ecx ;pareil
	push	edx ;...
	push	esi ;...
	mov	ecx, 0 ; compteur du nombre de caracteres a afficher 

divideLoop:
	inc	ecx ; incrémente le compteur de caractère de 1
	mov	edx, 0 ; place 0 dans edx ( qui sera le reste de la div ) 
	mov	esi, 10 ; place 10 dans esi ( ce par quoi on va div )
	idiv	esi ; on divise eax par esi le reste sera dans edx
	add	edx, 48 ; on ajoute 48 au reste
	push	edx ; on conserve la valeur de edx
	cmp	eax, 0 ; on verifie qu'on ne soit pas à la fin de la chaine
	jnz	divideLoop ; boucle

printLoop:
	dec 	ecx ; on decremente le compteur de caracteres
	mov 	eax, esp ; on mets dans eax l'adresse du haut de la pile
	call 	sprint ; call notre fonction print
	pop 	eax ; remove le dernier caractere de la pile
	cmp 	ecx, 0 ; verifier qu'il ya toujours des caracteres a print
	jnz	printLoop ; si oui print
	
	pop 	esi ; sinon on recupere ttes les valeurs stockées sur la pile
	pop 	edx
	pop	ecx;
	pop	eax;
	ret

iprintLF:
	call iprint
	
	push	eax 
	mov	eax, 0Ah 
	push	eax ; On rajoute le caractere de fin de ligne sur la pile 
	mov	eax, esp ; Recupere ládresse de la pile
	call	sprint ; print
	pop	eax ; on enleve notre caractere de fin de ligne
	pop	eax ; et notre ancien eax
	ret
	

slen:
	push	ebx
	mov	ebx, eax

nextchar:
	cmp	byte [eax], 0
	jz	finished
	inc	eax
	jmp	nextchar

finished:
	sub 	eax, ebx
	pop 	ebx
	ret

sprint:
	push 	edx
	push 	ecx
	push 	ebx
	push 	eax
	call 	slen

	mov 	edx, eax
	pop	eax

	mov 	ecx, eax
	mov 	ebx, 1
	mov 	eax, 4
	int 	80h
	
	pop	ebx
	pop	ecx
	pop	edx
	ret

sprintLF:
	call 	sprint
	push	eax ; on push le message
	mov	eax, 0Ah
	push	eax ; on push le caractere fin de ligne
	mov 	eax, esp; esp pointe sur le dernier element rajouté sur la pile
	call	sprint
	pop 	eax
	pop	eax
	ret

quit:
	mov 	ebx, 0
	mov	eax, 1
	int	80h
	ret
