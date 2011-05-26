bits 16
extern c_handler
global start
start:	
	cli

	mov	ax,cs
	mov	ds,ax
	mov	es,ax
	mov	ss,ax
	mov	sp,0
	;

	mov	ah,02h
	mov	al,06h
	mov	cl,2h
	mov	ch,0h
	mov	dh,0h
	mov	bx,data
	int	13h
	
	jc	grp
	
	cli

	lgdt	[GDTpointer]
	lidt	[IDTpointer]

	mov	eax, cr0
	or	al, 1
	mov	cr0, eax
	
	jmp	8:next
bits 32
next:
	mov	eax, 0x10
	mov	es, eax
	mov	ss, eax
	mov	ds, eax
	mov	sp, 0xFFFF
	
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

;	cli
;	hlt
	sti
;	call	c_handler

cycle:
	hlt
	jmp	cycle
mask_1	db	0
mask_2	db	0
color_mask	db	2
GDTpointer:
	dw	GDTlen - 1
	dw	GDT
	dw	0
IDTpointer:
	dw	256*8
	dd	data

GDT:
	times	8 db 0
	db	0xFF,0xFF,0x00,0x00,0x00,0x9A,0xCF,0x00
	db	0xFF,0xFF,0x00,0x00,0x00,0x92,0xCF,0x00
	db	0x00,0x02,  0x00,0x00,0x00,  10011000b, 00000000b, 0x00
	db	0x00,0x02,  0x00,0x00,0x00,  10010110b, 00000000b, 0x00
	db	0x00,0x0A,  0x00,0x00,0x00,  10010010b, 00000000b, 0x00
	db	0x00,0xf0,  0x00,0x80,0x0B,  10010010b, 00000000b, 0x00
GDTlen db $ - GDT
int_all:
	iret
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
	call	c_handler
	in	al,60h
	mov	al,20h	; 
	out	20h,al	; 
	iret
grp:
	mov ah, 0eh
	mov bl, 11h
	mov bh, 00h
	mov al, 07h
	int 10h
	hlt
	times 510 - ($ - $$) db 0
	dw 0xaa55
	
data:

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
	db	0, 10001110b
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
