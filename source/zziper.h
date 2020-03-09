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
    FILE* header;
    uint number_of_bytes;
    string file_name;
    string file_address;

    void (*destructor) (struct HeaderRecord*);
    void (*compute_file_size) (struct HeaderRecord*, string);
    void (*init_next_file) (struct HeaderRecord*, string, string);
    void (*add_to_output) (struct HeaderRecord*, FILE*);
} HeaderRecord;

HeaderRecord* HeaderRecord_creation (void*, void*, void*, void*);
void HeaderRecord_destruction(HeaderRecord*);
void init_next_file (HeaderRecord*, string, string);
void add_to_record(HeaderRecord* self, FILE *outfile);
void compute_file_size(HeaderRecord*, string);

typedef struct Zziper
{
    uint number_of_files;
    uint bytes;
    string *files;
    string path;
    string archive_name;
    FILE* output_dump;
    HeaderRecord* h_record;

    void (*destructor) (struct Zziper*);
    void (*searcher) (struct Zziper*, string, FILE *outfile);
    void (*file_handler) (HeaderRecord* in);
    void (*repr) ();

} Zziper;


// Zziper methods
Zziper* Zziper__creation (HeaderRecord*, void*, void*);
void Zziper_destruction(Zziper*);
void list_directory(Zziper* self, string dir_name, FILE *outfile);
void compress (Zziper* self);
void decompress (Zziper* self);
void repr(Zziper* self);


#endif //SIMPLEARCHIVER_ZZIPER_H
