//
// Created by Professional on 08.03.2020.
//
#include "zziper.h"

/*
 *  STRING METHODS:
 *  1. concatenate three strings
 *  2. clib substring  from main string
 *
 */

string concatenate(string first, string second, string third)
{
    size_t len = strlen(first) + strlen(second) + strlen(third) + 1;
    string new_string = (string) malloc(len);
    strcpy(new_string, first);
    strcat(new_string, second);
    strcat(new_string, third);
    return new_string;
}

string concatenate_path (string_list* tokens, string symbol)
{
    size_t len = strlen(tokens->list[0]) + 2;
    string path = (string) malloc (len);

    strcpy(path, tokens->list[0]);
    strcat(path,symbol);

    for(size_t i = 1; i < tokens->ptr_to_last; i++)
        {
            len += strlen(tokens->list[i]) + 2;
            path = realloc (path, len);
            strcat(path, tokens->list[i]);
            strcat(path, symbol);
        }

    return path;

}

string clip_substring(string main, string sub)
{
    string clipped = &main[strlen(sub)];
    return clipped;
}

int check_path(string address)
{
    DIR *new_dir;
    new_dir = opendir(address);
    if (new_dir)
        return 1;
    else return -1;
}
/*
 *  STRING_LIST METHODS
 *  1. tokenization
 *  2. destructor
 */

string_list* tokenization (string main, string symbol)
{
    string main_copy = strdup(main);
    int number_of_tokens = 0;
    for (size_t i = 0; i < strlen(main_copy); i++)
        if ((int) main_copy[i] == (int)*(symbol))
            number_of_tokens++;
    number_of_tokens++;

    // STRING_LIST INITIALIZATION
    string_list* tokens = malloc(sizeof(string_list));
    tokens->list = (string*) malloc(number_of_tokens * sizeof(string));
    tokens->size = number_of_tokens;
    tokens->ptr_to_last = 0;

    //
    int counter = 0;
    string token = strtok(main_copy, symbol);
    while (token != NULL)
    {
        size_t token_size = strlen(token);
        tokens->list[counter] = (string)malloc(token_size+1);
        tokens->list[counter] = token;
        token = strtok(NULL, symbol);
        counter ++;
    }
    return tokens;
}


void free_string_list(string_list* s_list)
{
    for(int i = 0; i < s_list->size; i++)
        free(s_list->list);
    free(s_list);
}
/* HEADER_RECORD METHODS:
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
    fseek(new->archive_header, 8, SEEK_SET);
    new->file_name = NULL;
    new->file_address = NULL;

    new->number_of_lines = 0;
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

void init_next_file(HeaderRecord* self, string file_name, string address, string root_directory)
{
    self->file_name = file_name;
    string clipped_address = clip_substring(address, root_directory);
    self->file_address = clipped_address;
    //string full_file_name = create_new_path(file_name, address);
    string dump_file_name = concatenate(address, "\\", file_name);
    self->file_size_in_bytes = self->compute_file_size(dump_file_name);
    free(dump_file_name);
}

/*
 *  ZZIPER METHODS:
 *
 *  1. constructor
 *  2. destructor
 *  3. recursive list traversal
 *  4. add file in directory to dump
 *  5. merge header and dump
 *  6. read dump
 *  7. unpack file from dump
 *
 */

Zziper* Zziper__creation(string root_directory, void* searcher, void* destroy,
        void* add_to_dump, void* read_dump, void* merge)
{
    Zziper* new = malloc(sizeof(Zziper));
    new->root_directory = root_directory;
    new->out_directory = "K:"; /* FIXME: DEBUG VARIANT*/
    new->h_record = HeaderRecord_creation(&HeaderRecord_destruction, &add_to_header,
                                                                 &compute_file_size, &init_next_file);
    new->output_dump = fopen("dump.bin", "wb");

    new->size_in_bytes = 0;

    new->searcher = searcher;
    new->destructor = destroy;
    new->add_to_dump = add_to_dump;
    new->merge = merge;
    new->read_dump = read_dump;
    return new;
}

void Zziper_destruction(Zziper* zip)
{
    printf("\nDestroy Zziper\n");
    printf("Files: %zu; Bytes: %zu", zip->h_record->number_of_lines, zip->size_in_bytes);
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
                    //string path = create_new_path(record_in_dir->d_name, dir_name);
                    string path = concatenate(dir_name, "\\", record_in_dir->d_name);
                    list_directory(self, path);
                }
                else
                {
                    record->init_next_file (record, record_in_dir->d_name, dir_name, self->root_directory);
                    record->add_to_header (record);
                    self->add_to_dump(self, record_in_dir->d_name, dir_name);

                    record->number_of_lines++;
                    self->size_in_bytes += record->file_size_in_bytes;

                }
            }
        }
        closedir(directory);
    }
    record->header_size_in_bytes = compute_file_size("header.bin");
    fclose(record->archive_header);
    fclose(self->output_dump);
}

