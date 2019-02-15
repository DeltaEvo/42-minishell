/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 16:44:35 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/02/15 10:35:00 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft/str.h"
#include <unistd.h>
#include <stdbool.h>

int	builtin_echo(int argc, char **argv)
{
	int		i;
	bool	no_newline;

	no_newline = argc > 1 && ft_strcmp(argv[1], "-n") == 0;
	i = 1 + no_newline;
	while (i < argc)
	{
		write(1, argv[i], ft_strlen(argv[i]));
		if (i != argc - 1)
			write(1, " ", 1);
		i++;
	}
	if (!no_newline)
		write(1, "\n", 1);
	return (0);
}
