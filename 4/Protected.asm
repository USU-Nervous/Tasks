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
	mov	bx,data
	int	13h
	jc	grp
	;
	cli
	lgdt	[GDTpointer]
	lidt	[IDTpointer]
	
	mov	eax, cr0
	or	al, 1
	mov	cr0, eax

	xor	ax, ax
	mov	es, ax
	mov	ax, 8*3
	mov	ss, ax
	mov	ax, 8*2
	mov	ds,	ax
	jmp	8:next
next:	sti

	jmp	0x1000:0
	cli
	;
	hlt
GDTpointer:
	dw	GDTlen - 1
	dd	7C00h + GDT
IDTpointer:
	dw	256*8
	dd	7C00h + data

GDT:
	times	8 db 0
	db	0x00,0x02,  0x00,0x7C,0x00,  10011000b, 00000000b, 0x00		; для CS
	db	0x00,0x02,  0x00,0x7C,0x00,  10010110b, 00000000b, 0x00		; для DS
	db	0x00,0x0A,  0x00,0x7C,0x00,  10010010b, 00000000b, 0x00		; для SS
	db	0x00,0x10,  0x00,0x80,0x0B,  10010010b, 00000000b, 0x00		; видеопамять
GDTlen db $ - GDT
str_all	db	'An ordinary INT'
str_all_len	dw	$ - str_all
int_all:
	push	cx
	push	si
	mov	cx, [str_all_len]
	mov	si, str_all
	call	print_string
	pop	si
	pop	cx
	iret
str_13	db	'General Protection Fault!!'
str_13_len	dw	$ - str_13
int_13:
	push	cx
	push	si
	mov	cx, [str_13_len]
	mov	si, str_13
	call	print_string
	hlt
	pop	si
	pop	cx
	iret
print_string:
	push	ax
	push	cx
	push	si
	push	di
	push	ds
	push	es
	mov	ax, 8*2
	mov	ds, ax
	mov	ax, 8*4
	mov	es, ax
	mov	di,	0
	mov	al, 04h
loop_1:
	movsb
	stosb
	loop	loop_1
	pop	es
	pop	ds
	pop	di
	pop	si
	pop	cx
	pop	ax
	ret
grp:
	mov ah, 0eh ; сюда попадаем в случае проблем с загрузкой сектора
	mov bl, 11h
	mov bh, 00h
	mov al, 07h ; здесь должно пропищать
	int 10h
	hlt
data:
	times 510 - ($ - $$) db 0
	dw 0xaa55
	
IDT:

	%rep	13
	dw	int_all, 8
	db	0, 10000110b
	dw	0
	%endrep
	
	dw	int_13, 8
	db	0, 10000110b
	dw	0

	%rep	256 - (13+1)
	dw	int_all, 8
	db	0, 10000110b
	dw	0
	%endrep