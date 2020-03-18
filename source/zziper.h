//
// Created by Professional on 08.03.2020.
//
#ifndef SIMPLEARCHIVER_ZZIPER_H
#define SIMPLEARCHIVER_ZZIPER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

typedef char* string;
string create_new_path(string, string);
string concatenate(string, string, string);
string clip_substring(string, string);
string* tokenization(string, string);
string concatenate_path(string*, string);

typedef struct string_list
{
    size_t size;
    string* list;

} string_list;

string_list* tokenization_(string);
void free_string_list(string_list*);

typedef struct HeaderRecord
{
    FILE* archive_header;
    size_t header_size_in_bytes;
    size_t file_size_in_bytes;
    string file_name;
    string file_address;

    void (*destructor) (struct HeaderRecord*);
    size_t (*compute_file_size) (string);
    void (*init_next_file) (struct HeaderRecord*, string, string, string);
    void (*add_to_header) (struct HeaderRecord*);
} HeaderRecord;

HeaderRecord* HeaderRecord_creation (void*, void*, void*, void*);
void HeaderRecord_destruction(HeaderRecord*);
void init_next_file (HeaderRecord*, string, string, string);
void add_to_header(HeaderRecord* self);
size_t compute_file_size(string);

typedef struct Zziper
{
    size_t number_of_files;
    size_t size_in_bytes;
    HeaderRecord* h_record;
    string root_directory;
    FILE* output_dump;
    FILE* archive_file;

    void (*destructor) (struct Zziper*);
    void (*searcher) (struct Zziper*, string);
    void (*add_to_dump) (struct Zziper*, string, string);
    void (*merge) (struct Zziper*);
    void (*file_from_dump) (struct Zziper*, size_t, string, string);
    void (*read_dump) (struct Zziper*, string);
} Zziper;


// Zziper methods
Zziper* Zziper__creation (string, void*, void*, void*, void*, void*);
void Zziper_destruction(Zziper*);
void list_directory(Zziper* self, string dir_name);
void merge (Zziper* self);
void file_from_dump (Zziper* self, size_t, string, string);
void add_to_dump(Zziper*, string, string);
void read_dump (Zziper*, string);

#endif //SIMPLEARCHIVER_ZZIPER_H