/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/22 10:06:40 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/12 13:17:36 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syntax.h"
#include "shell.h"
#include "exec.h"
#include "ft/mem.h"
#include "builtin.h"

#include <limits.h>

#define CSI "\33["

static bool		is_executable(struct s_shell *shell, char *cmd, size_t cmd_size)
{
	char	bin[PATH_MAX + 1];

	if (find_builtin(cmd, cmd_size))
		return (true);
	if (ft_memchr(cmd, '/', cmd_size))
	{
		if (cmd_size + 1 > sizeof(bin))
			return (false);
		ft_memcpy(bin, cmd, cmd_size);
		bin[cmd_size] = 0;
		return (check_binary(bin, false));
	}
	if (shell->path)
	{
		return (check_binary(lookup_path(cmd, cmd_size, shell->path + 5),
					false));
	}
	return (false);
}

void			syntax_echo(struct s_rl_state *state, char *part, size_t size)
{
	struct s_shell	*shell;
	char			*space;
	size_t			len;

	shell = state->user_data;
	while (true)
	{
		if ((space = ft_memchr(part, '\0', size)))
			len = space - part;
		else
			len = size;
		if (part == state->buffer)
			write(STDOUT_FILENO,
				is_executable(shell, part, len) ? CSI "37m" : CSI "31m", 5);
		else
			write(STDOUT_FILENO, CSI "36m", 5);
		write(STDOUT_FILENO, part, len);
		if (!space)
			break ;
		write(STDOUT_FILENO, " ", 1);
		size -= len + 1;
		part = space + 1;
	}
	write(STDOUT_FILENO, CSI "0m", 4);
}
