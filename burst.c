/*
  burst.c

  Counts the lines in the listed files and writes to multiple files using pthreads

 Roopa Singarapu
 rs189@zips.uakron.edu

*/

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define BUFSIZE 4096
#define MAXFILENAME 512
#define R_FLAGS O_RDONLY
#define W_FLAGS (O_WRONLY | O_CREAT)
#define W_PERMS (S_IRUSR  | S_IWUSR)

struct copy_t {
  int infd;
  int outfd;
  int status;
  pthread_t tid;
};

void* copyfile_thread(void* args) {
  struct copy_t* copyinfo = args;
  int totalbytes = 0;
 return (void *) copyfile(copyinfo->infd, copyinfo->outfd, &totalbytes);
}


int copyfile(int infd, int outfd, int* totalbytes) {
	char buf[BUFSIZE];
	int lines = 0;
    while (1) {
    // read a block
    ssize_t bytesread = read(infd, buf, BUFSIZE);
    if ((bytesread == -1) && (errno == EINTR))
      continue;
    if (bytesread == -1) {
      puts("I HAD AN ERROR");
      return 1;
    }
    if (bytesread == 0)
      break;
//count the number of lines
 for (int i = 0; i < bytesread; ++i){
      if (buf[i] == '\n'){
        ++lines;
	if(lines%500 == 0){
	break;

      }//500 lines check ends
     }//new line check ends
    }//for loop end
    // write the block that was read
    ssize_t byteswrote;
    while (((byteswrote = write(outfd, buf, bytesread)) == -1) &&
           (errno == EINTR))
      ;

    // "real" write error
    if (byteswrote == -1){
     	printf("Error in writing to file\n");
	 return 1;
	}
	if(lines%500 == 0){
	break;
	}
  }//end of while

  return infd;
}

int countlines(int argc, char *filename){
int infd1 = STDIN_FILENO;
  if (argc > 1) {
    infd1 = open(filename, O_RDONLY);
  }
  if (infd1 < 0) {
    perror("Input open error");
    printf("Input err");
    return 1;
  }
char buf[BUFSIZE];
int lines =0;
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
        //and writing to the file
    for (int i = 0; i < bytesread; ++i){
      if (buf[i] == '\n'){
        ++lines;
        }
       }
 }//end of while
// close the input file
  if (infd1 != STDIN_FILENO){
	close(infd1);
        printf("close the file\n");
        }
return lines;
}

int main(int argc, char* argv[]) {

char* inpfile = malloc(strlen(argv[1]) + 1);
strcpy(inpfile, argv[1]);
int lines = countlines(argc, inpfile);
printf("lines in file: %d\n", lines);

int numcopiers;
if(lines>0){
numcopiers = lines/500;
}
//printf("Numcopiers: %d",numcopiers);

int infd1 = STDIN_FILENO;
if(argc > 1){
infd1 = open(argv[1], R_FLAGS);
}
if(infd1 < 0){
perror("error in input");
}
	//tokenize the input file name to get the output file name
	char *head = strtok(inpfile, "."); 
	char *ext = strtok(NULL," ");
	//printf("head: %s\n ext: %s\n", head, ext); 

void* status;
struct copy_t* copyinfo = calloc(numcopiers, sizeof(struct copy_t));
for(int i=0; i< numcopiers; ++i){

	char filename[MAXFILENAME];
	snprintf(filename, MAXFILENAME, "%s%d.%s", head, i+1, ext);
	fprintf(stderr, "%s\n", filename);

	int outfd = STDOUT_FILENO;
	outfd = open(filename, W_FLAGS, W_PERMS);
	if(outfd<0){
	perror("Error in opening the Output file");
	return 1;
	}
	// use the thread to copy the file
    copyinfo[i].infd = infd1;
    copyinfo[i].outfd = outfd;
    printf("Creating the thread\n");
	if(pthread_create(&copyinfo[i].tid, copyfile_thread, NULL, &copyinfo[i])!=0){
	printf("Error in creating thread\n");
	}
	infd1 =(int)status;
	printf("status: %d", infd1);
}

/*
	//close output file descriptor
	if(outfd != STDOUT_FILENO){
	close(outfd);
	}

*/
return 0;
}
