#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

int main(){
  DIR * d = opendir(".");
  struct dirent * info = readdir(d);

  struct stat * s = malloc(sizeof(struct stat));
  stat(".", s);

  char * name;
  int size = 0;

  printf("Statistics for Directory: %s\n", ".");

  while (info != NULL) {
    size += s->st_size;
    name = info->d_name;
    if (info->d_type == DT_DIR)
      printf("Directory: %s \n", name);
    else
      printf("File: %s\n", name);
    info = readdir(d);
  }
  printf("Total Directory Size: %d Bytes\n", size);

  closedir(d);
  free(s);
  return 0;
}
