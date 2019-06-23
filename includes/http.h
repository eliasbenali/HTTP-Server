#ifndef HTTP_H
# define HTTP_H

# include <fcntl.h>
# include <inttypes.h>
# include <netdb.h>
# include <signal.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/mman.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <pthread.h>

# include "queue.h"

/*
** HTTP Server configuration
*/

# define DEFAULT_PORT		6060
# define CONNECTION_BACKLOG	4
# define TIME_OUT		10
# define CRLF			"\r\n"

#ifndef PAGE_SIZE
# define PAGE_SIZE		getpagesize()
#endif

typedef struct		s_http
{
	int				fd;
	uint8_t			method;
	char			*path;
	uint8_t			protocol;

	char			*accept;
	uint8_t			content_type;
	char			*boundary;

	size_t			content_length;
	t_queue			*content;
}			t_http;

typedef struct		s_content
{
	char			*key;
	char			*value;
	uint8_t			flag;
}			t_content;

int				socket_int(void);
int				socket_bind(int fd, int port, char **address);
int				socket_accept(int fd, char **address);

int				request(int fd, t_http **data, int *status);

/*
** signal.c
*/
void				exit_server(void);
void				sighandler_init(void);

#endif
