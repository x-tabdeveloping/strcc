#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main() {
    char * text = malloc(strlen(("babcicavagyok;szeretematejet;kecske;")+1)*sizeof(char));
    strcpy(text,"babcicavagyok;szeretematejet;kecske;");
    int check = replaceline(&text,2,";adlak");
    if (check == -1) {
        printf("Failed to replace\n");
        return 1;
    }
    printf("%s\n",text); 
    free(text);
    
    return 0;
}