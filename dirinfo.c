#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

char * strrpt(char * str, int n){
  char * rtrnStr = malloc(n * sizeof(char));
  while(n){
    strcat(rtrnStr, str);
    n--;
  }
  return rtrnStr;
}

char * permisshelp(int x){
    char * rtrnStr = malloc(3 * sizeof(char));

    if(x >= 4){
        x -= 4;
        strcat(rtrnStr, "r");
    }else{
        strcat(rtrnStr, "-");
    }

    if(x >= 2){
        x -= 2;
        strcat(rtrnStr, "w");
    }else{
        strcat(rtrnStr, "-");
    }

    if(x >= 1){
        x -= 1;
        strcat(rtrnStr, "x");
    }else{
        strcat(rtrnStr, "-");
    }

    return rtrnStr;
}

char * permiss(int x){
    int usr = x / 64;
    x -= usr * 64;
    int grp = x / 8;
    x -= grp * 8;
    int otr = x;

    char * rtrnStr = malloc(9 * sizeof(char));

    sprintf(rtrnStr, "%s%s%s", permisshelp(usr), permisshelp(grp), permisshelp(otr));

    return rtrnStr;
}

char * datasize(long long int s){
    char * rtrnStr = malloc(256 * sizeof(char));

    int numGB = 0;
    int numMB = 0;
    int numKB = 0;

    if(!s){
        rtrnStr = "0 B";
        return rtrnStr;
    }

    if(s > 1000000000){
        sprintf(rtrnStr, "%f GB", s / 1000000000.0);
        return rtrnStr;
    }

    if(s > 1000000){
        sprintf(rtrnStr, "%f MB", s / 1000000.0);
        return rtrnStr;
    }

    if(s > 1000){
        sprintf(rtrnStr, "%f KB", s / 1000.0);
        return rtrnStr;
    }

    sprintf(rtrnStr, "%lld B", s);
    return rtrnStr;
}

char * userid(long stuid){
  return getpwuid(stuid)->pw_name;
}

char * grupid(long stgid){
  return getgrgid(stgid)->gr_name;
}

char * subdir(char * dirname, int numtabs){
  DIR * d = opendir(dirname);
  struct dirent * info = readdir(d);

  struct stat * s = malloc(sizeof(struct stat));
  stat(".", s);

  char * name;
  int size = 0;

  char * stats = malloc(sizeof(char) * 64);
  sprintf(stats, "%sStatistics for Directory: %s\n", strrpt("\t", numtabs), dirname);

  char * dirs = malloc(sizeof(char) * 256);
  strcat(dirs, "Directories: \n");
  char * fils = malloc(sizeof(char) * 256);
  strcat(fils, "Files: \n");
  char * subdirs = malloc(sizeof(char) * 512);

  while (info != NULL) {
    name = info->d_name;
    stat(name, s);
    if (info->d_type == DT_DIR){
      strcat(dirs, strrpt("\t", numtabs + 1));
      strcat(dirs, permiss(s->st_mode));
      strcat(dirs, "  ");
      strcat(dirs, userid(s->st_uid));
      strcat(dirs, "  ");
      strcat(dirs, grupid(s->st_gid));
      strcat(dirs, "  ");
      strcat(dirs, name);
      strcat(dirs, " \n");
      strcat(subdirs, subdir(name, numtabs + 1));
    }else{
      size += s->st_size;
      strcat(fils, strrpt("\t", numtabs + 1));
      strcat(fils, permiss(s->st_mode));
      strcat(fils, "  ");
      strcat(fils, userid(s->st_uid));
      strcat(fils, "  ");
      strcat(fils, grupid(s->st_gid));
      strcat(fils, "  ");
      strcat(fils, name);
      strcat(fils, " \n");
    }
    info = readdir(d);
  }

  char * all = malloc(2048 * sizeof(char));

  strcat(all, stats);
  strcat(all, strrpt("\t", numtabs));
  strcat(all, "Total Directory Size: ");
  strcat(all, datasize(size));
  strcat(all, "\n");

  strcat(all, dirs);
  strcat(all, fils);
  strcat(all, subdirs);

  closedir(d);
  free(s);
  free(dirs);
  free(fils);
  return all;
}

int main(){
  char * container = subdir(".", 0);
  printf("%s", container);
  free(container);
  return 0;
}
