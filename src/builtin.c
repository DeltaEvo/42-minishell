/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 15:53:34 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/20 16:23:03 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "ft/str.h"
#include <stdio.h>

t_builtin	find_builtin(char *cmd)
{
	const struct s_builtin commands[] = {
		{ "echo", builtin_echo },
		{ "exit", builtin_exit },
		{ "cd", builtin_cd },
		{ "setenv", builtin_setenv },
		{ "unsetenv", builtin_unsetenv },
		{ "env", builtin_env }
	};
	size_t					i;

	i = 0;
	while (i < sizeof(commands)/sizeof(struct s_builtin))
		if (ft_strcmp(cmd, commands[i++].name) == 0)
			return (commands[i - 1].fn);
	return (NULL);
}
