#include "source/zziper.h"

// TODO: класс HeaderRecord содержит указаетль на файл заголовка (done)
// TODO: создать объявление record'a внутри конструктора Zziper'а (done)
// TODO: запись бинарного файла


int main()
{



    Zziper* zip = Zziper__creation(&list_directory, &Zziper_destruction, &add_to_dump);

    zip->searcher(zip, "D:\\Web");

    zip->destructor(zip);



}
