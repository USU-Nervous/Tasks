	cli
	jmp	07C0h:x
x:
	mov	ax,cs
	mov	ds,ax
	mov	es,ax
	mov	ss,ax
	mov	sp,0
	;
	mov	al,00010001b		;
	out	20h,al		;даем команду на инициализацию
	out	0a0h,al		;
	;
	mov	al,20h		;смещаем вектора
	out	21h,al		;
	mov	al,28h		;
	out	0a1h,al		;
	;
	mov	al,2		;устанавливаем линию "второго контроллера в каскаде"
	out	21h,al		;
	mov	al,2		;устанавливаем "уровень второго контроллера"
	out	0a1h,al		;
	;
	mov	al,00000101b;установим тип процессора, режим контроллера(slave/master),
	out	21h,al		;
	mov	al,00000001b;
	out	0a1h,al		;
	;
	mov	al,11111101b		;оставим только клавиатуру
	out	21h,al		;
	mov	al,0ffh		;
	out	0a1h,al		;
	;
	mov	ah,02h
	mov	al,01h
	mov	cl,2h
	mov	ch,0h
	mov	dh,0h
	mov	bx,data
	int	13h
	jc	grp
	;
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
next:
	sti
cycle:
	hlt
	jmp	cycle
mask_1	db	0
mask_2	db	0
color_mask	db	2
GDTpointer:
	dw	GDTlen - 1
	dd	7C00h + GDT
IDTpointer:
	dw	256*8
	dd	7C00h + data

GDT:
	times	8 db 0
	db	0x00,0x02,  0x00,0x7C,0x00,  10011000b, 00000000b, 0x00
	db	0x00,0x02,  0x00,0x7C,0x00,  10010110b, 00000000b, 0x00
	db	0x00,0x0A,  0x00,0x7C,0x00,  10010010b, 00000000b, 0x00
	db	0x00,0xf0,  0x00,0x80,0x0B,  10010010b, 00000000b, 0x00
GDTlen db $ - GDT
int_all:
	iret
int_hlt:
	cli
	hlt
int_hard:
	mov	al,20h
	out	20h,al
	iret
int_hard_2:
	mov	al,20h
	out	20h,al
	out	0a0h,al
	iret
int_button:
	mov	ax, 8*4
	mov	es, ax
	;
	add	di,cx
	;
	mov	di, 1280
	mov	cx,	0eh
_1:
	mov	al, 01h
	stosb
	mov	al, [color_mask]
	stosb
	inc	al
	mov	[color_mask], al
	loop	_1
	;
	in	al,60h
	mov	al,20h	; даем понять контроллеру, что прерывание обработано 
	out	20h,al	; и в ISR можно скинуть старшую единицу
	iret
grp:
	mov ah, 0eh
	mov bl, 11h
	mov bh, 00h
	mov al, 07h
	int 10h
	hlt
data:
	times 510 - ($ - $$) db 0
	dw 0xaa55
	
IDT:
	%rep	32
	dw	int_all, 8
	db	0, 10000110b
	dw	0
	%endrep
	dw	int_hard, 8
	db	0, 10000110b
	dw	0
	dw	int_button, 8
	db	0, 10000110b
	dw	0
	%rep	6
	dw	int_hard, 8
	db	0, 10000110b
	dw	0
	%endrep
	%rep	8
	dw	int_hard_2, 8
	db	0, 10000110b
	dw	0
	%endrep
	%rep	208
	dw	int_all, 8
	db	0, 10000110b
	dw	0
	%endrep
