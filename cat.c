#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int fd;
	fd=open(argv[1], O_RDONLY);
	if(fd < 0){
		fprintf(stderr, "%s\n", "error");
		exit(0);
	}
	char buff[128];
	int r = read(fd, buff, 100);
	write(1, buff, r);
	return(0);
}