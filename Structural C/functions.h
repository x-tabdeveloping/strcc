#ifndef _FUNC_H_
#define _FUNC_H_
int searchinstring (const char * search,const char * base, int searchfrom);
int getlinescount (const char * text);
int getlineindex (const char * text, int line);
int countlinelength (const char * text, int line);
char * gettextline (const char * text,int line);
void writeline (const char * text,int line);
char * findouttype (const char * text, const char * object);
void writetype (const char * text,const char * object);
int removewhitespace (const char ** text);
int removespace (const char ** text);
int replaceintext(char **text, const char * replacewith,  int beginindex, int endindex);
int replaceline (char ** text,int line,const char * replacewith);
int searchargumentbytype_index (const char * text, const char * function,const char * type);
int searchfromto (const char * text,const char * object, int searchfrom, int searchto);
int searchargumentbytype_count (const char * text, const char * function,const char * type);
int searchargumentbycount_index (const char * text,const char * function, int argument, int searchfrom);
#endif