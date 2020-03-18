#include "source/zziper.h"

// TODO: класс HeaderRecord содержит указаетль на файл заголовка (done)
// TODO: создать объявление record'a внутри конструктора Zziper'а (done)
// TODO: запись бинарного файла (done)
// TODO: контроль открытия/закрытия файлов
// TODO: реализовать примитивный парсер бинарного файла без буфера в виде string (done)
// TODO: октрытие закрытие бинарных файлов через имена, хранящиеся в структурах
// TODO: как передавать информацию о корневой директории в заголовок (возможно, через символ ".")
// TODO: создать рекурсивный обход строки, содержащую полное распожоление разахивированного файла
// TODO: предусмотреть повторяющиеся пути


int main()
{

     //Zziper* zip = Zziper__creation("D:\\Web", &list_directory, &Zziper_destruction, &add_to_dump, &read_dump, &merge);
//    zip->searcher(zip, "D:\\Web");
//    zip->destructor(zip);
    //file_from_dump(zip, 0, "", "D:\\Web\\HTML\\myfirstpage");
   string* tokens = tokenization("D:\\Web\\HTML\\myfirstpage", "\\");
   concatenate_path(tokens, "\\");
   free(tokens);


}
