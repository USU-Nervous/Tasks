	cli
	jmp	07C0h:x
x:
	mov	ax,cs
	mov	ds,ax
	mov	es,ax
	mov	ss,ax
	mov	sp,0

	mov	ah,02h
	mov	al,01h
	mov	cl,2h
	mov	ch,0h
	mov	dh,0h
	mov	bx,grp
	int	13h
	jnc	grp
	hlt
	
grp:
	mov ah, 0eh ; ���� �������� � ������ ������� � ��������� �������
	mov bl, 11h
	mov bh, 00h
	mov al, 07h ; ����� ������ ���������
	int 10h
	hlt
	times 510 - ($ - $$) db 0
	dw 0xaa55
	
	mov	di,0b800h ;��������� �� ������ �����������
	push	di
	pop	es
	;
	xor	ax,ax
	push	ax
	pop	ds
	mov	ax,[ds:44ch] ; ����� ����� ������ �����������, �.�. ����� ��������
	;
	mov	cx,ax
	shr	ax,4 ; ������ ���-� � ����������
	xchg	bl,bh
	xor	bh,bh
	mul	bx
	push	es
	pop	bx
	add	ax,bx
	push	ax
	pop	es ; �������� � ������� ������ ������� ��������
	push	cs
	pop	ds
	
	; "������" �����
	shr	cx,1
	xor	di,di
	mov	ax,0e020h
	rep	stosw
	;
	

	xor	al,al
	mov	cx,1h
	mov	di,(32+4*80)*2 ;��������� � ������ �������� ���� ������� �������� 
	mov	ah,0d0h ; � �� ����� ����, � �L - ��� �������
_3:
	cmp	al,0ffh
	je	_4
	stosw
	cmp	cx,10h	; ��������� � ��������� ������ �������
	jne _5
	add	di,80*2-16*2
	xor	cx,cx
_5:
	inc	cx
	inc	ax	
	jmp	_3
_4:
	stosw
	hlt
	times 1024 - ($ - $$) db 0