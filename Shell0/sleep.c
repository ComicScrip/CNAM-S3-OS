#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv, char** env)
{
	sleep(5);
	printf("after 5s\n");

  return EXIT_SUCCESS;
}

