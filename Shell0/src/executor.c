#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>

#include "../include/executor.h"
#include "../include/simple_command.h"
#include "../include/utils.h"

void execute_cmd_in_pipeline (simple_command* sc, int in, int out)
{
  pid_t pid;
  if ((pid = fork ()) == 0){
    if (in != STDIN_FILENO){
      dup2(in, STDIN_FILENO);
      close(in);
    }
    if (out != STDOUT_FILENO){
      dup2(out, STDOUT_FILENO);
      close(out);
    }

    execute_simple_command(sc);
  }
}

void execute_pipeline(pipeline* p, int async, shell* s) {
  int child_exit_status = -1;
  int ret_fork = fork();

  if(ret_fork == -1) {  handle_error(1, "Could not fork"); }
  else if (ret_fork == 0) { // pipeline process
    int nb_commands = p->simple_commands->size;

    int nb_pipes = nb_commands - 1;
    //printf("nb pipes : %d\n", nb_pipes);
    simple_command* sc = NULL;
    int in, fd_pipe[2];
    // The first process should get its input from STDIN_FILENO
    in = STDIN_FILENO;
    // Spawn all commands, execpt the last in the pipeline
    for (int i = 0; i < nb_pipes; i++) {
      sc = pipeline_get_next_simple_command(p);
      pipe(fd_pipe);
      execute_cmd_in_pipeline(sc, in, fd_pipe[1]);
      // Close end of the pipe, the child will write here.
      close (fd_pipe[1]);
      // The next child will read from the read end of the pipe
      in = fd_pipe[0];
    }

    // Last command of pipeline, set STDIN_FILENO be the read end of the previous pipe
    if (in != STDIN_FILENO) {
      dup2 (in, STDIN_FILENO);
    }
    // Execute the last command in the current process
    simple_command* last = pipeline_get_next_simple_command(p);
    int exit_status_last = EXIT_FAILURE;
    int fork_last = fork();
    if(fork_last == -1) {  handle_error(1, "Could not fork"); }
    else if (fork_last == 0 && last != NULL) {
      execute_simple_command(last);
    }
    else {
      waitpid(fork_last, &exit_status_last, 0);
      exit((WIFEXITED(exit_status_last) && WEXITSTATUS(exit_status_last)) == EXIT_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE);
    }
  } else { // shell process
    if(!async) {
      //printf("\nwaiting for %d\n", ret_fork);
      waitpid(ret_fork, &child_exit_status, 0);
      //printf("\nwaited for %d, ret : %d\n", ret_fork, child_exit_status);
      shell_set_var(s, "?", child_exit_status == 0 ? "0" : "1");
    }
  }
}

void execute_pipeline_list(pipeline_list* pl, shell* s) {
  pipeline* p = pipeline_list_get_next_pipeline(pl);
  pipeline* previous_p = NULL;
  char* exit_status_previous = NULL;
  char exit_code_var[2] = "?";

  for(int i=0; i< pl->pipelines->size; i++){
    if(previous_p != NULL){
      if(previous_p->terminating_token == SEPARATOR || previous_p->terminating_token == ASYNC) {
        execute_pipeline(p, p->terminating_token == ASYNC, s);
      } else { // AND, OR
        exit_status_previous = shell_get_variable(s, exit_code_var);
        if(exit_status_previous != NULL && (
          (previous_p->terminating_token == AND && exit_status_previous[0] == '0') ||
          (previous_p->terminating_token == OR && exit_status_previous[0] != '0')
        )){
          execute_pipeline(p, 0, s);
        }
      }
      exit_status_previous = shell_get_variable(s, exit_code_var);
    } else {
      execute_pipeline(p, p->terminating_token == ASYNC, s);
    }
    previous_p = p;
    p = pipeline_list_get_next_pipeline(pl);
  }
}

void set_env_variables(simple_command* sc) {
  char* va = NULL;
  char** va_parts = NULL;
  char* name = NULL;
  char* value = NULL;
  int nb_parts = 0;

  while((va = simple_command_get_next_variable_assignement(sc))){
    printf("\nva : --%s--\n", va);
    va_parts = split(va, '=', &nb_parts);
    printf("%d\n", nb_parts);
    if(nb_parts == 2){
      name = va_parts[0];
      value = va_parts[1];
      printf("\nsetting env k: --%s-- v:--%s--, va:%s\n", name, value, va);
      // dont use putenv, it keeps a referecne to the assignment string and does not copy it
      //int ret_se = setenv(name, value, 1);
      //printf("\nsetenv : %d\n", ret_se);
      // dont use setenv() neiher https://rachelbythebay.com/w/2014/08/16/forkenv/ JUST DO NOTHING, ITS NOT POSSIBLE...
    }
    //for(int j = 0; j < nb_parts; j++) free(va_parts[j]);
    //free(va_parts);
  }
}

