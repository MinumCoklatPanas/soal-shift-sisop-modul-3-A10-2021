#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/wait.h> 
#include<string.h> 
#include<sys/types.h> 

int main() 
{ 
	int pipe1[2], pipe2[2]; 
    // Used to store two ends of first pipe and Used to store two ends of second pipe 
	pid_t p1,p2;
	if (pipe(pipe1)==-1) //nilai failure
	{ 
		fprintf(stderr, "Pipe Gagal" ); 
		return 1; 
	} 
	if (pipe(pipe2)==-1)  //nilai failure
	{ 
		fprintf(stderr, "Pipe Gagal" ); 
		return 1; 
	} 
	p1=fork();
	if (p1< 0) 
	{ 
		fprintf(stderr, "Fork Gagal" ); //tidak dapat dibentuk
		return 1; 
	}  

    //parent
    else if (p1 > 0) {
    p2 = fork();
    if (p2< 0) 
	{ 
		fprintf(stderr, "Fork Gagal" ); //tidak dapat dibentuk
		return 1; 
	}  

    else if(p2 > 0){    
        dup2(pipe2[0], 0);
        
        close(pipe2[0]);
        close(pipe2[1]);

        char *arg1[] = {"head", "-5", NULL};
        execvp("/usr/bin/head", arg1);
        exit(0);
    }

    else{
        //read = 0; write = 1
        close(pipe2[0]);
        close(pipe2[1]);

        dup2(pipe1[0], 0); //baca isi dr pipe1 untuk dieksekusi
        dup2(pipe2[1], 1); // output ke pipe2

        close(pipe1[0]);
        close(pipe1[1]);

        char *arg2[] = {"sort", "-nrk 3,3",NULL};
        execvp("/bin/sort", arg2);
    }
    }

    
	else{ //jika fork()==0
    	dup2(pipe1[1], 1);//write

		close(pipe1[0]);//close read
        close(pipe1[1]);//close write

        char *arg2[] = {"ps", "aux",NULL};
        execvp("/bin/ps", arg2);
	} 
} 
