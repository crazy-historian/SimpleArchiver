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
    int len = strlen(in) + 1;
    cmd = malloc(len);
    snprintf(cmd, len,"%s", in);
    return cmd;
}

string create_new_path(string file_name, string dir_name)
{
    string path;
    int len = strlen(dir_name)+1;
    path = malloc(len * 2);
    strcpy(path, dir_name);
    strcat(path, "/");
    strcat(path, file_name);
    return path;
}

/* HeaderRecord methods:
 *
 *  1. constructor
 *  2. destructor
 *  3. add info about next file in directory to archive header
 *  4. compute size of current file
 *
 */

HeaderRecord* HeaderRecord_creation (string name, string  address,
                                    void* destructor, void* add, void* compute_file_size)
{
    HeaderRecord* new = malloc(sizeof(HeaderRecord));
    new->compute_file_size = compute_file_size;
    new->name = name;
    new->address = address;
    new->destructor = destructor;
    new->add_to_output = add;
    new->compute_file_size(new, name);

    return new;
}
void destroy_record(HeaderRecord* record)
{
    printf("\nDestroy record\n");
    free(record);
}

void add_to_record(HeaderRecord* self, FILE *outfile)
{ fprintf(outfile, ("%s %s %lu\n"), self->name, self->address, self->number_of_bytes); }

void compute_file_size(HeaderRecord *self, string file_name)
{
    FILE *current_file;
    current_file = fopen(file_name, "r");
    fseek(current_file, SEEK_SET, SEEK_END);
    uint number = ftell(current_file);
    self->number_of_bytes = number;
    printf("Tne %s size is %lu", file_name, number);
    fclose(current_file);

}
// zziper methods

Zziper* Zziper__creation(void* searcher, void* destroy)
{   Zziper* new = malloc(sizeof(Zziper));
    new->files = (string*) malloc(sizeof(string));
    new->number_of_files = 0;
    new->archive_name = NULL;
    new->path = NULL;

    new->searcher = searcher;
    new->destructor = destroy;
    return new;
}
void Zziper_destroy(Zziper* record)
{
    printf("\nDestroy Zziper\n");
    free(record);
}

void list_directory(Zziper* self, string dir_name, FILE *outfile)
{
    DIR *directory;
    directory = opendir(dir_name);
    struct dirent *dir_record;
    HeaderRecord* record;

    if (directory)
    {
        while ((dir_record = readdir(directory)) != NULL)
        {
            if (strcmp(dir_record->d_name, ".") != 0 && strcmp(dir_record->d_name, "..") != 0)
            {
                if (dir_record->d_type == DT_DIR)
                {
                    printf("%s %s \n", "New level of recursion:", dir_record->d_name);
                    string path = create_new_path(dir_record->d_name, dir_name);
                    list_directory(self, path, outfile);
                }
                else
                {
                    string file_name = create_new_path(dir_record->d_name, dir_name);
                    record = HeaderRecord_creation(file_name, dir_name,
                            &destroy_record, &add_to_record, &compute_file_size);
                    record->destructor(record);

                }

            }
        }
        closedir(directory);
    }
}

void repr (Zziper* self)
{
    printf("\nThe list of scanned files\n");

    for (int i=0; i < self->number_of_files; i++)
        printf("%s\n", self->files[i]);

}