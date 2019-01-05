/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 16:44:45 by dde-jesu          #+#    #+#             */
/*   Updated: 2018/12/20 16:44:47 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "shell.h"
#include "ft/str.h"
#include <unistd.h>

int	builtin_env(int argc, char **argv, t_shell *shell)
{
	size_t	i;

	(void)argc;
	(void)argv;
	i = 0;
	while (i < shell->env_len)
	{
		write(1, shell->env[i], ft_strlen(shell->env[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}
