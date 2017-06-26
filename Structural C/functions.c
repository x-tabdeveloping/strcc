#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

void faultcheck (void) {
    printf("Still no fault\n");
}

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

/*int getlineindex (const char * text, int line){
    int i = 0;
    int count = 0;
    line--;
    while(i<=strlen(text)) {
        if ((text[i] == ';') || (text[i] == '{')) {count++;}
        if ((count-1) == line) {return (i);}
        i++;
    }
}*/

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
    /*printf("firstportion : %s\n",firstportion);
    printf("replacewith : %s\n",replacewith);
    printf("secondportion : %s\n",secondportion);*/
    strcpy(value,firstportion);
    strcat(value,replacewith);
    strcat(value,secondportion);
    value[strlen(firstportion)+strlen(secondportion)+strlen(replacewith)+1] = 0;
    free(*text);
    *text = value;
    return 0;
}
int placeintext(char ** text,const char * object, int index) {
    char * firstportion = malloc((index+1) * sizeof(char));
    int secondlength = strlen(*text) - index + 1;
    char * secondportion = malloc(secondlength * sizeof(char));
    strncpy(firstportion,(*text),index);
    firstportion[index+1] = 0;
    strncpy(secondportion,(*text)+index+1,secondlength-1);
    secondportion[secondlength] = 0;
    int length = strlen(secondportion)+strlen(firstportion)+strlen(object)+1;
    char * value = malloc(length * sizeof(char));
    strcpy(value,firstportion);
    strcat(value,object);
    strcat(value,secondportion);
    value[length-1] = 0;
    free(*text);
    *text = value;
    return 0;
}

int placeintextonlywhenfirst (char ** text,const char * object, int index){
    char * firstportion = malloc((index+1) * sizeof(char));
    int secondlength = strlen(*text) - index + 1;
    char * secondportion = malloc(secondlength * sizeof(char));
    strncpy(firstportion,(*text),index);
    firstportion[index+1] = 0;
    strncpy(secondportion,(*text)+index,secondlength-1);
    secondportion[secondlength] = 0;
    int length = strlen(secondportion)+strlen(firstportion)+strlen(object)+1;
    char * value = malloc(length * sizeof(char));
    strcpy(value,firstportion);
    strcat(value,object);
    strcat(value,secondportion);
    value[length-1] = 0;
    free(*text);
    *text = value;
    return 0;
}

int replaceline (char ** text,int index,const char * replacewith){
    int count = index;
    while ((count <= strlen(*text)) && ((*text)[count] != ';')) count++;
    /*printf("%d\n",beginindex);
    printf("%d\n",endindex);*/
    int check = replaceintext(text,replacewith,index,count);
    if (check == -1) return check;
    //check = removewhitespace(text);
    return check;
}

/*int adaptfunctioncall (char ** text, int line, const char * object){

}
*/
/*int checkfunction (const char * text, const char * object) {

    int index = searchinstring(object,text,0);
    if (index == -1) return index;

}*/

// Returns dynamically allocated string don't forget to free!!

char * getarguments (const char * text, const char * function) {
    int funcindex = searchfromto(text,function,0,strlen(text));
    int indexfrom = funcindex;
    while ((text[indexfrom] != '(') && (indexfrom<=strlen(text))) indexfrom++;
    indexfrom++;
    int indexto = indexfrom;
    while ((text[indexto] != ')') && (indexto<=strlen(text))) indexto++;
    indexto--;
    int length = (indexto-indexfrom)*sizeof(char)+1;
    char * value = malloc(length);
    strncpy(value,text+indexfrom,length);
    value[length] = 0;
    removewhitespace(&value);
    //removespace(&value);
    return value;
}

