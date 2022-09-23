#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bools.h"

bool parser(char*, FILE*, FILE*);

/*Lexicon*/
bool lexDecVar(char*);
bool lexDelimiter(char);
bool lexMathOperator(char);
bool lexLogOperator(char);
bool lexNumber(char);
bool lexKeyword(char*);
bool finalDelimiter(char);

void writeVariable(char*, int, char*, FILE*, FILE*);

int main(){
	FILE *fdata = fopen("./compiled/src/data.src", "r+"), *fstart = fopen("./compiled/src/start.src", "r+"), *fsrc = fopen("./testproject/main.nl", "r+");
	FILE *fasm = fopen("./compiled/main.asm", "w+");
	char* sourceBuffer = 0, *dataBuffer = 0, *startBuffer = 0;
	long length = 0;
	if (fsrc){
		fseek(fsrc, 0, SEEK_END);
		length = ftell(fsrc);
		fseek(fsrc, 0, SEEK_SET);
		sourceBuffer = malloc(length+1);
		if (sourceBuffer){
			fread(sourceBuffer, 1, length, fsrc);
		}
		fclose(fsrc);
		sourceBuffer[length] = '\0';
	}
	freopen(NULL,"w+", fstart);
	freopen(NULL,"w+", fdata);
	freopen(NULL,"w+", fasm);
	fprintf(fstart, "section .bss\n");
	fprintf(fstart, "	res resb 4\n\n");
	fprintf(fstart, "section .text\n	global _start\n\n");
	fprintf(fdata, "section .data\n");
	fprintf(fstart, "_start:\n");
	if (fdata == NULL || fsrc == NULL){
		printf("error");
		exit(1);
	}

	printf("%s\n\n", sourceBuffer);
	parser(sourceBuffer, fdata, fstart);
	fprintf(fstart, "	mov eax, 1\n");
	fprintf(fstart, "	int 80h ;\n\n");

	if (fstart){
		length = 0;
		fseek(fstart, 0, SEEK_END);
		length = ftell(fstart);
		fseek(fstart, 0, SEEK_SET);
		startBuffer = malloc(length+1);
		if (startBuffer){
			fread(startBuffer, 1, length, fstart);
		}
		fclose(fstart);
		startBuffer[length] = '\0';
	}
	if (fdata){
		length = 0;
		fseek(fdata, 0, SEEK_END);
		length = ftell(fdata);
		fseek(fdata, 0, SEEK_SET);
		dataBuffer = malloc(length+1);
		if (dataBuffer){
			fread(dataBuffer, 1, length, fdata);
		}
		fclose(fdata);
		dataBuffer[length] = '\0';
	}

	fprintf(fasm, "%s", startBuffer);
	fprintf(fasm, "%s", dataBuffer);

	return 0;
}

bool parser(char* input, FILE* fdata, FILE* fstart){
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
						varValue[digits] = word[digits];
						digits++;
					}
					varValue[digits] = '\0';
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
							printf("%s", varName);
							for (j = 0; word[j] != '\0'; j++){
								varName[j] = word[j];
							}
							varName[j] = '\0';
							printf("%s", varName);
							variables--;
						}else{
							printf("[error]");
						}
					}
					if (number == true){
						printf("[number(%i)]", digits);
						writeVariable(varName, digits, varValue, fdata, fstart);
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

bool lexDelimiter(char ch){
	if (
		ch == '+' || ch == '-' || ch == '=' || ch == ';' || ch == ' ' || ch == '"' || ch == '\n'
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
		strcmp(word, "i4") == 0 || strcmp(word, "fn") == 0 || strcmp(word, "put") == 0
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

void writeVariable(char* variable, int digCount, char* value, FILE* fdata, FILE* fstart){
	fprintf(fstart, "	mov ecx, [%s]\n", variable);
	
	fprintf(fstart, "	mov edx, %d\n", digCount);
	fprintf(fstart, "	mov ecx, %s\n", variable);
	fprintf(fstart, "	mov ebx, 1\n");
	fprintf(fstart, "	mov eax, 4\n");
	fprintf(fstart, "	int 0x80\n\t\n");

	fprintf(fdata, "	%s dd '%s'\n", variable, value);
}
