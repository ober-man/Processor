#pragma once

#include<cstdio>
#include<cctype>
#include<cstdlib>
#include<cstring>
#include<cassert>
#include<iostream>
#include<cmath>
#include "enums.h"

#define MAXNUM 500
#define eps 1e-10
//-------------------------------------------------------------------
//! Function "MyStrtok" breaks line to tokens
//!
//!@param [in] string Line we are working with
//!@param [in] delim Array, contains separators
//!
//!@param [out] num Number of tokens in line
//!
//!@return Array with pointers to start of tokens
//!
//!@note If there is more tokens that defined,
//!      or there isn't any tokens,
//!      function returned NULL
//!
//-------------------------------------------------------------------
char** MyStrtok(char string[], const char* delim, size_t* num)
{
    assert(string != NULL);
    assert(delim != NULL);

    char** array = new char*[MAXNUM];
    size_t string_counter = 0; /// counter in "string"
    size_t array_counter = 0; /// counter in "array"
    size_t len = strlen(string);

    /// Ignoring separators in the beginning
    while(IsSymbol(string[string_counter], delim) && string[string_counter] != '\0')
        ++string_counter;

    /// If there are only separators in line
    if(string_counter == len)
        return NULL;

    /// The first token
    array[0] = &string[string_counter];
    ++string_counter;
    ++array_counter;

    while(string[string_counter] != '\0')
    {
        if(array_counter > MAXNUM)
            return NULL;
        if(IsSymbol(string[string_counter], delim) && string_counter + 1 <= len)
            string[string_counter] = '\0';
        else if(string[string_counter - 1] == '\0')
        {
            array[array_counter++] = &string[string_counter];
        }
        ++string_counter;
    }
    *num = array_counter;
    delete [] &array[array_counter];
    return array;
}

//------------------------------------------------------
//! Function "IsSymbol" checks existing symbol in line
//!
//!@param [in] symb Symbol to check
//!@param [in] delim Line to check
//!
//!@return true, if symbol exist in line
//!        false, if not
//!
//------------------------------------------------------
bool IsSymbol(char symb, const char* delim)
{
    assert(delim != NULL);

    size_t len = strlen(delim);
    for(size_t i = 0; i < len; ++i)
    {
        if(symb != delim[i])
            continue;
        else return true;
    }
    return false;
}

//-----------------------------------------------
//! Function "CountSymbols" count symbols in file
//!
//!@param [in] file File we are working with
//!
//!@return Number of symbols
//!
//-----------------------------------------------
size_t CountSymbols(FILE* in)
{
    /// Checking correctness of entry
    assert(in != NULL);

    /// Counting symbols
    fseek(in, 0, SEEK_END);
    size_t count = ftell(in);
    fseek(in, 0, SEEK_SET);
    //rewind(in);

    /// Checking emptiness
    if(count == 0)
    {
        printf("File is empty");
        exit(1);
    }
    return count;
}

//----------------------------------------------
//! Function "FileToArray" write file to array
//!
//!@param [in] file File we are working with
//!
//!@return Pointer to the start of the array
//!
//----------------------------------------------
char* FileToArray(const char* in_file)
{
    /// Checking correctness of entry
    FILE* in = fopen(in_file, "r");
    assert(in != NULL);
    size_t numsymb = CountSymbols(in);

    /// Creating array of symbols
    char* text = new char[numsymb];
    if(text == NULL)
    {
        printf("Memory error\n");
        exit(1);
    }
    fread(text, sizeof(char), numsymb, in);
    fclose(in);
    return text;
}

int Compare(double param, double num)
{
    if(param >= num + eps)
        return 1;
    if(param < num - eps)
        return -1;
    else
        return 0;
}

//------------------------------------------------------
//! Function "IsNumeral" checks if the word is a numeral
//!
//!@param [in] data Word to check
//!
//!@return true, if word is a numeral
//!        false, if not
//!
//------------------------------------------------------
bool IsNumeral(char* data)
{
    assert(data != NULL);
    bool first_dot = false;

    size_t len = strlen(data);
    for(size_t i = 0; i < len; ++i)
        if(!isdigit(data[i]))
        {
            /// Maybe it is a floating type numeral
            if(first_dot == false && i != len - 1 && data[i] == '.')
                first_dot = true;
            else
                return false;
        }
    return true;
}

//------------------------------------------------------------
//! Function "IsLabel" checks if the word is a label
//!
//!@param [in] data Word to check
//!
//!@return true, if word is a label
//!        false, if not
//!
//!@note Allowed only letters, numbers and underscore(_) symbol
//!      and label must END with a column(:)
//------------------------------------------------------------
bool IsLabel(char* data)
{
    assert(data != NULL);

    size_t len = strlen(data);
    if(data[len-1] != ':')
        return false;

    for(size_t i = 0; i < len - 1; ++i)
        if(!isalnum(data[i]) && data[i] != '_')
            return false;
    return true;
}

//--------------------------------------------------------------------------
//! Function "IsLabelArg" checks if the word is an argument to function "JMP"
//!
//!@param [in] data Word to check
//!
//!@return true, if word is a label-argument
//!        false, if not
//!
//!@note Allowed only letters, numbers and underscore symbol
//!      and label must BEGIN with a column
//--------------------------------------------------------------------------
bool IsLabelArg(char* data)
{
    assert(data != NULL);

    size_t len = strlen(data);
    if(data[0] != ':')
        return false;

    for(size_t i = 1; i < len; ++i)
        if(!isalnum(data[i]) && data[i] != '_')
            return false;
    return true;
}

char* DeleteColumn(char* data)
{
    assert(data != NULL);
    if(data[0] == ':')
        return (data + 1);

    size_t len = strlen(data);
    char* res = new char[len];
    for(size_t i = 0; i < len-1; ++i)
        res[i] = data[i];
    res[len-1] = '\0';
    return res;
}

void CompError(Error err, int num)
{
    switch(err)
    {
        case UNKNOWN:
            printf("Compilation error: word %d: unknown statement\n", num);
            exit(1);
        case WRONG_TOKEN:
            printf("Compilation error: line %d: wrong statement\n", num);
            exit(1);
        case NEED_ARG:
            printf("Compilation error: line %d: need argument after this command\n", num);
            exit(1);
        case NO_ARG:
            printf("Compilation error: line %d: can't be any arguments after this command\n", num);
            exit(1);
        case WRONG_END:
            printf("Compilation error: line %d: list can't end with this command\n", num);
            exit(1);
        case NO_BEGIN:
            printf("Compilation error: couldn't find BEGIN\n");
            exit(1);
        case MANY_BEGIN:
            printf("Compilation error: line %d: second BEGIN\n", num);
            exit(1);
    }
}
