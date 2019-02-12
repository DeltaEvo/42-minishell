/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 16:44:45 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/02/12 15:08:36 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "exec.h"
#include "env.h"
#include "ft/str.h"
#include "ft/mem.h"
#include "ft/args.h"
#include <unistd.h>

#define USAGE "[-i] [name=value]... [utility [argument...]]"
#define DESCRIPTION "env - set the environment for command invocation"
#define I_1 "Invoke utility with exactly the environment specified" 
#define I_2 "by the arguments"

static void	print_env(char **env)
{
	size_t	i;

	i = 0;
	while (env[i])
	{
		write(1, env[i], ft_strlen(env[i]));
		write(1, "\n", 1);
		i++;
	}
}

/*
** Create env array by moving it by one to add a 0 on the argv array
*/
static int exec_not_inherit(int argc, char **args, struct s_shell *shell)
{
	size_t	i;
	char	*path;

	i = 0;
	path = shell->path;
	while (i < (size_t)argc && ft_strchr(args[i], '='))
	{
		if (ft_strncmp(args[i], "PATH=", 5) == 0)
			path = args[i];
		args[i - 1] = args[i];
		i++;
	}
	args[i - 1] = 0;
	return (exec_binary(path, argc - i, args + i, args - 1));
}

static char	*copy_env_and_get_path(char **env, int argc, char **args, struct s_shell *shell)
{
	char	*pos;
	char	*path;
	char	*cenv;
	size_t	i;
	size_t	j;
	size_t	k;

	path = shell->path;
	i = -1;
	j = shell->env_len;
	while (++i < (size_t)argc && (pos = ft_strchr(args[i], '=')))
	{
		if (ft_strncmp(args[i], "PATH=", 5) == 0)
			path = args[i];
		if (!(cenv = sh_getenv(shell, args[i], pos - args[i])))
			env[j++] = args[i];
		else
		{
			cenv -= pos - args[i] + 1;
			k = 0;
			while (k < shell->env_len)
				if (shell->env[k++] == cenv)
					env[k - 1] = args[i];
		}
	}
	return (path);
}

static int	exec_inherit(int argc, char **args, struct s_shell *shell)
{
	size_t	len;
	size_t	i;
	char	*pos;
	char	**env;
	char	*path;

	i = 0;
	len = shell->env_len;
	while (i < (size_t)argc && (pos = ft_strchr(args[i], '=')))
	{
		if (!sh_getenv(shell, args[i], pos - args[i]))
			len++;
		i++;
	}
	env = __builtin_alloca(sizeof(*env) * (len + 1));
	ft_memcpy(env, shell->env, sizeof(*env) * shell->env_len);
	env[len] = 0;
	path = copy_env_and_get_path(env, argc, args, shell);
	return (exec_binary(path, argc - i, args + i, env));
}

int			builtin_env(int argc, char **argv, struct s_shell *shell)
{
	size_t			i;
	bool			notInherit;
	const t_arg	args[] = {
		{ ARG_BOOLEAN, 'i', "", &notInherit, I_1 " " I_2 },
		{ ARG_END, 0, 0, 0, 0 }};
	int			ret;

	notInherit = false;
	if ((ret = parse_args(args, argc, argv)) < 0)
		return (args_usage(args, argv[0], USAGE, DESCRIPTION) || 1);
	argc -= ret;
	argv += ret;
	i = 0;
	if (argc)
		return ((notInherit ? exec_not_inherit
					: exec_inherit)(argc, argv, shell));
	else
		print_env(shell->env);
	return (0);
}
