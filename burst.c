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
#include <getopt.h>

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
		//printf("arg->start: %d\n", arg->start);
		arg->end;
		arg->outfd;
		arg->fbuf;
  writefile(arg->start, arg->end, arg->outfd, arg->fbuf);
}
int writefile(int start, int end, int outfd, char* fbuf) {
		char buf[1];
		//printf("\nIn writefile\n");
	for(int i = start; i<=end; i++){
		buf[0]= fbuf[i];
		ssize_t bytesread = 1;
		ssize_t byteswrote;
		//writing to a file
		//printf("\n write file : %c",fbuf[i]);
	while (((byteswrote = write(outfd, buf, bytesread)) == -1) &&
		   (errno == EINTR))
	  ;
	// "real" write error
	if (byteswrote == -1){
		fprintf(stderr, "Error in writing to file\n");
		 return 1;
		}
	}//end of for loop
//close output file desc
close(outfd);
  return 0;
}
int countlines(int lsize, char *filename, char *fbuf){
int infd1 = STDIN_FILENO;
	infd1 = open(filename, O_RDONLY);
  if (infd1 < 0) {
	perror("Input open error");
	//printf("Input err");
	return 1;
  }
char buf[BUFSIZE];
int lines =0;
int l = 0;
	   for(int l=0;l<lsize;++l){
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
	   // printf("close the file\n");
		}
return lines;
}

struct option longopts[] = {
  { "help",               no_argument,       NULL, 'h'},
  { "version",            no_argument,       NULL, 'V'},
  { "lines",           required_argument, NULL, 'l'},
 };

int main(int argc, char* argv[]) {
  int flag = 0;
  int oc;
  int a = 500;
  int option_index = 0;
  while ((oc = getopt_long(argc, argv, "hVl:", (struct option*) longopts, &option_index)) != -1) {

    // invalid options
    if (oc == '?') {
      fprintf(stderr, "invalid argument: -%c\n", oc);
      continue;
    }

	//fprintf(stderr, "li: %d\n oc: %c\n", option_index, oc);

    // generic argument reporter
//    fprintf(stderr, "gen--%s", longopts[option_index].name);
    if (longopts[option_index].has_arg == required_argument)
      fprintf(stderr, "=%s", optarg);
    fprintf(stderr, "\n");
    // continue;

    switch (oc) {
	case 'h':
flag =1;
	fprintf(stderr, "Report bugs to: rs189@zips.uakron.edu\n");
fprintf(stderr, "burst home page: <http://www.gnu.org/software/burst/>\n");
fprintf(stderr, "General help using GNU software: <http://www.gnu.org/gethelp/>\n");
break;
    case 'V':
flag =1;
fprintf(stderr, "GNU  burst 2.9\nCopyright (C) 2007 Free Software Foundation, Inc.\n");
fprintf(stderr, "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n");
fprintf(stderr, "This is free software: you are free to change and redistribute it.\n");
fprintf(stderr, "There is NO WARRANTY, to the extent permitted by law. \n");
      break;
    case 'l':
      	a = atoi(optarg);
      //fprintf(stderr, "-lines= %s\n", optarg);
      // printf("a: %d\n",a);
		 if(a==0){
		 fprintf(stderr, "Invalid number to split\n"); 
		 }
         break;
        default:
        break;
    };
  }
if(flag==1){
return 0;
}

 /* Print any remaining command line arguments (not options). */
char* inpfile2;
 if(optind < argc){
        //  printf ("\n non-option ARGV-elements: \n");
      //while (optind < argc)
      //printf ("%s ", argv[optind++]);
     inpfile2 = argv[optind++];
	
}
char* inpfile = malloc(strlen(inpfile2 + 1));
strcpy(inpfile, inpfile2);
struct stat statbuf;
int status = stat(inpfile2, &statbuf);
if(status == -1){
perror("Error in getting status");
return 1;
}
unsigned int lsize = statbuf.st_size;
char *fbuf = malloc(lsize);
int lines = countlines(lsize, inpfile, fbuf);
//printf("lines in file: %d\n", lines);
int numcopiers;
if(lines>0 && a>0){
numcopiers = lines/a;
if((lines%a)!=0){
numcopiers+=1;
}
}
//printf("Numcopiers: %d\n",numcopiers);
  struct t_args* threadinfo = calloc(numcopiers, sizeof(struct t_args));
		//tokenize the input file name to get the output file name
		char *head = strtok(inpfile, ".");
		char *ext = strtok(NULL," ");
		//printf("head: %s\n ext: %s\n", head, ext);
int start = 0;
int end;
int flines = 0;
int count = 0;
for(int i=0;i<lsize;++i){
//printf("buf[i]: %c\n", fbuf[i]);
if(fbuf[i] == '\n'){
++flines;
int mod = flines%a;
//printf("mod: %d",mod);
if(mod == 0){
end = i;
//create a new file
 char filename[MAXFILENAME];
 snprintf(filename, MAXFILENAME, "%s%d.%s", head, count+1, ext);
// fprintf(stderr, "%s\n", filename);
 //printf("Filename : %s \n",filename);
 int outfd = STDOUT_FILENO;
 outfd = open(filename, W_FLAGS, W_PERMS);
	if(outfd<0){
		perror("Error in opening the Output file");
//		printf("\nError in op file opening\n");
		return 1;
		}
//spawn a thread
threadinfo[count].start = start;
threadinfo[count].end = end;
threadinfo[count].outfd = outfd;
threadinfo[count].fbuf = fbuf;
//printf("start:  %d\n",threadinfo[count].start);
//printf("end:  %d\n",threadinfo[count].end);
int st =  pthread_create(&threadinfo[count].tid, NULL, burst_thread, &threadinfo[count]);
if(st==0){
//printf("\n No Error in thread creation\n");
}
//reset start variable
start = i+1;
count++;
//printf("Count value >> %d \n", count);
flines = 0;
}//end if mod
}//if end \n
if(numcopiers == count+1) {
		break;
}
}//end of for loop
/*
		//close output file descriptor
		if(outfd != STDOUT_FILENO){
		close(outfd);
		}
*/
if(numcopiers == count+1) {
//	printf("For last chunk of the input file \n");
		start = end+1;
		end = lsize-1;
		//create a new file
		 char filename[MAXFILENAME];
		 snprintf(filename, MAXFILENAME, "%s%d.%s", head, count+1, ext);
//		 fprintf(stderr, "%s\n", filename);
		 //printf("Filename : %s \n",filename);
		 int outfd = STDOUT_FILENO;
		 outfd = open(filename, W_FLAGS, W_PERMS);
				if(outfd<0) {
						perror("Error in opening the Output file");
//						printf("\nError in op file opening\n");
						return 1;
				}
		//spawn a thread
		threadinfo[count].start = start;
		threadinfo[count].end = end;
		threadinfo[count].outfd = outfd;
		threadinfo[count].fbuf = fbuf;
//		printf("Last chunk start:  %d\n",threadinfo[count].start);
//		printf("Last chunk end:  %d\n",threadinfo[count].end);
		int st =  pthread_create(&threadinfo[count].tid, NULL, burst_thread, &threadinfo[count]);
		if(st==0){
//		 printf("\n No Error in thread creation\n");
		}
}
//printf("Im about to enter for loop \n");
for(int x=0;x<numcopiers-1;++x)
{
//	printf("start for loop %d\n", x);
	if(pthread_join(threadinfo[x].tid, NULL)){
	//printf("Error joining thread %d\n", x);
	}
	//printf("end for loop %d\n", x);
}
//printf("All threads executed successfully \n");
pthread_exit(NULL);
return 0;
}
