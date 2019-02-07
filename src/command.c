/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/31 15:40:25 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/02/07 17:06:01 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "exec.h"
#include "builtin.h"
#include "expand.h"
#include "rl.h"
#include "ft/mem.h"
#include "ft/str.h"
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
	while ((space = ft_memchr(buffer, ' ', state->len
					- (buffer - state->buffer))))
	{
		if (space == state->buffer || space[-1] == '\0')
		{
			ft_memmove(space, space + 1, state->buffer + state->len - (space + 1));
			state->len -= 1;
			state->index -= 1;
			buffer = space ;
		}
		else
		{
			*space = '\0';
			buffer = space + 1;
		}
	}
}

#include <dirent.h>
#include <stdio.h>

void complete_command(struct s_rl_state *state, char *cmd, size_t size)
{
	struct s_shell	*shell;
	char			*shell_path;
	char			path[PATH_MAX + 1];
	char			*end;
	size_t			path_len;
	DIR				*dir;
	struct dirent	*entry;
	char			suggestion[sizeof(entry->d_name)];
	size_t			suggestion_len;

	suggestion_len = sizeof(suggestion) + 1;
	shell = state->user_data;
	if (ft_memchr(cmd, '/', size))
	{
	}
	if (shell->path)
	{
		shell_path = shell->path + 5;
		while (1)
		{
			end = ft_strchr(shell_path, ':');
			if (!end)
				end = shell_path + ft_strlen(shell_path);
			path_len = end - shell_path;
			if (path_len + 1 > sizeof (path))
			{
				shell_path = end + 1;
				continue ;
			}
			ft_memcpy(path, shell_path, path_len);
			path[path_len] = 0;
			shell_path = end + 1;
			if (!(dir = opendir(path)))
				continue ;
			while ((entry = readdir(dir)))
			{
				if (ft_strncmp(entry->d_name, cmd, size) == 0 && entry->d_namlen < sizeof(suggestion))
				{
					if (entry->d_namlen <= suggestion_len)
					{
						ft_memcpy(suggestion, entry->d_name, entry->d_namlen);
						suggestion_len = entry->d_namlen;
					}
				}
			}
			closedir(dir);
			if (*end == 0)
				break ;
		}
		if (suggestion_len > sizeof(suggestion))
			return ;
		ft_memcpy(state->buffer, suggestion, suggestion_len);
		state->len += suggestion_len - size;
		state->index += suggestion_len - size;
	}
}

static void on_tab(struct s_rl_state *state)
{
	char	*word;
	size_t	len;

	word = state->buffer + state->y_offset + state->x_pos;
	len = 0;
	while (word > state->buffer && word[-1] != '\0')
	{
		len++;
		word--;
	}
	if (len)
	{
		if (word == state->buffer)
			complete_command(state, word, len);
	}
}


#define PROMPT ("\xF0\x9F\xA6\x84  > ")
#define PROMPT_LEN 5

#include <stdio.h>

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
			[RL_NONE] = on_text,
			[RL_TAB_CTRL_I] = on_tab
		},
		.echo_hook = echo
	};
	r = readline(&state);
	write(1, "\n", 1);
	if (r > 0)
	{
		state.buffer[r] = '\0';
		r = perform_expansion(shell, r + 1);
		if (r)
			exec_buffer(shell, r);
	}
	else if (r < 0)
		exit(0);
}
