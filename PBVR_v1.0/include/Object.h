/*
LICENCE
*/
//Object.h
//definition and implementation of basic class Object
///throw exception and Log

#ifndef OBJECT_H
#define OBJECT_H

#include <cstdio>

class Object
{
public:
    void throwScn(char str[])const {printf("%s",str);}
    void throwLog(FILE* fid, char* str)const {fprintf(fid, "%s",str);}
};

#endif // OBJECT_H
