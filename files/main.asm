section .text
	global _start

_start:
	mov edx, 1
	mov ecx, testVar
	mov ebx, 1
	mov eax, 4
	int 0x80
	mov ebx, 1
	int 0x80

section .data
	testVar DB '7'
