#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int id = 0, key = 0, oper = 0, del = 0, consts = 0, invalid = 0, literal = 0, header = 0;

int isKeyword(char buffer[]) {
    const char *keywords[] = {
        "auto","break","case","char","const","continue","default",
        "do","double","else","enum","extern","float","for","goto",
        "if","int","long","register","return","short","signed",
        "sizeof","static","struct","switch","typedef","union",
        "unsigned","void","volatile","while","printf"
    };
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < numKeywords; ++i) {
        if (strcmp(keywords[i], buffer) == 0) {
            return 1;
        }
    }
    return 0;
}

int isInvalid(char buffer[]) {
    int hasDigit = 0, hasAlpha = 0;
    for (int i = 0; i < strlen(buffer); ++i) {
        if (isdigit(buffer[i])) hasDigit = 1;
        if (isalpha(buffer[i])) hasAlpha = 1;
    }
    if (hasDigit && hasAlpha) {
        return 1;
    }
    if (buffer[0] == '#' && strlen(buffer) == 2) {
        return 1;
    }
    return 0;
}

void printToken(char buffer[]) {
    if (isKeyword(buffer)) {
        printf("%s keyword ", buffer);
        key++;
    } else if (isdigit(buffer[0])) {
        if (isInvalid(buffer)) {
            printf("%s invalid ", buffer);
            invalid++;
        } else {
            printf("%s const ", buffer);
            consts++;
        }
    } else if (buffer[0] == '#') {
        if (isInvalid(buffer)) {
            printf("%s invalid ", buffer);
            invalid++;
        } else {
            printf("%s header ", buffer);
            header++;
        }
    } else if (buffer[0] == '"') {
        printf("%s literal ", buffer);
        literal++;
    } else {
        if (isInvalid(buffer)) {
            printf("%s invalid ", buffer);
            invalid++;
        } else {
            printf("%s id ", buffer);
            id++;
        }
    }
}

void processBuffer(char buffer[], int *j) {
    if (*j != 0) {
        buffer[*j] = '\0';
        printToken(buffer);
        *j = 0;
    }
}

void printDelimiter(char ch) {
    switch (ch) {
        case ';': printf(";-delim "); break;
        case ',': printf(",-delim "); break;
        case '(': printf("(-delim "); break;
        case ')': printf(")-delim "); break;
        case '[': printf("[-delim "); break;
        case ']': printf("]-delim "); break;
        case '{': printf("{-delim "); break;
        case '}': printf("}-delim "); break;
        case '<': printf("<-delim "); break;
        case '>': printf(">-delim "); break;
    }
    del++;
}

void printOperator(char ch) {
    switch (ch) {
        case '+': printf("plus-op "); break;
        case '-': printf("minus-op "); break;
        case '*': printf("mul-op "); break;
        case '/': printf("div-op "); break;
        case '%': printf("mod-op "); break;
        case '=': printf("eq-op "); break;
    }
    oper++;
}

int main() {
    char ch, buffer[100], operators[] = "+-*/%=";
    FILE *fp1;
    int i, j = 0;

    fp1 = fopen("input.txt", "r");

    if (fp1 == NULL) {
        printf("Error while opening the file\n");
        exit(0);
    }

    while ((ch = fgetc(fp1)) != EOF) {
        int isOperator = 0;

        for (i = 0; i < sizeof(operators) - 1; ++i) {
            if (ch == operators[i]) {
                processBuffer(buffer, &j);
                printOperator(ch);
                isOperator = 1;
                break;
            }
        }

        if (!isOperator) {
            if (isalnum(ch) || ch == '#' || ch == '.' || ch == '"' || ch == '<' || ch == '>') {
                buffer[j++] = ch;
            } else {
                processBuffer(buffer, &j);
                if (ch == ' ' || ch == '\n') {
                    if (ch == '\n') printf("\n");
                    continue;
                }
                printDelimiter(ch);
            }
        }

        if (ch == '\n') {
            printf("\n");
        }
    }
    // Process any remaining buffer content
    processBuffer(buffer, &j);

    printf("\n\nKeywords: %d\nIdentifiers: %d\nOperators: %d\nDelimiters: %d\nConstants: %d\nInvalid: %d\nLiterals: %d\nHeaders: %d\n", key, id, oper, del, consts, invalid, literal, header);

    fclose(fp1);
    return 0;
}
