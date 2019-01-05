#include "shell.h"
#include "builtin.h"
#include "ft/str.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

static bool	lookup_path(char *name, char *path, char *res, size_t res_size)
{
	char 			*end;
	const size_t	name_len = ft_strlen(name) + 1;
	size_t			path_len;

	while (1)
	{
		end = strchr(path, ':');
		if (!end)
			end = strchr(path, 0);
		path_len = end - path;
		// TODO: E2BIG ?
		if (path_len + 1 + name_len > res_size)
			continue ;
		memcpy(res, path, path_len);
		res[path_len] = '/';
		memcpy(res + path_len + 1, name, name_len);
		if (access(res, X_OK) != -1)
			return (true);
		if (*end == 0)
			return (false);
		path = end + 1;
	}
}

static int	exec(struct s_shell *shell, size_t argc, char **argv)
{
	t_builtin	builtin;
	char 	bin[PATH_MAX + 1];

	builtin = find_builtin(argv[0]);
	if (builtin)
	 	return builtin(argc, argv, shell);
	if (!lookup_path(argv[0], shell->path + 5, bin, sizeof(bin))
	{
		// Unknown command;
		return (-1);
	}

}

void	exec_buffer(struct s_shell *shell, size_t buffer_size)
{
	char 	**argv;
	size_t	argc;
	size_t	i;
	size_t	j;

	i = 0;
	argc = 0;
	while (i < buffer_size)
		argc += (shell->buffer[i++] == '\0');
	argv = __builtin_alloca((argc + 1) * sizeof(char *));
	argv[argc] = 0;
	i = 0;
	j = 0;
	argv[j++] = (char *)shell->buffer;
	while (i < buffer_size)
		if (shell->buffer[i++] == '\0')
			argv[j++] = (char *)shell->buffer;
	exec(shell, argc, argv);
}
