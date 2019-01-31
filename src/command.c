/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/31 15:40:25 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/01/31 15:42:34 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "exec.h"
#include "builtin.h"
#include "rl.h"
#include "ft/mem.h"
#include <stdlib.h>
#include <limits.h>

static bool	is_executable(struct s_shell *shell, char *cmd, size_t cmd_size)
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
		return (access(bin, X_OK) == 0);
	}
	if (shell->path)
		return (lookup_path(cmd, cmd_size, shell->path + 5, bin, sizeof(bin)));
	return (false);
}

#define CSI "\33["

static void	echo(struct s_rl_state *state, char *part, size_t size)
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

static void	on_text(struct s_rl_state *state)
{
	char	*buffer;
	char	*space;

	buffer = state->buffer;
	while ((space = ft_memchr(buffer, ' ', state->buffer_size
					- (buffer - state->buffer))))
	{
		*space = '\0';
		buffer = space + 1;
	}
}

#define PROMPT ("\xF0\x9F\xA6\x84  > ")
#define PROMPT_LEN 5

void		read_command(struct s_shell *shell)
{
	struct s_rl_state	state;
	ssize_t				r;

	state = (struct s_rl_state) {
		.user_data = shell,
		.prompt = PROMPT,
		.prompt_size = sizeof(PROMPT) - 1,
		.prompt_len = PROMPT_LEN,
		.buffer = (char *)shell->buffer + shell->env_size,
		.buffer_size = shell->buffer_size - shell->env_size - 1,
		.hooks = {
			[RL_NONE] = on_text
		},
		.echo_hook = echo
	};
	r = readline(&state);
	write(1, "\n", 1);
	if (r > 0)
	{
		state.buffer[r] = '\0';
		exec_buffer(shell, r + 1);
	}
	else if (r < 0)
		exit(0);
}
