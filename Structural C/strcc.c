#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main(int argc, char * argv[]){
    /***opening up sourcefile***/
    int ch,i;
    int sfcharcount = -1;
    FILE * sourcefile = fopen(argv[1],"r");
    if (!sourcefile){
        printf("File doesn't exist or cannot read it\n");
        return 1;
    }
    ch = 48;
    /***figuring out size***/
    while (ch!=EOF) {
        ch = fgetc(sourcefile);
        sfcharcount++;
    }
    /***allocate memory dynamically on the heap for the buffer***/
    char * buffer = malloc(sfcharcount*sizeof(char));
    fclose(sourcefile);
    sourcefile = fopen(argv[1],"r");
    i = 0;
    /***reading file into the buffer***/
    do {
        ch = fgetc(sourcefile);
        buffer[i] = (char) ch;
        i++;

    } while (ch != EOF);
    buffer[i-1] = 0;
    /***printing out just for testing***/
    //printf("%s\n",buffer);
    //printf("%d\n",buffer[i-1]);
    fclose(sourcefile);
    /***creating temporary file for gcc***/
    FILE * temporaryc = fopen("temporary.c","wb");
    unsigned int outputsize = i-1; //***declaring size of output***
    /***check if able to open it up***/
    if(!temporaryc) {
        printf("Unable to open\n");
        return 1;
    }

    /***
    
    Modifications of buffer start (pay attention on outputsize!!)
    
    ***/


    int cica = searchargumentbycount_index (buffer,"increasewith",1,0);
    int adlak = searchargumentbytype_index (buffer, "increasewith","int*");
    //printf("%d\n",cica);
    //printf("%d\n",adlak);
    int cicabackup = cica;
    int adlakbackup = adlak;
    while (cica <= (cicabackup + strlen("int*"))){
        printf("%c",buffer[cica]);
        cica++;
    }
    printf("\n");
    while (adlak <= (adlakbackup + strlen("int*"))) {
        printf("%c",buffer[adlak]);
        adlak++;
    }
    printf("\n");
    
    
    /***
    
    Modifications of buffer end
    
    ***/
    
    /***write buffer to temporary.c***/
    fwrite(buffer,sizeof(char),outputsize,temporaryc);
    /***closing temporary.c***/
    fclose(temporaryc);
    /***freeing up buffer***/
    free(buffer);
    /***running gcc***/
    system("sh compile.sh");
    /***remove temporary.c***/
    int removefail = remove("temporary.c");
    if (removefail) {
        printf("Unable to remove temporary file\n");
        return 1;
    }
    //printf("%s\n",argv[1]);
    return 0;
}