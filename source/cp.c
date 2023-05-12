#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <libgen.h>
#include <string.h>

void print_error_msg(char* file, char* program) 
{
	ft_putstr(basename(program)); 
	ft_putstr(": ");
	ft_putstr(file);
	ft_putstr(": ");			
	ft_putstr(strerror(errno)); 
	ft_putstr("\n");
	errno = 0;
}

//source_file은 읽기전용, dest_file은 쓰기전용으로 open
//source_file 을 dest_file로 copy
//read와 write의 return으로 bytes_read, bytes_written을 비교하어 오류 확인
//오류 발생 시, print_error_msg로 exception 처리
void copy_file(int source_fd, int dest_fd, char* source_file, char* dest_file, char* program)
{
	char buffer[4096];
	ssize_t	bytes_read, bytes_written; 

	//Copy with error check
	while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
		if ((bytes_written = write(dest_fd, buffer, bytes_read)) != bytes_read) {
			print_error_msg(dest_file, program);
			return;
		}
	}

	if (bytes_read < 0) {
		print_error_msg(source_file, program);
	}
}

int main(int argc, char* argv[])
{
	//cp argc error
	if (argc != 3) {
		fprintf(stderr, "Usage: %s source_file dest_file\n", argv[0]);
		return 1;
	}

	char* source_file = argv[1];
	char* dest_file = argv[2];

	int source_fd = open(source_file, O_RDONLY);
	if (source_fd == -1) {
		print_error_msg(source_file, argv[0]);
		return 1;
	}

	int dest_fd = open(dest_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (dest_fd == -1) {
		print_error_msg(dest_file, argv[0]);
		close(source_fd);
		return 1;
	}

	copy_file(source_fd, dest_fd, source_file, dest_file, argv[0]);

	close(source_fd);
	close(dest_fd);

	return 0;
}