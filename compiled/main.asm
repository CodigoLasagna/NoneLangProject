section .bss
	res resb 4

section .text
	global _start

_start:
	mov ecx, [var]
	mov edx, 2
	mov ecx, var
	mov ebx, 1
	mov eax, 4
	int 0x80
	
	mov eax, 1
	int 80h ;

section .data
	var dd '12'