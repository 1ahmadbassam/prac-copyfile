#ifndef STRING_H
#define STRING_H

extern int strsize(const char* str);
extern int strcopy(const char* src, char* dest, int start, int end);
#define TRIM_USES_MALLOC
extern const char* trim(const char* str);
extern int compstr(const char* str1, const char* str2);
extern int nullstr(const char* str, int size);
#define TOLOWERSTR_USES_MALLOC
extern const char* tolowerstr(const char* str);

#endif