void execute_simple_command(simple_command* sc) {
  apply_redirections(sc);
  if(strlen(sc->name) > 0){
    //set_env_variables(sc);
    //setenv("TEST", "42", 1);
    char* env[] = {"TEST=42", NULL};
    /*char* assignment = NULL;
    for(int i = 0; i < sc->env_assignements->size; i++) {
      env = realloc(env, sizeof(char*) * (i + 1));

      env[i] = realloc(env[i], sizeof(char) * strlen(assignment))
    }*/
    //handle_error((execve(sc->name, sc->argv, env)) == -1, "err exec");
    handle_error(execvpe(sc->name, sc->argv, env) == -1, "exec error");
  }
}

void apply_redirections(simple_command* sc) {
  for(char * ri = simple_command_get_next_redirection_intent(sc); ri != NULL; ri = simple_command_get_next_redirection_intent(sc)){
    if(contains(ri, ">>")){
      // 'cmd >> file' means redirect STDOUT, 'cmd 2>> file means' STDERR,
      // 'cmd n>> file' with n>2 means open a new file descriptor that points to the file
      int stream_to_redirect = STDOUT_FILENO;
      int first_chevron_idx = index_of(ri, ">", 1);
      if(first_chevron_idx > 0) {
        char* stream_to_redirect_str = calloc(first_chevron_idx, sizeof(char));
        strncpy(stream_to_redirect_str, ri, first_chevron_idx);
        stream_to_redirect = atoi(stream_to_redirect_str);
        free(stream_to_redirect_str);
      }
      char* file_name = &ri[2 + first_chevron_idx];
      int fd = open(file_name, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
      handle_error(dup2(fd, stream_to_redirect) < 0, "dup error");
      close(fd);
    }
    else if (contains(ri, ">")) {
      if(contains(ri, "&")) { // 'cmd > file 2>&1' mean redirect all from STDERR wherever STDOUT is going
        int stream_to_redirect = STDOUT_FILENO;
        int first_chevron_idx = index_of(ri, ">", 1);
        if(first_chevron_idx > 0) {
          char* stream_to_redirect_str = calloc(first_chevron_idx, sizeof(char));
          strncpy(stream_to_redirect_str, ri, first_chevron_idx);
          stream_to_redirect = atoi(stream_to_redirect_str);
          free(stream_to_redirect_str);
        }

        int fd = 1;
        int idx_ampersand = index_of(ri, "&", 1);
        if(idx_ampersand > 0) {
          char* fd_str = calloc((strlen(ri) - idx_ampersand - 1), sizeof(char));
          strcpy(fd_str, &ri[idx_ampersand + 1]);
          fd = atoi(fd_str);
          free(fd_str);
        }
        handle_error(dup2(fd, stream_to_redirect) < 0, "dup error");
        close(fd);
      } else {
        int stream_to_redirect = STDOUT_FILENO;
        int first_chevron_idx = index_of(ri, ">", 1);
        if(first_chevron_idx > 0) {
          char* stream_to_redirect_str = calloc(first_chevron_idx, sizeof(char));
          strncpy(stream_to_redirect_str, ri, first_chevron_idx);
          stream_to_redirect = atoi(stream_to_redirect_str);
          free(stream_to_redirect_str);
        }
        char* file_name = &ri[1 + first_chevron_idx];
        // not appending, reseting the file
        if(access(file_name, F_OK ) == 0 ) {
          handle_error(remove(file_name) != 0, "could not remove !");
        }
        int fd = open(file_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        handle_error(dup2(fd, stream_to_redirect) < 0, "dup error");
        close(fd);
      }
    }
    else if (contains(ri, "<")) {
      int stream_to_redirect = STDIN_FILENO;
      int first_chevron_idx = index_of(ri, "<", 1);
      if(first_chevron_idx > 0) {
        char* stream_to_redirect_str = calloc(first_chevron_idx, sizeof(char));
        strncpy(stream_to_redirect_str, ri, first_chevron_idx);
        stream_to_redirect = atoi(stream_to_redirect_str);
        free(stream_to_redirect_str);
      }
      char* file_name = &ri[1 + first_chevron_idx];
      int fd = open(file_name, O_RDONLY);
      handle_error(dup2(fd, stream_to_redirect) < 0, "dup error");
      close(fd);
    } else if (contains(ri, "<<")) {
      // TODO: handle heredoc
    }

    // TODO: factor this shit out
    // TODO: handle fd closing (>&-)
    // TODO: find out what the heck is 1>&3-
    // TODO: try to solve that http://tldp.org/LDP/abs/html/ioredirintro.html then kill myself.
  }
}
