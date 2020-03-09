#include "source/zziper.h"


int main()
{

    FILE *outfile;
    outfile = fopen("output.txt", "w");
    HeaderRecord* record = HeaderRecord_creation(&HeaderRecord_destruction, &add_to_record, &compute_file_size, &init_next_file);
    Zziper* zip = Zziper__creation(record, &list_directory, &Zziper_destruction);
    zip->searcher(zip, "C:\\Third level\\GPSS", outfile);
    zip->destructor(zip);
    record->destructor(record);
    fclose(outfile);


}
