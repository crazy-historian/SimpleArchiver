//
// Created by Professional on 08.03.2020.
//

#include "zziper.h"

/*
 *  String methods:
 *
 *  1. get new unknown len string
 *  2. create new path for access to subdirectory
 *
 */

string get_string(const char *in)
{
    string cmd;
    uint len = strlen(in) + 1;
    cmd = malloc(len);
    snprintf(cmd, len,"%s", in);
    return cmd;
}

string create_new_path(string file_name, string dir_name)
{
    string path;
    uint len = strlen(dir_name) + 1;
    len += strlen(file_name) + 2;
    path = malloc(len);
    strcpy(path, dir_name);
    strcat(path, "\\");
    strcat(path, file_name);
    return path;
}

/* HeaderRecord methods:
 *
 *  1. constructor
 *  2. destructor
 *  3. add info about next file in directory to archive header
 *  4. compute size of current file
 *  5. init data attributes of next file in directory
 *
 */

HeaderRecord* HeaderRecord_creation (void* destructor, void* add, void* compute_file_size, void* init_new_file)
{
    HeaderRecord* new = malloc(sizeof(HeaderRecord));
    new->archiver_header = fopen("header.txt", "w");
    new->file_name = NULL;
    new->file_address = NULL;
    new->number_of_bytes = 0;
    new->compute_file_size = compute_file_size;
    new->destructor = destructor;
    new->add_to_header = add;
    new->init_next_file = init_new_file;
    return new;
}

void HeaderRecord_destruction(HeaderRecord* header)
{
    printf("\nDestroy record\n");
    fclose(header->archiver_header);
    free(header);
}

void add_to_header(HeaderRecord* self)
{
    printf(("%s||%s||%lu\n"), self->file_name, self->file_address, self->number_of_bytes);
    fprintf(self->archiver_header, ("%s||%s||%lu\n"), self->file_name, self->file_address, self->number_of_bytes);
}

void compute_file_size(HeaderRecord *self, string file_name)
{
    FILE *current_file;
    current_file = fopen(file_name, "r");
    fseek(current_file, SEEK_SET, SEEK_END);
    uint number = ftell(current_file);
    self->number_of_bytes = number;
    fclose(current_file);

}

void init_next_file(HeaderRecord* self, string file_name, string address)
{
    self->file_name = file_name;
    self->file_address = address;
    string full_file_name = create_new_path(file_name, address);
    self->compute_file_size(self, full_file_name);
}

/*
 *  Zziper methods:
 *
 *  1. constructor
 *  2. destructor
 *  3. recursive list traversal
 *
 */

Zziper* Zziper__creation( void* searcher, void* destroy)
{
    Zziper* new = malloc(sizeof(Zziper));
    new->h_record = record;
    new->number_of_files = 0;
    new->archive_name = NULL;
    new->path = NULL;

    new->searcher = searcher;
    new->destructor = destroy;
    return new;
}

void Zziper_destruction(Zziper* zip)
{
    printf("\nDestroy Zziper\n");
    printf("Files: %lu; Bytes: %lu", zip->number_of_files, zip->bytes);
    free(zip);
}

// TODO: обработка исключений с целью выяснить, какие файлы не были открыты
// TODO: проверить возможность переноса специальных структуры в Zziper
void list_directory(Zziper* self, string dir_name)
{
    DIR *directory;
    directory = opendir(dir_name);
    struct dirent *dir_record;
    HeaderRecord* record = self->h_record;

    if (directory)
    {
        while ((dir_record = readdir(directory)) != NULL)
        {
            if (strcmp(dir_record->d_name, ".") != 0 && strcmp(dir_record->d_name, "..") != 0)
            {
                if (dir_record->d_type == DT_DIR)
                {
                    string path = create_new_path(dir_record->d_name, dir_name);
                    list_directory(self, path);
                }
                else
                {
                    record->init_next_file (record, dir_record->d_name, dir_name);
                    record->add_to_header (record);

                    self->number_of_files++;
                    self->bytes += record->number_of_bytes;
                }
            }
        }
        closedir(directory);
    }
}

void repr (Zziper* self)
{
    printf("\nThe list of scanned files\n");

    for (uint i=0; i < self->number_of_files; i++)
        printf("%s\n");

}