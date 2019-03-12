/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 14:09:52 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/12 14:27:57 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "builtin.h"
#include "ft/str.h"
#include "ft/io.h"
#include "ft/mem.h"

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/stat.h>

char		*lookup_path(char *name, size_t name_len, char *path)
{
	static char	bin[PATH_MAX + 1];
	char		*end;
	size_t		path_len;

	while (1)
	{
		end = ft_strchr(path, ':');
		if (!end)
			end = path + ft_strlen(path);
		path_len = end - path;
		if (path_len + 1 + name_len + 1 > sizeof(bin))
		{
			path = end + 1;
			continue ;
		}
		ft_memcpy(bin, path, path_len);
		bin[path_len] = '/';
		ft_memcpy(bin + path_len + 1, name, name_len);
		bin[path_len + 1 + name_len] = '\0';
		if (access(bin, F_OK) != -1)
			return (bin);
		if (*end == 0)
			return (NULL);
		path = end + 1;
	}
}

extern int g_pid;

bool		check_binary(char *bin, bool print)
{
	struct stat	stats;

	if (!bin)
		return (false);
	if (lstat(bin, &stats) != 0)
	{
		if (print)
			ft_putf_fd(2, "minishell: command not found: %s\n", bin);
		return (false);
	}
	if (!(stats.st_mode & S_IXUSR))
	{
		if (print)
			ft_putf_fd(2, "minishell: permission denied: %s\n", bin);
		return (false);
	}
	if (!(S_ISREG(stats.st_mode) || S_ISLNK(stats.st_mode)))
	{
		if (print)
			ft_putf_fd(2, "minishell: not a regular file: %s\n", bin);
		return (false);
	}
	return (true);
}

int			exec_binary(char *path, char **argv, char **env)
{
	char			*bin;
	int				pid;
	int				status;
	const size_t	av0_size = ft_strlen(argv[0]);

	if (ft_memchr(argv[0], '/', av0_size))
		bin = argv[0];
	else if (!(path
		&& (bin = lookup_path(argv[0], av0_size, path + 5))))
	{
		ft_putf_fd(2, "minishell: command not found: %s\n", argv[0]);
		return (-1);
	}
	if (!check_binary(bin, true))
		return (-1);
	pid = fork();
	if (pid == -1)
		exit(2);
	else if (pid != 0)
	{
		g_pid = pid;
		return (waitpid(pid, &status, 0) == 0 ? status : 1);
	}
	execve(bin, argv, env);
	exit(1);
}

static int	exec(struct s_shell *shell, size_t argc, char **argv)
{
	t_builtin		builtin;
	const size_t	av0_size = ft_strlen(argv[0]);

	if ((builtin = find_builtin(argv[0], av0_size)))
		return (builtin(argc, argv, shell));
	else
		return (exec_binary(shell->path, argv, shell->env));
}

void		exec_buffer(struct s_shell *shell, size_t buffer_size)
{
	char	**argv;
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
	argv[j++] = (char *)shell->buffer + shell->env_size;
	while (i < buffer_size)
		if (shell->buffer[shell->env_size + i++] == '\0')
			argv[j++] = (char *)(shell->buffer + shell->env_size + i);
	argv[argc] = 0;
	exec(shell, argc, argv);
}
