/*
  burst.c

  splitting the given file into multiple files using threads

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

struct t_args {
 int start;
 int end;
  int outfd;
  char *fbuf;
  int status;
  pthread_t tid;
};

void* burst_thread(void* args) {
  struct t_args* arg = args;
  	arg->start;
	printf("arg->start: %d", arg->start);
	arg->end;
	arg->outfd;
	arg->fbuf;
  writefile(arg->start, arg->end, arg->outfd, arg->fbuf);
}

int writefile(int start, int end, int outfd, char* fbuf) {
	char buf[1];
    for(int i = start; i<end; ++i){
  	ssize_t bytesread = fbuf[i];
	ssize_t byteswrote;
	//writing to a file
    while (((byteswrote = write(outfd, buf, bytesread)) == -1) &&
           (errno == EINTR))
      ;

    // "real" write error
    if (byteswrote == -1){
     	printf("Error in writing to file\n");
	 return 1;

	}
    }//end of for loop
  return 0;
}


int countlines(int lsize, char *filename, char *fbuf){
int infd1 = STDIN_FILENO;
    infd1 = open(filename, O_RDONLY);

  if (infd1 < 0) {
    perror("Input open error");
    printf("Input err");
    return 1;
  }
char buf[BUFSIZE];
int lines =0;
int l = 0;
       for(int l=0;l<lsize;++l){
    	//while(l<lsize){
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
	fbuf[l]=buf[i];//
	++l;
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
struct stat statbuf;
int status = stat(argv[1], &statbuf);
if(status == -1){
perror("Error in getting status");
return 1;
}
unsigned int lsize = statbuf.st_size;
//printf("file size: %d", lsize);

char *fbuf = malloc(lsize);
int lines = countlines(lsize, inpfile, fbuf);
//printf("fbuf: %s",fbuf);
//printf("lines in file: %d\n", lines);
int numcopiers;
if(lines>0){
numcopiers = lines/20;
if((lines%20)!=0){
numcopiers+=1;
}
}
printf("Numcopiers: %d",numcopiers);

  struct t_args* threadinfo = calloc(numcopiers, sizeof(struct t_args));
	//tokenize the input file name to get the output file name
	char *head = strtok(inpfile, "."); 
	char *ext = strtok(NULL," ");
	//printf("head: %s\n ext: %s\n", head, ext); 

int start = 0;
int end;
int flines = 0;
int count = 0;
//printf("lsize: %d\n",lsize);
for(int i=0;i<lsize;++i){
//printf("buf[i]: %c\n", fbuf[i]);
if(fbuf[i] == '\n'){
++flines;
//printf("i: %d  flines: %d\n",i,flines);

//printf("flines: %d",flines);
int mod = flines%20;
//printf("mod: %d",mod);
if(mod==0){ 
//printf("inside 20 flines: %d",flines);

end = i;
//printf("end: %d",end);
//printf("create new file\n");
//create a new file 
 char filename[MAXFILENAME];
 snprintf(filename, MAXFILENAME, "%s%d.%s", head, count+1, ext);
 fprintf(stderr, "%s\n", filename);
 //printf("Filename : %s \n",filename);
 int outfd = STDOUT_FILENO;
 outfd = open(filename, W_FLAGS, W_PERMS);
    if(outfd<0){
        perror("Error in opening the Output file");
        printf("\nError in op file opening\n");
        return 1;
        }
//spawn a thread
//printf("spawining a thread\n");
//printf("Count 1: %d    flines: %d", count,flines);
threadinfo[count].start = start;
threadinfo[count].end = end;
threadinfo[count].outfd = outfd;
threadinfo[count].fbuf = fbuf;//
//printf("fbuf: %s\n",fbuf);
printf("\ntinfo fbuf: %s\n",threadinfo[count].fbuf);
pthread_attr_t attr;
int s = pthread_attr_init(&attr);
int st =  pthread_create(&threadinfo[count].tid,&attr, &burst_thread, &threadinfo[count]);
if(st==0){
//printf("Error in thread creation");
}
//reset start variable
//printf("Resetting the start variable\n");
start = i;
count++;
//printf("Count inc: %d    flines: %d\n",count, flines);
//flines = 0;
}//end if \n
}//if end mod
}//end of for loop 
/*
	//close output file descriptor
	if(outfd != STDOUT_FILENO){
	close(outfd);
	}

*/
return 0;
}
