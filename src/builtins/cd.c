/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 16:44:27 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/02/12 15:25:35 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "env.h"
#include "ft/args.h"
#include <unistd.h>
#include <limits.h>

#define ARG2BIG "Too many arguments for cd command\n"
#define NO_HOME "$HOME not set\n"
#define NO_OLDPWD "$OLDPWD not set\n"

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
	ret = chdir(directory);
	sh_setenv(shell, "OLDPWD", cwd);
	return (ret);
}
