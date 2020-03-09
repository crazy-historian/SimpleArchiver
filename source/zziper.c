//
// Created by Professional on 08.03.2020.
//

#include "zziper.h"

// string methods

string get_string(const char *in)
{
    string cmd;
    int len = strlen(in) + 1;
    cmd = malloc(len);
    snprintf(cmd, len,"%s", in);
    return cmd;
}

// zziper methods

void Zziper__init(Zziper* self)
{
    self->files = (string*) malloc(sizeof(string));
    self->number_of_files = 0;
    self->file_name = NULL;
    self->path = NULL;
}

/* HeaderRecord methods:
 *
 *  1. constructor
 *  2. destructor
 *  3. add info about next file to header
 */

HeaderRecord* HeaderRecord_creation (uint number, string name, string  address, void* destructor, void* add)
{
    HeaderRecord* new = malloc(sizeof(HeaderRecord));
    new->number_of_bytes = number;
    new->name = name;
    new->address = address;
    new->destructor = destructor;
    new->add_to_output = add;

    return new;
}
void destroy_record(HeaderRecord* record)
{free(record);}

void add_to_record(HeaderRecord* self, FILE *outfile)
{ fprintf(outfile, ("%s %s %lu\n"), self->name, self->address, self->number_of_bytes); }

// zziper methods
void list_directory(Zziper* self, string dir_name, FILE *outfile)
{
    string file_name;
    DIR *directory;
    directory = opendir(dir_name);
    struct dirent *dir_record;

    HeaderRecord record;
    static int i = 0;
    int len = 0;

    string path;
    if (directory)
    {
        while ((dir_record = readdir(directory)) != NULL)
        {
            if (strcmp(dir_record->d_name, ".") != 0 && strcmp(dir_record->d_name, "..") != 0)
            {
                if (dir_record->d_type == DT_DIR)
                {
                    printf("%s %s \n", "New level of recursion:", dir_record->d_name);
                    // TODO: сделать функцию get_string супер универсальной
                    len = strlen(dir_name)+1;
                    path = malloc(len * 2);
                    strcpy(path, dir_name);
                    strcat(path, "/");
                    strcat(path, dir_record->d_name);

                    list_directory(self, path, outfile);
                }
                else
                {
                    file_name = dir_record->d_name;
                    len = strlen(file_name) + 1;
                    self->files = (string*)realloc(self->files, sizeof(string) * (i + 2));
                    self->files[i] = (string) malloc(len);
                    snprintf(self->files[i], len, "%s", file_name);
                    printf("%s %s\n", "Simple file: ", self->files[i]);
                    i++;
                    self->number_of_files ++;

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