int getarguments_amount (const char * text, const char * function) {
    char * temporary = getarguments(text,function);
    /*printf("func : %s\n",function);
    printf("temporary : %s\n",temporary);*/
    int count = 1;
    for (int i = 0; i <= strlen(temporary);i++) {
        if (temporary[i] == ',') count++;
    }
    free(temporary);
    return count;
}

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
    searchfrom++;
    int searchto = searchfrom;
    while ((text[searchto] != ')') && (searchto<=strlen(text))) searchto++;
    searchto--;
    if ((searchfrom == strlen(text)) || (searchto == strlen(text))) return -1;
    int count = 1;
    if (count == argument) return searchfrom;
    while ((count < argument) && (searchfrom <= searchto)) {
        /*if (text [searchfrom] == ',') count++;*/
        while (text[searchfrom] != ',') searchfrom++;
        count++;
    }
    if (searchfrom == searchto) return -1;
    searchfrom++;
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
        printf("malloc failed\n");
        exit(-1);
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

int removewhitespacefrom(char ** text, int removefrom) {
    int length = removefrom;
    for (int i = removefrom; i<=strlen(*text);i++) {
        if (/*(*text[i] != ' ') &&*/ ((*text)[i] != 9) && ((*text)[i] != 10)) length++;
    }
    char * value = malloc(length*(sizeof(char)+1));
    if (value == NULL) {
        printf("malloc failed\n");
        exit(-1);
    }
    int count /*= 0*/;
    for (count = 0; count<=removefrom; count++) {
        value[count] = (*text)[count];
    }
    for (int i = removefrom; i<=strlen(*text);i++) {
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


/*int writetype (const char * text,const char * object) {
    char * towrite = findouttype(text,object);
    if (towrite == NULL) {
        printf("Failed to write type\n");
        return -1;
        }
    printf("%s\n",towrite);
    free(towrite);
}*/

char * addstring (const char * text, const char * addition) {
    int length = (strlen(text) + strlen(addition) + 1);
    char * value = malloc(length*sizeof(char));
    if (value == NULL) {
        return value;
    }
    strcpy(value,text);
    strcat(value,addition);
    value[length] = 0;
    return value;
}
int findrelativefunctioncall (const char * text, const char * object, int searchfrom) {
    char * workwith = addstring(object,".");
     int index = searchfromto(text,workwith,searchfrom,strlen(text));
    free(workwith);
    return index;
}

int findrelativefunctioncall_by_function (const char * text, const char * object, const char * function, int searchfrom) {
    char * temporary = addstring(object,".");
    char * workwith = addstring(temporary,function);
    free(temporary);
     int index = searchfromto(text,workwith,searchfrom,strlen(text));
    free(workwith);
    return index;
}

char * getlinebyindex(const char * text, int index) {
    int count = index;
    while ((count <= strlen(text)) && (text[count] != ';')) count++;
    /*for (count = index; count < strlen(text); count++) {
        if (text[count] == ';') break;
    }*/
    int length = count - index + 2;
    //printf("%d\n",count);
    char * value = malloc(length*sizeof(char));
    if (!value) {
        printf("malloc failed\n");
        exit(-1);
    }
    strncpy(value,text+index,length-1);
    value[length-1] = 0;
    return value;
}

int findendingofdefinition (const char * text){
    int index = 0;
    while ((text[index] == 9) || (text[index] == 10)) index++;
    if (text[index] != '#') {
        printf("Error : No definition part found!!\n");
        exit(-1);
    }
    while (text[index] == '#') {
        index++;
        while (text[index] != 10) {
            index++;
        }
        index++;
    }
    return index;
}

/*int can_passargument_relative_count (const char * text,const char * object, const char * function,int searchfrom) {
    char * arguments = getarguments(text,function);
    char * type = findouttype(text,object);
    char * pointertype = addstring(type,"*");
    int index = searchfromto(arguments,pointertype,0,strlen(arguments));
    int i = index;
    int count = 1;
    while (i >= 0) {
        if (arguments[i] == ',') count++;
        i--;
    }
    free(arguments);
    free(type);
    free(pointertype);
    return count;
}*/

int getlineindex (const char * text, int index){
    int i = index;
    while (text[i] != ';' && text[i] != '{') {
        i--;
    }
    i++;
    return i;
}

char * findouttype (const char * text, const char * object) {
    int searchfrom = searchfromto(text,"main(",0,strlen(text));
    int index = searchfromto(text,object,0,strlen(text));
    index = getlineindex(text,index);
    char * line = getlinebyindex(text,index);
    removewhitespace(&line);
    //printf("%s\n",line);
    int length = 0;
    while (line[length] != ' ') {
        //printf("%c\n",line[length]);
        length++;
    }
    length++;
    char * type = malloc(length*sizeof(char));
    strncpy(type,line,length);
    type[length] = 0;
    removespace(&type);
    removewhitespace(&type);
    //printf("%s\n",type);
    free(line);
    return type;
}

void removefromstring (char ** text,int removefrom,int removeto) {
    int length = strlen(*text)-(removeto-removefrom)+1;
    char * value = malloc(length*sizeof(char));
    if (value == NULL) {
        printf("malloc failed\n");
        exit(-1);
    }
    int count = 0;
    for (int i = 0; i < removefrom; i++) {
        value[count] = (*text)[i];
        count++;
    }
    for (int i = removeto; i <= strlen(*text); i++) {
        value[count] = (*text)[i];
        count++;
    }
    *text = value;
}

void passargumentinline(char ** text,const char * object,int argumentcount){
    int startindex = 0;
    while ((*text)[startindex] != '(') {
        startindex++;
    }
    startindex++;
    char * function = malloc((startindex-1)*sizeof(char));
    strncpy(function,*text,startindex-2);
    function[startindex-1] = 0;
    int endindex = startindex;
    while ((*text)[endindex] != ')') endindex++;
    endindex--;
    int argumentamount = getarguments_amount(*text,function);
    free(function);
    char * useobject;
    if (argumentamount+1 != argumentcount) {
        useobject = addstring(object,",");
    }
    else {
        useobject = addstring(",",object);
    }
    int wheretoput = startindex+1;
    int count = 1;
    while (wheretoput <= endindex && count < argumentcount) {
        if ((*text)[wheretoput] == ',') count++;
        wheretoput++;
    }
    wheretoput--;
    if (argumentcount == 1) placeintextonlywhenfirst(text,useobject,wheretoput); else
    placeintext (text,useobject,wheretoput);
    removewhitespace(text);
    free(useobject);
}

void passobjectasargumentinline_relative(char ** text,int argumentcount) {
    removewhitespace(text);
    int count = 0;
    while ((*text)[count] != '.') {
        count++;
    }
    char * tempobject = malloc((count+1)*sizeof(char));
    strncpy(tempobject,(*text),count);
    tempobject[count+1] = 0;
    char * object = addstring("&",tempobject);
    free(tempobject);
    passargumentinline(text,object,argumentcount);
    //printf("%s\n",*text);
    removefromstring (text,0,count+1);
    free(object);
}

void create_functioncall_relative (char ** text,int lineindex){
    char * line = getlinebyindex(*text,lineindex);
    int i = 0;
    while(line[i] != '.') i++;
    char * object = malloc((i+1)*sizeof(char));
    strncpy(object,line,i);
    object[i+1] = 0;
    printf("%s\n",object);
    int ibackup = i+1;
    while(line[i] != '(') i++;
    char * function = malloc((i-ibackup+1)*sizeof(char));
    strncpy(function,line+ibackup,i-ibackup);
    function[i-ibackup+1] = 0;
    printf("%s\n",function);
    char * temptype = findouttype(*text,object);
    char * type = addstring(temptype,"*");
    printf("%s\n",type);
    free(temptype);
    char * arguments = getarguments(*text,function);
    printf("%s\n",arguments);
    int index = searchfromto(arguments,type,0,strlen(arguments));
    if (index == -1) {
        printf(ANSI_COLOR_RED "Error : " ANSI_COLOR_RESET "not matching type" ANSI_COLOR_BLUE " -- relative function call in line :" ANSI_COLOR_RESET" \"%s\"\n",line);
        exit(-1);
    }
    int argumentcount = 1;
    for (i = index; i>=0; i--) {
        if (arguments[i] == ',') {
            argumentcount++;
        }
    }
    passobjectasargumentinline_relative(&line,argumentcount);
    replaceline(text,lineindex,line);
    free(line);
    free(object);
    free(function);
    free(type);
    free(arguments);
}
