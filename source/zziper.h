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
string concatenate(string, string, string);
string clip_substring(string, string);
int check_path (string address);

typedef struct string_list
{
    size_t size;
    size_t ptr_to_last;
    string* list;

} string_list;

void append(struct string_list, int n, ...);
void free_string_list(string_list*);
string_list* tokenization(string, string);

string concatenate_path(string_list*, string);

typedef struct HeaderRecord
{
    FILE* archive_header;
    size_t header_size_in_bytes;
    size_t number_of_lines;

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
    size_t size_in_bytes;
    HeaderRecord* h_record;
    string root_directory;
    string out_directory;
    FILE* output_dump;


    void (*destructor) (struct Zziper*);
    void (*searcher) (struct Zziper*, string);
    void (*add_to_dump) (struct Zziper*, string, string);
    void (*merge) (struct Zziper*);
    int (*file_from_dump) (struct Zziper*, int, long int, string, string);
    void (*create_directory) (struct Zziper*, string_list, size_t);
    void (*read_dump) (struct Zziper*, string);
} Zziper;


// Zziper methods
Zziper* Zziper__creation (string, void*, void*, void*, void*, void*);
void Zziper_destruction(Zziper*);
void list_directory(Zziper* self, string dir_name);
void merge (Zziper* self);
int file_from_dump (Zziper* self, int, long int, string, string);
void create_directory  (struct Zziper*, string_list*);
void add_to_dump(Zziper*, string, string);
void read_dump (Zziper*, string);

#endif //SIMPLEARCHIVER_ZZIPER_H