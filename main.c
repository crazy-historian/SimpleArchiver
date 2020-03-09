#include "source/zziper.h"


int main()
{

    FILE *outfile;
    outfile = fopen("output.txt", "r");
    Zziper* zip = Zziper__creation(&list_directory, &Zziper_destroy);
    zip->searcher(zip, ".", outfile);
    zip->destructor(zip);
    fclose(outfile);


}
