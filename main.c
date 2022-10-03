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

void writeVariable(char*, int, FILE*, FILE*);
void assignVariable(char*, char*, FILE*, bool);
void printVariable(char*, FILE*);

void sumVars(char*, char*, FILE*);

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
	fprintf(fdata, "\tnewLine db 0x0a\n");
	
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
	unsigned counter = 0, i = 0, j = 0, digits = 0, variables = 0, varCounter = 0;
	unsigned varBits = 0;
	char* varValue = malloc(sizeof(char) * 128);
	char* word = malloc(sizeof(char) * 1024);
	char* varName = malloc(sizeof(char) * 128);
	char* varNameAsign = malloc(sizeof(char) * 128);
	char c;
	bool number = false, assign = false, print = false, sum = false;
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
						varBits = word[1] - '0';
					}
					if (strcmp(word, "put") == 0){
						print = true;
					}
				}else{
					if (number == false ){
						if (variables > 0){
							printf("[variable]");
							strcpy(varName, word);
							printf("%s", varName);
							writeVariable(varName, varBits, fdata, fstart);
							variables--;
						}else{
							if (print == false){
								printf("[error]");
								if (varCounter == 1){
									strcpy(varNameAsign, varName);
									varCounter = 0;
								}
								varCounter++;
								strcpy(varName, word);
							}else{
								printf("[variableToWrite]");
								printVariable(word, fstart);
								print = false;
							}
							if (assign == true){
								assignVariable(varNameAsign, word, fstart, false);
								assign = false;
							}
							if (sum == true){
								sumVars(varNameAsign, word, fstart);
							}
						}
					}
					if (number == true){
						printf("[number(%i)]", digits);
						if (assign == true){
							assignVariable(varName, varValue, fstart, true);
							assign = false;
						}
						if (sum == true){
							sumVars(varName, word, fstart);
						}
						strcpy(varValue, "");
						number = false;
						digits = 0;
						varBits = 0;
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
				switch (c) {
					case '+':
						sum = true;
					break;
				}
			}
			if (lexLogOperator(c)){
				printf("%c", c);
				printf("[Logic operator]");
				switch (c) {
					case '=':
						assign = true;
					break;
				}
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
		strcmp(word, "i4") == 0 || strcmp(word, "i2") == 0 || strcmp(word, "fn") == 0 || strcmp(word, "put") == 0
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

void writeVariable(char* variable, int digCount, FILE* fdata, FILE* fstart){
	switch (digCount) {
		case 2 :
			fprintf(fdata, "\t%s dw '00', 0x0\n", variable);
		break;
		case 4 :
			fprintf(fdata, "\t%s dd '0000', 0x0\n", variable);
		break;
	}
}
void assignVariable(char* variable, char* value, FILE* fstart, bool t){
	if (t == false){
		fprintf(fstart, "\tmov ecx, [%s]\n", value);
		fprintf(fstart, "\tmov [%s], ecx\n\t\n", variable);
	}else{
		fprintf(fstart, "\tmov ecx, '%s'\n", value);
		fprintf(fstart, "\tmov [%s], ecx\n\t\n", variable);
	}
}

void sumVars(char* varA, char* varB, FILE* fstart){
	fprintf(fstart, "\tmov ecx, [%s]\n", varA);
	fprintf(fstart, "\tsub ecx, '0'\n");
	fprintf(fstart, "\tmov ebx, '%s'\n", varB);
	fprintf(fstart, "\tsub ebx, '0'\n");
	fprintf(fstart, "\tmov eax, ecx\n");
	fprintf(fstart, "\tadd eax, ebx\n");
	fprintf(fstart, "\tadd eax, '0'\n");
	fprintf(fstart, "\tmov [%s], eax\n\n", varA);
}

void printVariable(char* variable, FILE* fstart){
	fprintf(fstart, "\tmov edx, %d\n", 1);
	fprintf(fstart, "\tmov ecx, %s\n", variable);
	fprintf(fstart, "\tmov ebx, 1\n");
	fprintf(fstart, "\tmov eax, 4\n");
	fprintf(fstart, "\tint 0x80\n\t\n");
}
