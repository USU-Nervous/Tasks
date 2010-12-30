global _add
_add:
	;mov	ax,11
	xor	eax,eax
	;
	push	ebp
	mov	ebp,esp
	add	eax,[ebp + 12]
	add	eax,[ebp + 8]
	pop	ebp
	;
	ret