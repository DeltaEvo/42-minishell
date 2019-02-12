/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 14:09:52 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/02/12 13:39:19 by dde-jesu         ###   ########.fr       */
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

bool		lookup_path(char *name, size_t name_len, char *path, char *res,
		size_t res_size)
{
	char	*end;
	size_t	path_len;

	while (1)
	{
		end = ft_strchr(path, ':');
		if (!end)
			end = path + ft_strlen(path);
		path_len = end - path;
		if (path_len + 1 + name_len + 1 > res_size)
		{
			path = end + 1;
			continue ;
		}
		ft_memcpy(res, path, path_len);
		res[path_len] = '/';
		ft_memcpy(res + path_len + 1, name, name_len);
		res[path_len + 1 + name_len] = '\0';
		if (access(res, X_OK) != -1)
			return (true);
		if (*end == 0)
			return (false);
		path = end + 1;
	}
}

int	exec_binary(char *path, size_t argc, char **argv, char **env)
{
	char			bin[PATH_MAX + 1];
	int				pid;
	int				status;
	const size_t	av0_size = ft_strlen(argv[0]);

	if (ft_memchr(argv[0], '/', av0_size) && av0_size < sizeof(bin))
		ft_memcpy(bin, argv[0], av0_size + 1);
	else if (!(path
		&& lookup_path(argv[0], av0_size, path + 5, bin, sizeof(bin))))
	{
		ft_putf_fd(2, "%s: command not found\n", argv[0]);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
		exit(2);
	else if (pid != 0)
		return (waitpid(pid, &status, 0) == 0 ? status : 1);
	execve(bin, argv, env);
	perror("minishell");
	exit(1);
}

static int	exec(struct s_shell *shell, size_t argc, char **argv)
{
	t_builtin		builtin;
	const size_t	av0_size = ft_strlen(argv[0]);

	if ((builtin = find_builtin(argv[0], av0_size)))
		return (builtin(argc, argv, shell));
	else
		return (exec_binary(shell->path, argc, argv, shell->env));
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
