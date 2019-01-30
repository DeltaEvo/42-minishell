#ifndef EXEC_H
# define EXEC_H

# include "shell.h"

void	exec_buffer(struct s_shell *shell, size_t buffer_size);
bool	lookup_path(char *name, size_t name_len, char *path, char *res, size_t res_len);

#endif
