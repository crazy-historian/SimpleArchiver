//
// Created by Professional on 08.03.2020.
//
#ifndef SIMPLEARCHIVER_ZZIPER_H
#define SIMPLEARCHIVER_ZZIPER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

typedef unsigned long int uint;

typedef char* string;
string get_string(const char *in);

typedef struct HeaderRecord
{
    uint number_of_bytes;
    string name;
    string address;

    void (*destructor) (struct HeaderRecord*);
    void (*add_to_output) (FILE*);
} HeaderRecord;

void destroy_record(HeaderRecord*);
void add_to_record(HeaderRecord* self, FILE *outfile);
HeaderRecord* HeaderRecord_creation (uint, string, string, void*, void*);

typedef struct Zziper
{
    int number_of_files;
    string *files;
    string path;
    string file_name;
    FILE* output_dump;

    void (*searcher) (struct Zziper*, string);
    void (*file_handler) (HeaderRecord* in);
    void (*repr) ();

} Zziper;


// Zziper methods
void Zziper__init (Zziper* self);
void list_directory(Zziper* self, string dir_name, FILE *outfile);
void compress (Zziper* self);
void decompress (Zziper* self);
void repr(Zziper* self);


#endif //SIMPLEARCHIVER_ZZIPER_H
