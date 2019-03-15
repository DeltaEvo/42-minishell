/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 16:44:27 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/15 16:05:38 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "env.h"
#include "ft/args.h"
#include "ft/io.h"
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

#define ARG2BIG "Too many arguments for cd command\n"
#define NO_HOME "$HOME not set\n"
#define NO_OLDPWD "$OLDPWD not set\n"

int	go_to(char *directory)
{
	struct stat	stats;

	if (stat(directory, &stats) == -1)
	{
		ft_putf_fd(2, "No such file or directory: %s\n", directory);
		return (1);
	}
	if (!S_ISDIR(stats.st_mode))
	{
		ft_putf_fd(2, "Not a directory: %s\n", directory);
		return (1);
	}
	if (!(stats.st_mode & S_IXUSR))
	{
		ft_putf_fd(2, "Permission denied: %s\n", directory);
		return (1);
	}
	return (chdir(directory));
}

int	builtin_cd(int argc, char **argv, struct s_shell *shell)
{
	const t_arg	args[] = {
		{ ARG_END, 0, 0, 0, 0 }};
	int			ret;
	char		*directory;
	char		cwd[PATH_MAX + 1];

	if ((ret = parse_args(args, argc, argv)) < 0)
		return (args_usage(args, argv[0], "[directory]", "") || 1);
	argc -= ret;
	argv += ret;
	if (argc == 0 && !(directory = sh_getenv(shell, "HOME", 4)))
		return (write(2, NO_HOME, sizeof(NO_HOME) - 1) & 0 + 1);
	if (argc == 1)
	{
		directory = argv[0];
		if (directory[0] == '-' && directory[1] == '\0'
			&& !(directory = sh_getenv(shell, "OLDPWD", 6)))
			return (write(2, NO_OLDPWD, sizeof(NO_OLDPWD) - 1) & 0 + 1);
	}
	if (argc > 1)
		return (write(2, ARG2BIG, sizeof(ARG2BIG) - 1) & 0 + 1);
	getcwd(cwd, sizeof(cwd));
	if (!(ret = go_to(directory)))
		sh_setenv(shell, "OLDPWD", cwd);
	return (ret);
}
