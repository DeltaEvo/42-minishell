/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 10:11:15 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/02/15 10:37:29 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "completion.h"
#include "shell.h"
#include "ft/str.h"
#include "ft/mem.h"
#include <dirent.h>
#include <limits.h>

void	complete_command(struct s_rl_state *state, char *cmd, size_t size)
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
