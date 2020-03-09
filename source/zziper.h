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
string create_new_path(string, string);

typedef struct HeaderRecord
{
    uint number_of_bytes;
    string name;
    string address;

    void (*destructor) (struct HeaderRecord*);
    void (*compute_file_size) (struct HeaderRecord*, string);
    void (*add_to_output) (FILE*);
} HeaderRecord;

HeaderRecord* HeaderRecord_creation (string, string, void*, void*, void*);
void destroy_record(HeaderRecord*);
void add_to_record(HeaderRecord* self, FILE *outfile);
void compute_file_size(HeaderRecord*, string);

typedef struct Zziper
{
    int number_of_files;
    string *files;
    string path;
    string archive_name;
    FILE* output_dump;

    void (*destructor) (struct Zziper*);
    void (*searcher) (struct Zziper*, string, FILE *outfile);
    void (*file_handler) (HeaderRecord* in);
    void (*repr) ();

} Zziper;


// Zziper methods
Zziper* Zziper__creation (void*, void*);
void Zziper_destroy(Zziper*);
void list_directory(Zziper* self, string dir_name, FILE *outfile);
void compress (Zziper* self);
void decompress (Zziper* self);
void repr(Zziper* self);


#endif //SIMPLEARCHIVER_ZZIPER_H
