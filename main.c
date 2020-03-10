#include "source/zziper.h"

// TODO: класс HeaderRecord содержит указаетль на файл заголовка (done)
// TODO: создать объявление record'a внутри конструктора Zziper'а (done)


int main()
{


    HeaderRecord* record = HeaderRecord_creation(&HeaderRecord_destruction, &add_to_header, &compute_file_size, &init_next_file);
    Zziper* zip = Zziper__creation(record, &list_directory, &Zziper_destruction);
    zip->searcher(zip, "D:\\test");
    record->destructor(record);
    zip->destructor(zip);

}
