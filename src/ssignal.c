#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 29

int main ( int argc, char **argv )
{
  printf( "Hello WOrld!!\n\n" );

  char *arr = "Hello from the writer process";

  while (1)
  {
      // Open FIFO for write only
      int fd = open("./my_fifo", O_WRONLY);

      // Take an input arr2ing from user.
      // 80 is maximum length
      write(fd, arr, strlen(arr)+1);

      close(fd);
  }


}
