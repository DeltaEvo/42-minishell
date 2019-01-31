/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 16:45:10 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/01/31 15:39:51 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "ft/str.h"

#define USAGE "Usage: unsetenv [NAME]\n"
#define VAR_NOT_FOUND " is not set\n"

int	builtin_unsetenv(int argc, char **argv, struct s_shell *shell)
{
	if (argc == 2)
	{
		if (sh_unsetenv(shell, argv[1]))
			return (0);
		else
		{
			write(2, argv[1], ft_strlen(argv[1]));
			write(2, VAR_NOT_FOUND, sizeof(VAR_NOT_FOUND) - 1);
			return (1);
		}
	}
	else
	{
		write(2, USAGE, sizeof(USAGE) - 1);
		return (1);
	}
}
