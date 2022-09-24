section .bss
	res resb 4

section .text
	global _start

_start:
	mov ecx, '3'
	mov [varA], ecx
	
	mov edx, 1
	mov ecx, varA
	mov ebx, 1
	mov eax, 4
	int 0x80
	
	mov edx, 1
	mov ecx, newLine
	mov ebx, 1
	mov eax, 4
	int 0x80
	
	mov ecx, [varA]
	mov [varA], ecx
	
	mov ecx, [varA]
	sub ecx, '0'
	mov ebx, '2'
	sub ebx, '0'
	mov eax, ecx
	add eax, ebx
	add eax, '0'
	mov [varA], eax

	mov edx, 1
	mov ecx, varA
	mov ebx, 1
	mov eax, 4
	int 0x80
	
	mov eax, 1
	int 80h ;

section .data
	newLine db 0x0a
	varA dw '00', 0x0
