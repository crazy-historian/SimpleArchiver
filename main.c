#include "source/zziper.h"

// TODO: класс HeaderRecord содержит указаетль на файл заголовка (done)
// TODO: создать объявление record'a внутри конструктора Zziper'а (done)
// TODO: запись бинарного файла (done)
// TODO: контроль открытия/закрытия файлов


int main()
{



    Zziper* zip = Zziper__creation(&list_directory, &Zziper_destruction, &add_to_dump, &read_dump, &create_archive);
    zip->searcher(zip, "D:\\1");
    //zip->read_dump(zip, "output.txt");
    zip->create_archive(zip);
    zip->destructor(zip);

}
