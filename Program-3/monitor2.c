#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// headers
void get_file_stats(char str[]);
void get_current_dir();
void kill_parent(int pid);
void kill_child(int chid);
void read_dir();
void sigIntHandler(int s);
void sigSTPHandler(int s);
void sigQUITHandler(int s);

// globals
FILE* file;
struct stat filestats;
struct dirent* dp;
int* chid;
int* pid;

int main(int argc, char* argv[]) {
  chid = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  pid = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

  *pid = getpid();
  printf("Program id: %d\n", *pid);

  while (1) {
    if (fork() == 0) {
      char str[100];
      *chid = getpid();
      printf("Child id: %d\n", *chid);

      printf("\033[0;34m");
      printf("Stat Prog");
      printf("\033[0m");
      printf("$\n");

      scanf("%s", str);
      if (strcmp(str, "q") == 0) {  // killing program
        // kill_parent(*pid);          // need to kill parent first, otherwise child will end without parent death
        kill_child(*chid);
        return 1;
        // printf("not dead\n");

      } else if (strcmp(str, "list") == 0) {  // reading all files in directory

        read_dir();
        return 1;

      } else if ((file = fopen(str, "r")) != NULL) {
        get_file_stats(str);
        return 1;
      }

    } else {
      signal(SIGINT, sigIntHandler);
      signal(SIGQUIT, sigQUITHandler);
      signal(SIGSTOP, sigSTPHandler);

      wait(NULL);
      printf("\033[0;34m");
      printf("-----waiting-----\n\n");
      printf("\033[0m");
      sleep(5);
    }
  }
  return 1;
}

void get_file_stats(char str[]) {
  if (stat(str, &filestats) < 0) {  // reading file stats
    printf("FILE NOT FOUND!\n");
  } else {
    printf("\033[1;31m");
    printf("\n-----File Stats-----\n");
    printf("\033[0m");
    printf("File Size: %lld\n", filestats.st_size);
    printf("File inode:   %d\n", (int)filestats.st_ino);
    printf("File dev id:   %d\n", (int)filestats.st_dev);
    printf("File mode:   %08x\n", filestats.st_mode);
    printf("File links:   %d\n", filestats.st_nlink);
    printf("File uid:   %d\n", (int)filestats.st_uid);
    printf("File gid:   %d\n\n", (int)filestats.st_gid);
  }
}

void kill_parent(int pid) {
  printf("\nkilling parent!\n");
  kill(pid, SIGINT);
}
void kill_child(int chid) {
  printf("\nkilling child!\n");
  kill(chid, SIGINT);
}

void read_dir() {
  DIR* dir = opendir("./");

  while ((dp = readdir(dir)) != NULL) {
    printf("\033[01;33m");
    printf("File: %s\n", dp->d_name);

    sleep(1);
  }
  printf("\n");
  printf("\033[0m");
  closedir(dir);
}

void sigIntHandler(int s) {
  // kill_child(*chid);
  // kill_parent(*pid);
}  // printf("Can't kill parent!\n")
void sigSTPHandler(int s) {
  // kill_child(*chid);
  // kill_parent(*pid);
}
void sigQUITHandler(int s) {
  // kill_child(*chid);
  // kill_parent(*pid);
}
