#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>

#define shell_readline_buffersize 1000000
#define shell_token_buffersize 100000
#define shell_token_delimiters " \t\r\n\a"

int shell_cd(char **a);
int shell_exit(char **a);
int shell_help(char **a);



char *builtin_commands[]={"cd","help","exit"};
int (*builtin_functions[])(char **) = {&shell_cd,&shell_help,&shell_exit};

int shell_no_builtins()
{
	return sizeof(builtin_commands)/sizeof(char *);
}

int shell_cd(char **a)
{
	if(a[1]==NULL)
		fprintf(stderr,"expected argument to \"cd\"\n");
	else
	{
		if(chdir(a[1]) != 0)
		{
			perror("shell");
		}
	}
	return 1;
}	

int shell_help(char **a)
{
   int i;
   printf("Type program names and arguments and hit enter\n");
   printf("The following are built-in\n");
   for(i=0;i<shell_no_builtins();i++)
   	printf(" %s\n",builtin_commands[i] );
   printf("Use the man command for information of other programs.\n");
   return 1;
}

int shell_exit(char **a)
{
    return 0;
}
char *shell_read_line()
{
  int buffer_size = shell_readline_buffersize;
  int pos = 0;
  char *buffer = malloc(sizeof(char) * buffer_size);
  int p;

  if (!buffer) {
    fprintf(stderr, "allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {

    p = getchar();

    if (p == EOF || p == '\n') {
      buffer[pos] = '\0';
      return buffer;
    } 
    else {
      buffer[pos] = p;
    }
    pos++;

    if (pos >= buffer_size) {

      buffer_size += shell_readline_buffersize;
      buffer = realloc(buffer, buffer_size);
      if (!buffer) {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}


void Dir() 
{ 
    char arr[1024]; 
    getcwd(arr, sizeof(arr)); 
    printf("\n%s", arr); 
} 


char **shell_split_line(char *line)
{
  int buffer_size = shell_token_buffersize, pos = 0;
  char **tokens = malloc(buffer_size * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, shell_token_delimiters);
  while (token != NULL)
   {
    tokens[pos] = token;
    pos++;

    if (pos >= buffer_size) 
    {
      buffer_size += shell_token_buffersize ;
      tokens = realloc(tokens, buffer_size * sizeof(char*));
      if (!tokens)
       {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
       }
    }

    token = strtok(NULL, shell_token_delimiters);
  }

  tokens[pos] = NULL;
  return tokens;
}

int shell_launch(char **args)
{
  pid_t pid, wait_pid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("error in shell");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("error in fork");
  } else {
    // Parent process
    do {
      wait_pid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int shell_execute(char **a)
{
	int i;
	if(a[0]==NULL)
		return 1;
	for(i=0;i<shell_no_builtins();i++)
	{
		if(strcmp(a[0],builtin_commands[i]) == 0)
			return (*builtin_functions[i])(a);
	}
	return shell_launch(a);
}

void loop()
{
  char *l;
  char **a;
  int s;

  do {
    Dir();
    printf(" > ");
    l = shell_read_line();
    a = shell_split_line(l);
    s = shell_execute(a);

    free(l);
    free(a);
  } while (s);
}

int main(int argc, char **argv)
{
     loop();
   
     return EXIT_SUCCESS;
}