void add_to_dump (Zziper* self, string file_name, string dir_name)
{
    char byte[1];
    //string full_file_name = create_new_path(file_name, dir_name);
    string full_file_name = concatenate(dir_name, "\\", file_name);

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

void merge (Zziper* self)
{
    self->output_dump = fopen("dump.bin", "rb");
    self->h_record->archive_header = fopen("header.bin", "rb+");
    FILE* header = self->h_record->archive_header;
    FILE* dump = self->output_dump;

    fseek(self->h_record->archive_header, 0, SEEK_SET);
    fwrite(&self->h_record->number_of_lines, 4, 1, header);
    fwrite(&self->h_record->header_size_in_bytes, 4, 1, header);
    fseek(header, 0, SEEK_END);

    char byte;
    while((byte = fgetc(dump)) != EOF)
    {
        fputc(byte, header);
    }

    fclose(header);
    fclose(dump);
    remove("dump.bin");
}

void read_dump(Zziper* self, string full_file_name)
{
    // открытие файла архива FIXME: имена промежуточных файлов поместить в структуры
    self->output_dump = fopen("header.bin", "rb");
    FILE* dump = self->output_dump;

    // чтение двух чисел: размер заголовка в байтах и число строк в нем
    long int* num_of_lines = malloc(sizeof(long int));
    long int* size_of_header = malloc(sizeof(long int));
    fread(num_of_lines, 1, 4, dump);
    fread(size_of_header, 1, 4, dump);

    // указатели на позиции в файлах
    int ptr_in_header = 0;
    int ptr_in_dump = 0;

    // размер строки с именем файла и его расположением, считываемый символ из заголовка
    size_t string_size = 0;
    int byte;

    for (long int i = 0; i < *(num_of_lines); i++)
    {
        long int *number = malloc(sizeof(long int)); // FIXME: вынести выделение памяти за цикл
        size_t num = fread(number, 1, 4, dump);
        if (!num)
            return;
        else
            {
                printf("\nNumber: %li\n", *(number));
                // FIXME: исправить на while()
                for (;;)
                {
                    byte = fgetc(dump);
                    string_size++;
                    if (byte == '\n')
                        {
                            string_size--;
                            string buffer = malloc(string_size * sizeof(char));
                            fseek(dump, -string_size, SEEK_CUR);
                            fread(buffer, 1, string_size, dump);
                            buffer[string_size - 1] = '\0';
                            printf("String: %s", buffer);

                            string_list* tags  = tokenization(buffer, "|");

                            /* *
                             *  Алгоритм перемещения указателя:
                             *
                             *  1. если читается первый файл - устанавливаем указатель в дампе на начало дампа
                             *  2. после прочтения файла указатель в дампе изменится на позиции конца прочтенного файла
                             *  3. после прочтения файла указатель возвращается на новую строку в заголовке
                             * */

                            ptr_in_header = ftell(dump);

                            if (ptr_in_dump == 0)
                            {
                                fseek(dump, *(size_of_header), SEEK_SET);
                                ptr_in_dump = ftell(dump);
                            }
                            ptr_in_dump = file_from_dump(self, ptr_in_dump, *(number), tags->list[0], tags->list[1]);
                            fseek(dump, ptr_in_header, SEEK_SET);


                            string_size = 0;
                            free(number); free(buffer); free_string_list(tags);
                            break;
                        }
                }
            }

    }
    free(num_of_lines);
    fclose(dump);
}

int file_from_dump (Zziper* self, int position, long int file_size,  string name, string address)
{
    // указатель ставится на конец предыдущего файла
    fseek(self->output_dump, position, SEEK_SET);

    // первоначальный путь
    string path = (string) malloc (strlen(self->out_directory) + strlen(address) + 1);
    strcpy(path, self->out_directory);
    strcat(path, address);

    // разбиение пути на токены и создание соответсвующей директории
    string_list* tokens = tokenization(path, "\\");
    create_directory(self, tokens);

    // чтение в буфер файла из дампа
    string buffer = malloc(file_size);
    fread(buffer, file_size, 1, self->output_dump);

    // создание имени файла в новой директории
    string new_file_name = realloc(path, strlen(path) + strlen(name) + 2);
    strcat(new_file_name, "\\");
    strcat(new_file_name, name);
    FILE* unpacked_file = fopen(new_file_name, "wb");

    //запись распакованного файла в созданной директории
    fwrite(buffer, file_size, 1, unpacked_file);

    // закрытие файлов и освобождение выделенной памяти
    fclose(unpacked_file);
    free(tokens); free(buffer); free(path); free(new_file_name);

    // указатель ставиться на конец новосозданного файла
    position = ftell(self->output_dump);
    return position;
}

void create_directory (Zziper* self, string_list *tokens)
{
    tokens -> ptr_to_last += 1;
    string path = concatenate_path(tokens, "\\");
    if(check_path(path) < 0)
        mkdir(path);
    if (tokens->ptr_to_last != tokens->size)
        create_directory(self, tokens);

}