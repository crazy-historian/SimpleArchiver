//
// Created by Professional on 08.03.2020.
//
#include "zziper.h"

/*
 *  String methods:
 *

 */

string create_new_path(string file_name, string dir_name)
{
    string path;
    size_t len = strlen(dir_name) + 1;
    len += strlen(file_name) + 3;
    path = malloc(len);
    strcpy(path, dir_name);
    strcat(path, "\\");
    strcat(path, file_name);
    return path;
}

string concatenate(string first, string second, string third)
{
    size_t len = strlen(first) + strlen(second) + strlen(third) + 1;
    string new_string = (string) malloc(len);
    strcpy(new_string, first);
    strcpy(new_string, first);
    strcpy(new_string, first);
    return new_string;
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

HeaderRecord* HeaderRecord_creation (void* destructor, void* add, void* compute_file_size , void* init_new_file)
{
    HeaderRecord* new = malloc(sizeof(HeaderRecord));
    new->archive_header = fopen("header.bin", "wb");
    // FIXME: размер uint - 4 это магическое число
    //fseek(new->archive_header, 4, SEEK_SET);
    new->file_name = NULL;
    new->file_address = NULL;
    new->header_size_in_bytes = 4;
    new->file_size_in_bytes = 0;

    new->compute_file_size = compute_file_size;
    new->destructor = destructor;
    new->add_to_header = add;
    new->init_next_file = init_new_file;
    return new;
}

void HeaderRecord_destruction(HeaderRecord* header)
{
    printf("\nDestroy record\n");
    fclose(header->archive_header);
    free(header);
}

void add_to_header(HeaderRecord* self)
{
    char sym[1] = "|";
    fwrite(&self->file_size_in_bytes, 4, 1, self->archive_header); fwrite(sym, 1, 1, self->archive_header);
    fwrite(self->file_name, strlen(self->file_name), 1,self->archive_header); fwrite(sym, 1, 1, self->archive_header);
    sym[0] = '\n';
    fwrite(self->file_address, strlen(self->file_address), 1, self->archive_header); fwrite(sym, 1, 1, self->archive_header);
    self->header_size_in_bytes += (6 + strlen(self->file_name) + strlen(self->file_address));
}
size_t compute_file_size(string file_name)
{
    FILE *current_file;
    size_t size_in_bytes;
    current_file = fopen(file_name, "rb");
    fseek(current_file, SEEK_SET, SEEK_END);
    size_in_bytes = ftell(current_file);
    fclose(current_file);
    return size_in_bytes;

}

void init_next_file(HeaderRecord* self, string file_name, string address)
{
    self->file_name = file_name;
    self->file_address = address;
    string full_file_name = create_new_path(file_name, address);
    self->file_size_in_bytes = self->compute_file_size(full_file_name);
    self->header_size_in_bytes += self->file_size_in_bytes;
    free(full_file_name);
}

/*
 *  Zipper methods:
 *
 *  1. constructor
 *  2. destructor
 *  3. recursive list traversal
 *
 */

Zziper* Zziper__creation(void* searcher, void* destroy, void* add_to_dump, void* read_dump, void* create_archive)
{
    Zziper* new = malloc(sizeof(Zziper));
    new->h_record = HeaderRecord_creation(&HeaderRecord_destruction, &add_to_header,
                                                                 &compute_file_size, &init_next_file);
    new->output_dump = fopen("dump.bin", "wb");
    new->number_of_files = 0;
    new->size_in_bytes = 0;

    new->searcher = searcher;
    new->destructor = destroy;
    new->add_to_dump = add_to_dump;
    new->create_archive = create_archive;
    new->read_dump = read_dump;
    return new;
}

void Zziper_destruction(Zziper* zip)
{
    printf("\nDestroy Zziper\n");
    printf("Files: %lu; Bytes: %lu", zip->number_of_files, zip->size_in_bytes);
    free(zip);
}

// TODO: обработка исключений с целью выяснить, какие файлы не были открыты
// TODO: проверить возможность переноса специальных структуры в Zziper
void list_directory(Zziper* self, string dir_name)
{
    DIR *directory;
    directory = opendir(dir_name);
    struct dirent *record_in_dir;
    HeaderRecord* record = self->h_record;

    if (directory)
    {
        while ((record_in_dir = readdir(directory)) != NULL)
        {
            if (strcmp(record_in_dir->d_name, ".") != 0 && strcmp(record_in_dir->d_name, "..") != 0)
            {
                if (record_in_dir->d_type == DT_DIR)
                {
                    string path = create_new_path(record_in_dir->d_name, dir_name);
                    list_directory(self, path);
                }
                else
                {
                    record->init_next_file (record, record_in_dir->d_name, dir_name);
                    record->add_to_header (record);

                    self->add_to_dump(self, record_in_dir->d_name, dir_name);
                    self->number_of_files++;
                    self->size_in_bytes += record->file_size_in_bytes;

                }

            }
        }
        closedir(directory);
    }
    fclose(record->archive_header);
    fclose(self->output_dump);
}

void add_to_dump (Zziper* self, string file_name, string dir_name)
{
    char byte[1];
    string full_file_name = create_new_path(file_name, dir_name);
    FILE* current_file = fopen(full_file_name, "rb");
    while (!feof(current_file) && !ferror(current_file))
    {
        if (fread(byte,1, 1, current_file) == 1)
            fwrite(byte, 1, 1, self->output_dump);
    }
    fclose(current_file);
    free(full_file_name);

}/*
 *  Алгоритм:
 *  1. окрытие файла заголовка и файла-дампа
 *  2. создание файла-архива
 *  3. в файл-архив добавить размер заголовка
 *  4. в файл-архив добавить заголовок
 *  5. в файл-архив добавить дамп
 *  6. закрыть файл-архив
 */

void create_archive (Zziper* self)
{
    self->output_dump = fopen("dump.bin", "rb");
    self->h_record->archive_header = fopen("header.bin", "rb+");
    FILE* header = self->h_record->archive_header;
    FILE* dump = self->output_dump;
    fseek(self->h_record->archive_header, 0, SEEK_SET);
    fwrite(&self->h_record->header_size_in_bytes, 4, 1, self->h_record->archive_header);
    fseek(header, 0, SEEK_END);

    char byte;
    while((byte = fgetc(dump)) != EOF)
    {
        fputc(byte, header);
    }

    fclose(header);
    fclose(dump);
}


/*
 * Алгоритм:
 * 1. открыть файл
 * 2. переместить указатель на нужную позицию
 * 3. считать n байт
 */
void read_dump(struct Zziper* self, string full_file_name)
{
    FILE *dump = fopen("header.bin", "rb");
    size_t string_size = 0;
    int byte;

    for (;;)
    {
        long int *number = malloc(sizeof(long int));
        // FIXME: проверка количества прочтенных элементов
        size_t num = fread(number, 1, 4, dump);
        if (!num)
        { return;}
        else
            {
                printf("\nNumber: %li\n", *(number));
                free(number);

                for (;;)
                {
                    byte = fgetc(dump);
                    string_size++;
                    if (byte == '\n') {
                        string_size--;
                        string buffer = malloc(string_size * sizeof(char));
                        fseek(dump, -string_size, SEEK_CUR);
                        fread(buffer, 1, string_size, dump);
                        buffer[string_size - 1] = '\0';
                        printf("String: %s", buffer);

                        string_size = 0;
                        free(buffer);
                        break;
                    } else if (byte == EOF)
                        return;
                }
            }

    }

}