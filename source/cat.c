#include "unistd.h"
#include "fcntl.h"
#include "errno.h"
#include "libgen.h"
#include "string.h"

void	ft_putstr(char *str)
{
	while (*str)
		write(1, str++, 1);
}

void	print_error_msg(char *file, char *program)
{
	ft_putstr(basename(program));
	ft_putstr(": ");
	ft_putstr(file);
	ft_putstr(": ");
	ft_putstr(strerror(errno));
	ft_putstr("\n");
	errno = 0;
}

void	display_file(int fd, char *file, char *program)
{
	long			size;
	unsigned char	c;

	size = read(fd, &c, 1);
	while (size)
	{
		if (errno)
		{
			print_error_msg(file, program);
			return ;
		}
		write(1, &c, size);
		size = read(fd, &c, 1);
	}
}

int	main(int argc, char *argv[])
{
	int		fd;
	int		i;
	char	*program;

	program = argv[0];
	if (argc == 1)
		display_file(0, 0, program);
	else
	{
		i = 0;
		while (++i < argc)
		{
			fd = open(argv[i], O_RDONLY);
			if (fd == -1)
			{
				print_error_msg(argv[i], program);
				continue ;
			}
			display_file(fd, argv[i], program);
			close(fd);
		}
	}
	return (0);
}