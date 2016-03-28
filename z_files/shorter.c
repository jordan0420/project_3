#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int id;
    int charge;
    FILE *fp;
    FILE *of;
    char * line=NULL;
    ssize_t len = 0;
    size_t read;
    fp = fopen("Moogle.txt","r");
    of = fopen("data.txt","w");
    for (int i=0; i<40; i++)
    {
        read = getline(&line,&len, fp);
            id = atoi(line);
        read = getline(&line,&len,fp);
            charge = atoi(line);
         fprintf(of,"%i\n", id);
        fprintf(of,"%i\n", charge);
    }
    fclose(fp);
    fclose(of);
exit(1);
}
