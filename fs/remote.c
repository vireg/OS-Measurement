#include <stdio.h>

int main(void){
   char url[255] = "http://google.com";

   char command[1024];
   snprintf(command, 1024, "wget -c '%s'", url);
   system(command);

   return 0;
}
