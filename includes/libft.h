#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>

void		strdel(char **str);
void		ptrdel(char **ptr);
char		**strsplit(char const *s, char c);
char		*concat(const char *s1, const char *s2);

void		print_memory(void *addr, size_t size);

#endif
