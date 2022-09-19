#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bools.h"

bool parser(char*, FILE*);

/*Lexicon*/
bool lexDecVar(char*);
bool lexDelimiter(char);
bool lexMathOperator(char);
bool lexLogOperator(char);
bool lexNumber(char);
bool lexKeyword(char*);
bool finalDelimiter(char);

void writeVariable(char*, int, char*, FILE*);

int main(){
	FILE *fptr;
	char* input = "i4 testVar = 7;";
	fptr = fopen("./files/main.asm", "w+");
	/*fprintf(fptr, "	.global _start\n\n	.text\n");
	fprintf(fptr, "_start:\n");*/
	/*fprintf(fptr, "section .data\n");*/
	/*fprintf(fptr, "section .text\n	global _start\n\n");*/
	fprintf(fptr, "section .text\n	global _start\n\n");
	fprintf(fptr, "_start:\n");
	if (fptr == NULL){
		printf("error");
		exit(1);
	}
	printf("%s\n\n", input);
	parser(input, fptr);
	fclose(fptr);
	return 0;
}

bool parser(char* input, FILE* fptr){
	unsigned counter = 0, i = 0, j = 0, digits = 0, variables = 0;
	char* varValue = malloc(sizeof(char) * 128);
	char* word = malloc(sizeof(char) * 1024);
	char* varName = malloc(sizeof(char) * 128);
	char c;
	bool number = false;
	for (c = *input; c != '\0'; c = input[++i]){
		if (lexDelimiter(c)){
			word[counter] = '\0';
			if (counter > 0){
				printf("%s", word);
				if (lexNumber(word[digits])){
					while(lexNumber(word[digits])){
						varValue[digits] = '\0';
						varValue[digits] = word[digits];
						digits++;
					}
					number = true;
				}
				if (lexKeyword(word)){
					printf("[keyword]");
					if (lexDecVar(word)){
						variables++;
					}
				}else{
					if (number == false ){
						if (variables > 0){
							printf("[variable]");
							/*fprintf(fptr, "	%s DW \n", word);*/
							printf("%s", varName);
							for (j = 0; word[j] != '\0'; j++){
								varName[j] = word[j];
							}
							printf("%s", varName);
							variables--;
						}else{
							printf("[error]");
						}
					}
					if (number == true){
						printf("[number(%i)]", digits);
						writeVariable(varName, digits, varValue, fptr);
						number = false;
						digits = 0;
					}
				}
			}
			if (finalDelimiter(c)){
				printf("%c", c);
				printf("[end of sentence]");
			}
			if (lexMathOperator(c)){
				printf("%c", c);
				printf("[Math operator]");
			}
			if (lexLogOperator(c)){
				printf("%c", c);
				printf("[Logic operator]");
			}
			printf("\n");
			counter = 0;
		}else{
			word[counter] = c;
			counter++;
		}
	}
	free(word);
	return false;
}
bool parser2(char* input){
	unsigned counter = 0, i = 0;
	char* word = malloc(sizeof(char) * 1024);
	bool var = false;
	int variables = 0;
	char c;
	for (c = *input; c != '\0'; c = input[++i]){
		if (lexDelimiter(c) == true){
			word[counter] = '\0';
			printf("%s", word);
			if (lexKeyword(word) == true){
				putchar(c);
				printf("< keyword");
				var = true;
			}else {
				if (counter > 0)
					variables++;
				if (var == true && variables > 0){
					printf("< variable");
					var = false;
					variables--;
				}else if (var == false && variables > 0){
					printf("< error");
					variables--;
				}
			}
			if (lexLogOperator(c)){
				putchar(c);
				printf("< logic operator");
			}
			if (lexMathOperator(c)){
				putchar(c);
				printf("< math operator");
			}
			if (lexNumber(c) && var == false){
				putchar(c);
				printf("< number");
			}
			if (finalDelimiter(c)){
				putchar(c);
				printf("< end of sentence");
			}
			printf("\n");
			counter = 0;
		}else{
			word[counter] = c;
			counter++;
		}
	}
	free(word);
	return false;
}

bool lexDelimiter(char ch){
	if (
		ch == '+' || ch == '-' || ch == '=' || ch == ';' || ch == ' ' || ch == '"'
		)
	{
		return true;
	}
	return false;
}
bool finalDelimiter(char ch){
	if (
		ch == ';'
		)
	{
		return true;
	}
	return false;
}

bool lexMathOperator(char ch){
	if (
		ch == '+' || ch == '-'
		)
	{
		return true;
	}
	return false;
}

bool lexLogOperator(char ch){
	if (
		ch == '=' || ch == '&'
		)
	{
		return true;
	}
	return false;
}

bool lexNumber(char ch){
	if (
		ch == '0' || ch == '1' || ch == '2' ||
		ch == '3' || ch == '4' || ch == '5' ||
		ch == '6' || ch == '7' || ch == '8' || ch == '9'
		)
	{
		return true;
	}
	return false;
}

bool lexKeyword(char* word){
	if (
		strcmp(word, "i4") == 0 || strcmp(word, "fn") == 0
		)
	{
		return true;
	}
	return false;
}

bool lexDecVar(char* word){
	if (
		strcmp(word, "i2") == 0 || strcmp(word, "i4") == 0
		)
	{
		return true;
	}
	return false;
}

void writeVariable(char* variable, int digCount, char* value, FILE* fptr){
	fprintf(fptr, "	mov edx, %d\n", digCount);
	fprintf(fptr, "	mov ecx, %s\n", variable);
	fprintf(fptr, "	mov ebx, 1\n");
	fprintf(fptr, "	mov eax, 4\n");
	fprintf(fptr, "	int 0x80\n");
	fprintf(fptr, "	mov ebx, 1\n");
	fprintf(fptr, "	int 0x80\n\n");

	fprintf(fptr, "section .data\n");
	fprintf(fptr, "	%s DB '%s'", variable, value);
}
