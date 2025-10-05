
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define buffer_size 1024 // it is the buffer size i.e. the size of the prompt
#define Tok_BUFSIZE 64
#define Tok_DELIM "\t\r\n\a "

/*
here we have created a custom getline function to input the user prompt, we cal also use the predefined getline function , 
here we are starting with a memory block of 1kb and then increasing the size as per the requirement 
we have also handled the error if system fails to allocate the memory 
 */
char *lsh_read_line(){
    int buffsize=buffer_size;
    char * buffer=malloc(sizeof(char)*buffsize);
    if(!buffer){
        fprintf(stderr,"sh:allocation failed \n");
        exit(EXIT_FAILURE);
    }
    int c;
    int position=0;
    while(1){
        c= getchar();
        if(c==EOF||c=='\n'){
            buffer[position]='\0';
            return buffer;
        }
        buffer[position++]=c;
        if(position>=buffsize){
            buffsize+=buffer_size;
            buffer=realloc(buffer,buffsize);
            if(!buffer){
                fprintf(stderr,"sh:allocation failed \n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

/*
->in this section we have parsed the input to get the arguments/operations which are to be performed .
for more simplifaction we have used only whitespaces to seperate the arguments here. 
->So the command (echo "this message") would not call echo with a single argument this message,
but rather it would call echo with two arguments: "this and message".
->we will be tokenizing the string using whitespaces as delimiters.
*/

char ** getargs(char* line){
    int buffsize=Tok_BUFSIZE;
    int position=0;
    char** tokens=malloc(buffsize*sizeof(char));
    char* token;
    if(!tokens){
        fprintf(stderr,"sh:allocation failed");
        exit(EXIT_FAILURE);
    }
    token=strtok(line,Tok_DELIM);
    while(token!=NULL){
        tokens[position++]=token;
        if(position>=buffsize){
            buffsize+=Tok_BUFSIZE;
            tokens=realloc(tokens,buffsize);
            if(!token){
                fprintf(stderr,"sh:allocation failed");
                exit(EXIT_FAILURE);
            }
        }
        token=strtok(NULL,Tok_DELIM);
        
    }
    tokens[position]=NULL;
    return tokens;
}

/*
->here comes the heart of the shell,so what a shell does, the kernal here starts 
only one single process (init) which then duplicated and converted to start 
a new process .
->we use the fork() to produce a duplicate process of the current running process,
and the exec() to replace the current running program with and entirly new one 
*/
int lsh_launch(char **args){
    pid_t pid,wpid;
    int status ;
    pid =fork();
    if(pid==0){
        // child process
        if(execvp(args[0],args)==-1){
            perror("sh");
        }
        exit(EXIT_FAILURE);
    }
    else if(pid<0){
        perror("sh");
    }
    else {
        // parent process
        do{
            wpid=waitpid(pid,&status,WUNTRACED);
        }while(!WIFEXITED(status)&&!WIFSIGNALED(status));
    }
    return 1;
}
void lsh_loop(){
    char *line ;
    char ** args;
    int status ;
    // used do while because status is executed once before checking
    do{
        printf(">");
        line =lsh_read_line();
        args =getargs(line);
        status=lsh_execute(args);
    }while(status);
}

// this section covers the shell builtins 
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

char *builtin_str[]={
    "cd","help","exit"
};
int (*builtin_func[])(char**)={
&lsh_cd,&lsh_help,&lsh_exit
};

int lsh_num_builtins(){
    return sizeof(builtin_str)/sizeof(char*);
}
int lsh_cd(char **args){
    if(args[1]==NULL){
        fprintf(stderr,"lsh: expected argument to \"cd\"\n");
    }
    else {
        if(chdir(args[1])!=0){
            perror("lsh");
        }
    }
    return 1;
}

int lsh_help(char **args){
    int i;
    printf("MY LSH\n");
    printf("Please type programe name and arguments and hit enter.\n");
    printf("The following in:\n");
    for(i=0;i<lsh_num_builtins();i++){
        printf(" %s\n",builtin_str[i]);
    }
    printf("Use the man command for information on other programs.\n");
    return 1;
}
int lsh_exit(char **args){
    return 0;
}

int lsh_execute(char **args){
    int i;
    if(args[0]==NULL){
        // Empty command
        return 1;
    }
    for(i=0;i<lsh_num_builtins();i++){
        if(strcmp(args[0],builtin_str[i])==0){
            return(*builtin_func[i])(args);
        }
    }
    return lsh_launch(args);
}

int main(int argc,char** argv){
    lsh_loop();
    exit(EXIT_SUCCESS);

}
