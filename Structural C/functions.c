#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int searchinstring (const char * search,const char * base, int searchfrom) {
    int count = 0;
    int countbase = searchfrom;
    //printf("%d\n",strlen(base));
    while (countbase <= (strlen(base))) {
        if (count == strlen(search)){
            return countbase - (strlen(search));
        }
        if (search [count] == base[countbase]) {
            count++;
        }
        else {
            count = 0;
        }
        countbase++;
    }
    return -1;
}

int searchfromto (const char * text,const char * object, int searchfrom, int searchto) {
    int count = 0;
    int countbase = searchfrom;
    //printf("%d\n",strlen(base));
    while (countbase <= searchto) {
        if (count == strlen(object)){
            return countbase - (strlen(object));
        }
        if (object [count] == text[countbase]) {
            count++;
        }
        else {
            count = 0;
        }
        countbase++;
    }
    return -1;
}

int getlinescount (const char * text) {
    int count = 0;
    int i = 0;
    while (i<=strlen(text)){
        if (text[i]==';') {count++;}
        i++;
    }
    return count;
}

int getlineindex (const char * text, int line){
    int i = 0;
    int count = 0;
    line--;
    while(i<=strlen(text)) {
        if ((text[i] == ';') || (text[i] == '{')) {count++;}
        if ((count-1) == line) {return (i);}
        i++;
    }
}

int countlinelength (const char * text, int line) {
    int index = getlineindex(text,line);
    int backup = index;
    while (((text[index]!=';') && (text[index]!='{') ) && (index<=strlen(text))) {
        index++;
    }
    //printf("Stillnotcoredumped\n");
    index++;
    return (index-backup);
}
char * gettextline (const char * text,int line) {
    int index = getlineindex(text, line);
    int size = (countlinelength(text,line)+1);
    char * val = (char *) malloc(size*sizeof(char));
    strncpy(val, text+index, countlinelength(text,line));
    val[(countlinelength(text,line)+1)] = 0;
    return val;
}
void writeline (const char * text,int line) {
    char * towrite = gettextline(text,line);
    printf("%s\n",towrite);
    free(towrite);
}

int replaceintext(char **text, const char * replacewith,  int beginindex, int endindex) {
    char * firstportion = malloc((beginindex+1)*sizeof(char));
    char *secondportion = malloc(((strlen(*text)-(beginindex + strlen(replacewith)))+1)*sizeof(char));
    int i;
    strncpy(firstportion,*text,beginindex);
    strcpy(secondportion, (*text)+endindex);
    char * value = malloc((strlen(firstportion)+strlen(secondportion)+strlen(replacewith)+1)+sizeof(char));
    if (value == NULL) {return -1;}
    value[0] = 0;
    printf("%s\n",firstportion);
    printf("%s\n",replacewith);
    printf("%s\n",secondportion);
    strcpy(value,firstportion);
    strcat(value,replacewith);
    strcat(value,secondportion);
    value[strlen(firstportion)+strlen(secondportion)+strlen(replacewith)+1] = 0;
    free(*text);
    *text = value;
    return 0;
}

int replaceline (char ** text,int line,const char * replacewith){
    int beginindex = getlineindex(*text,line);
    int endindex = getlineindex(*text,line+1);
    printf("%d\n",beginindex);
    printf("%d\n",endindex);
    int check = replaceintext(text,replacewith,beginindex,endindex);
    if (check == -1) return check;
    check = removewhitespace(text);
    return check;
}

/*int adaptfunctioncall (char ** text, int line, const char * object){

}
*/
/*int checkfunction (const char * text, const char * object) {

    int index = searchinstring(object,text,0);
    if (index == -1) return index;

}*/

