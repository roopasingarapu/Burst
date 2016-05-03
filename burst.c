/*
  burst.c

  Counts the lines in the listed files
*/

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>

#define BUFSIZE 4096

int main(int argc, char* argv[]) {
int infd1 = STDIN_FILENO;
  if (argc > 1) {
    infd1 = open(argv[1], O_RDONLY);
  }
  if (infd1 < 0) {
    perror("Input open error");
    printf("Input err");
    return 1;
  }
char buf[BUFSIZE];
int lines;
        while(1){
    // read a block
    ssize_t bytesread = read(infd1, buf, BUFSIZE);
    if ((bytesread == -1) && (errno == EINTR))
      continue;
    if (bytesread == -1) {
      puts("I HAD AN ERROR");
      return 1;
    }
    if (bytesread == 0) {
        //printf("end of the file");
        break;
        }
    // increment by the number of newlines in this block
    for (int i = 0; i < bytesread; ++i)
      if (buf[i] == '\n')
        ++lines;
        }
// close the input file
  if (infd1 != STDIN_FILENO)
    close(infd1);

printf("%d",lines);
return 0;
}

