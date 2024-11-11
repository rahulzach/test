#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char **productions;
int fvar;
int findPos(char NonTer) {
    int i = 0;
    while (productions[i][0] != NonTer) {
        i++;
    }
    return i;
}
char* findGenerating(char Ter) {
    int i = 0;
    while (productions[i][0] != Ter) {
        i++;
    }
    return productions[i];
}
void findFirst(char *prod) {
    int i;
    for (i = 3; i < strlen(prod); i++) {
        if ((prod[i] >= 'a' && prod[i] <= 'z') || prod[i] == ')' || prod[i] == '(' || prod[i] == ',') {
            printf(" %c ", prod[i]);

            while (prod[i] != '/' && prod[i] != '\0') {
                i++;
            }
        } else if (prod[i] >= 'A' && prod[i] <= 'Z') {
            findFirst(findGenerating(prod[i]));
            return;
        } else if (prod[i] == '#') {
            printf("  #");
        } else {
            continue;
        }
    }
}
void findFollow(char GeneratingSymbol, int n) {
    int i, j;
    if (GeneratingSymbol == 'S')
        printf(" $ ");
    for (j = 0; j < n; j++) {
        for (i = 3; i < strlen(productions[j]); i++) {
            if (GeneratingSymbol == productions[j][i]) {
                if ((productions[j][i + 1] >= 'a' && productions[j][i + 1] <= 'z') || productions[j][i + 1] == ')' || productions[j][i + 1] == '(' || productions[j][i + 1] == ',') {
                    printf(" %c ", productions[j][i + 1]);
                } else if (productions[j][i + 1] >= 'A' && productions[j][i + 1] <= 'Z') {
                    findFirst(findGenerating(productions[j][i + 1]));
                } else if (i + 1 == strlen(productions[j])) {
                    findFollow(productions[j][0], n);
                } else {
                    continue;
                }
            }
        }
    }
}

int main() {
    int i, n;
    printf("Enter the number of productions: ");
    scanf("%d", &n);
    getchar(); // consume newline after the number input
    productions = (char*)malloc(sizeof(char) * n);
    for (i = 0; i < n; i++)
        productions[i] = (char*)malloc(sizeof(char) * 20);
      for (i = 0; i < n; i++) {
        printf("Enter production %d: ", i + 1);
        fgets(productions[i], 20, stdin);
        productions[i][strcspn(productions[i], "\n")] = 0; // Remove newline character
    }
    for (i = 0; i < n; i++) {
        printf("\nFIRST(%c)={  ", productions[i][0]);
        findFirst(productions[i]);
        printf("}\n");
    }
     for (fvar = 0; fvar < n; fvar++) {
        printf("\nFOLLOW(%c)={", productions[fvar][0]);
        findFollow(productions[fvar][0], n);
        printf("}\n");
    }

    printf("\nThe End\n");

    // Free allocated memory
    for (i = 0; i < n; i++) {
        free(productions[i]);
    }
    free(productions);
    return 0;
}First and follow
