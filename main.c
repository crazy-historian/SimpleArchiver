
#include "source/zziper.h"


int main() {

//    Zziper zip;
////    Zziper__init(&zip);
////
////    repr(&zip);
////    printf("%d %s\n", zip.number_of_files, " - number of files");
////
////   FILE *outfile;
////
////   outfile = fopen("outfile.txt", "a+");
////   list_directory(&zip, ".", outfile);
////   free(zip.files);
////   fclose(outfile);


    HeaderRecord* record = HeaderRecord_creation(10, "Name", "Address",&destroy_record, &add_to_record);
    record->destructor(record);






}
