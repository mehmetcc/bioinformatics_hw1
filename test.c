#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>

int read_from_file() {
       static const char filename[] = "input.fa";
   FILE *file = fopen ( filename, "r" );
   char *dest[2048];
   if ( file != NULL )
   {
      char line [ 128 ]; /* or other suitable maximum line size */
      while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
      {
         strcat(dest,line);
      }

      printf("%s", dest);
      fclose ( file );
   }
   else
   {
      perror ( filename ); /* why didn't the file open? */
   }
   return 0;


}

int main() {
    read_from_file();
}