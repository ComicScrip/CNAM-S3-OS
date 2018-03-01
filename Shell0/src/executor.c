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

#include "../include/sds.h"
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

void execute_pipeline_sync(pipeline* p) {
  int child_exit_status = -1;
  int ret_fork = fork();

  if(ret_fork == -1) {  handle_error(1, "Could not fork"); }
  else if (ret_fork == 0) { // pipeline process
    int nb_commands = p->simple_commands->size;
    if(nb_commands == 1) {
      execute_sync(p->simple_commands->head->data);
    }

    int nb_pipes = nb_commands - 1;
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
    simple_command* s = pipeline_get_next_simple_command(p);
    execute_simple_command(s);
  } else { // shell process
    waitpid(ret_fork, &child_exit_status, 0);
    //sc->exit_status = child_exit_status;
  }
}

void execute_sync(simple_command* sc) {
  int child_exit_status = -1;
  int ret_fork = fork();

  switch (ret_fork){
      case -1: handle_error(1, "Could not fork"); break;
      case 0 : execute_simple_command(sc); break;
      default:
        waitpid(ret_fork, &child_exit_status, 0);
        sc->exit_status = child_exit_status;
  }
}

void execute_simple_command(simple_command* sc) {
  //apply_redirections(sc);
  execvp(sc->name, sc->argv);
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