int searchargumentbytype_index (const char * text, const char * function,const char * type) {
    int funcindex = searchfromto(text,function,0,strlen(text));
    int searchfrom = funcindex;
    while ((text[searchfrom] != '(') && (searchfrom<=strlen(text))) searchfrom++;
    int searchto = searchfrom;
    while ((text[searchto] != ')') && (searchto<=strlen(text))) searchto++;
    if ((searchfrom == strlen(text)) || (searchto == strlen(text))) return -1;
    int typeindex = searchfromto(text,type,searchfrom,searchto);
    return typeindex;
}

int searchargumentbytype_count (const char * text, const char * function,const char * type){
    int typeindex = searchargumentbytype_index(text,function,type);
    if (typeindex == -1) return typeindex;
    int index = typeindex;
    int count = 1;
    while ((text[index] != '(') && (index >= 0)) {
        if (text[index] == ',') count++;
        index--;
    }
    if (index == 0) return -1;
    return count;
}

int searchargumentbycount_index (const char * text,const char * function, int argument,int searchfrom){
    int funcindex = searchfromto(text,function,searchfrom,strlen(text));
    searchfrom = funcindex;
    while ((text[searchfrom] != '(') && (searchfrom<=strlen(text))) searchfrom++;
    int searchto = searchfrom;
    while ((text[searchto] != ')') && (searchto<=strlen(text))) searchto++;
    if ((searchfrom == strlen(text)) || (searchto == strlen(text))) return -1;
    int count = 1;
    while ((count <= argument) && (searchfrom <= searchto)) {
        if (text [searchfrom] == ',') count++;
        searchfrom++;
    }
    if (searchfrom == searchto) return -1;
    return searchfrom;
    
}

int replaceargumentbycount (char ** text, const char * function, int argument) {

}

int removewhitespace (char ** text) { //remove whitespace function
    int length = 0;
    for (int i = 0; i<=strlen(*text);i++) {
        if (/*(*text[i] != ' ') &&*/ ((*text)[i] != 9) && ((*text)[i] != 10)) length++;
    }    
    char * value = malloc(length*(sizeof(char)+1));
    if (value == NULL) {
        return -1;
    }
    int count = 0;
    for (int i = 0; i<=strlen(*text);i++) {
        if (/*(*text[i] != ' ') && */((*text)[i] != 9) && ((*text)[i] != 10)) {
            value[count] = (*text)[i];
            count++;
        }
    }
    value[count] = 0;
    free(*text);
    *text = value;
    return 0;
}

int removespace (char ** text) {
    int length = 0;
    for (int i = 0; i<=strlen(*text);i++) {
        if ((*text)[i] != ' ') length++;
    }
    char * value = malloc(length*(sizeof(char)+1));
    int count = 0;
    for (int i = 0; i<=strlen(*text);i++) {
        if ((*text)[i] != ' ') {
            value[count] = (*text)[i];
            count++;
        }
    }
    value[count] = 0;
    free(*text);
    *text = value;
    return 0;
}

char * findouttype (const char * text, const char * object) {
    int index = searchinstring(object,text,0);
    int i = index;
    /*while ((text[i] == ' ') || (text[i] == 9) || (text[i] == 10)) {
        i--;
    }*/
    //if (index == i) return NULL;
    //int newindex = i;
    while ((text[i] != ' ') && (text[i] != 9) && (text[i] != 10)) {
        i--;
    }
    int newindex = i;
    while ((text[i] == ' ') || (text[i] == 9) || (text[i] == 10)) {
        i--;
    }
    while ((text[i] != ' ') && (text[i] != 9) && (text[i] != 10)) {
        i--;
    }
    char * type = malloc((newindex - i + 1)*sizeof(char));
    int count = 0;
    while (i<=newindex) {
        type[count] = text [i];
        i++; count++;
    }
    type[count] = 0;
    int check = removewhitespace(&type);
    if (check == -1) return NULL;
    check = removespace(&type);
    if (check == -1) return NULL;
    return type;
}

int writetype (const char * text,const char * object) {
    char * towrite = findouttype(text,object);
    if (towrite == NULL) {
        printf("Failed to write type\n");
        return -1;
        }
    printf("%s\n",towrite);
    free(towrite);
}

