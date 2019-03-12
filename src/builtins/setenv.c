/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 16:45:03 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/12 15:01:52 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "ft/str.h"

#define USAGE "Usage: setenv [NAME] [VALUE]\n"
#define ENV2BIG "Cannot add new env variable, env will be bigger than ARG_MAX\n"
#define INVALIDNAME "Invalid env variable name\n"

int	builtin_setenv(int argc, char **argv, struct s_shell *shell)
{
	if (argc == 3)
	{
		if (ft_strchr(argv[1], '='))
		{
			write(2, INVALIDNAME, sizeof(INVALIDNAME) - 1);
			return (1);
		}
		if (sh_setenv(shell, argv[1], argv[2]))
			return (0);
		else
		{
			write(2, ENV2BIG, sizeof(ENV2BIG) - 1);
			return (1);
		}
	}
	else
	{
		write(2, USAGE, sizeof(USAGE) - 1);
		return (1);
	}
}
