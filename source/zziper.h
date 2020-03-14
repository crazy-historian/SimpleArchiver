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
string create_new_path(string, string);

typedef struct HeaderRecord
{
    FILE* archive_header;
    size_t header_size_in_bytes;
    size_t file_size_in_bytes;
    string file_name;
    string file_address;

    void (*destructor) (struct HeaderRecord*);
    size_t (*compute_file_size) (string);
    void (*init_next_file) (struct HeaderRecord*, string, string);
    void (*add_to_header) (struct HeaderRecord*);
} HeaderRecord;

HeaderRecord* HeaderRecord_creation (void*, void*, void*, void*);
void HeaderRecord_destruction(HeaderRecord*);
void init_next_file (HeaderRecord*, string, string);
void add_to_header(HeaderRecord* self);
size_t compute_file_size(string);

typedef struct Zziper
{
    uint number_of_files;
    uint size_in_bytes;
    HeaderRecord* h_record;
    FILE* output_dump;
    FILE* archive_file;


    void (*destructor) (struct Zziper*);
    void (*searcher) (struct Zziper*, string);
    void (*add_to_dump) (struct Zziper*, string, string);
    void (*create_archive) (struct Zziper*);
    void (*read_dump) (struct Zziper*, string);
    void (*repr) ();

} Zziper;


// Zziper methods
Zziper* Zziper__creation (void*, void*, void*, void*, void*);
void Zziper_destruction(Zziper*);
void list_directory(Zziper* self, string dir_name);
void create_archive (Zziper* self);
void add_to_dump(Zziper*, string, string);
void read_dump (Zziper*, string);

#endif //SIMPLEARCHIVER_ZZIPER_H