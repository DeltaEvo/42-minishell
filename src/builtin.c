/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 15:53:34 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/01/31 15:38:34 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "ft/mem.h"

t_builtin	find_builtin(char *cmd, size_t cmd_size)
{
	const struct s_builtin	commands[] = {
		{ "echo", 4, builtin_echo },
		{ "exit", 4, builtin_exit },
		{ "cd", 2, builtin_cd },
		{ "setenv", 6, builtin_setenv },
		{ "unsetenv", 8, builtin_unsetenv },
		{ "env", 3, builtin_env }
	};
	size_t					i;

	i = 0;
	while (i < sizeof(commands) / sizeof(struct s_builtin))
	{
		if (commands[i].name_size == cmd_size
			&& ft_memcmp(cmd, commands[i].name, cmd_size) == 0)
			return (commands[i].fn);
		i++;
	}
	return (NULL);
}
