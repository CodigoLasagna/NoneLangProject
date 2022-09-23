section .bss
	res resb 4

section .text
	global _start

_start:
	mov ecx, '3'
	mov [a], ecx
	
	mov ecx, '5'
	mov [b], ecx
	
	mov edx, 1
	mov ecx, a
	mov ebx, 1
	mov eax, 4
	int 0x80
	
	mov edx, 1
	mov ecx, newLine
	mov ebx, 1
	mov eax, 4
	int 0x80
	
	mov edx, 1
	mov ecx, b
	mov ebx, 1
	mov eax, 4
	int 0x80
	
	mov eax, 1
	int 80h ;

section .data
	newLine db 0x0a
	a dw '01', 0x0
	b dw '01', 0x0
