#include "shell.h"
#include "builtin.h"
#include "ft/str.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

bool	lookup_path(char *name, size_t name_len, char *path, char *res, size_t res_size)
{
	char 			*end;
	size_t			path_len;

	while (1)
	{
		end = strchr(path, ':');
		if (!end)
			end = strchr(path, 0);
		path_len = end - path;
		// TODO: E2BIG ?
		if (path_len + 1 + name_len + 1 > res_size)
			continue ;
		memcpy(res, path, path_len);
		res[path_len] = '/';
		memcpy(res + path_len + 1, name, name_len);
		res[path_len + 1 + name_len] = '\0';
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
	int		pid;
	int		status;

	builtin = find_builtin(argv[0]);
	if (builtin)
	 	return builtin(argc, argv, shell);
	if (strchr(argv[0], '/'))
		strcpy(bin, argv[0]);
	else if (!(shell->path && lookup_path(argv[0], ft_strlen(argv[0]), shell->path + 5, bin, sizeof(bin))))
	{
		ft_putf_fd(2, "%s: command not found\n", argv[0]);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
		exit(2);
	else if (pid != 0)
	{
		waitpid(pid, &status, 0);
		return (status);
	}
	execve(bin, argv, shell->env);
	perror("minishell");
	exit(0);
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
		argc += (shell->buffer[shell->env_size + i++] == '\0');
	argv = __builtin_alloca((argc + 1) * sizeof(char *));
	i = 0;
	j = 0;
	argv[argc] = 0;
	argv[j++] = (char *)shell->buffer + shell->env_size;
	while (i < buffer_size)
		if (shell->buffer[shell->env_size + i++] == '\0')
			argv[j++] = (char *)(shell->buffer + shell->env_size + i);
	argv[argc] = 0;
	exec(shell, argc, argv);
}
