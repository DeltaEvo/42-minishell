/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 10:11:15 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/02/22 17:12:52 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "completion.h"
#include "shell.h"
#include "str.h"
#include "ft/str.h"
#include "ft/mem.h"
#include <dirent.h>
#include <limits.h>

#define SUGGESTION_SIZE (sizeof(((struct dirent *)0)->d_name))

static void		search_dir(struct s_str dir, struct s_str cmd, char *suggestion,
		size_t *suggestion_len)
{
	DIR				*d;
	struct dirent	*entry;
	char			path[PATH_MAX + 1];

	if (dir.len > PATH_MAX)
		return ;
	ft_memcpy(path, dir.value, dir.len);
	path[dir.len] = 0;
	if (!(d = opendir(path)))
		return ;
	while ((entry = readdir(d)))
	{
		if (entry->d_name[0] != '.'
			&& ft_strncmp(entry->d_name, cmd.value, cmd.len) == 0
			&& entry->d_namlen < SUGGESTION_SIZE)
		{
			if (entry->d_namlen <= *suggestion_len)
			{
				ft_memcpy(suggestion, entry->d_name, entry->d_namlen);
				*suggestion_len = entry->d_namlen;
			}
		}
	}
	closedir(d);
}

static size_t	search_in_path(struct s_shell *shell, char *cmd, size_t size,
		char *suggestion)
{
	size_t	suggestion_len;
	char	*path;
	char	*end;
	size_t	path_len;

	suggestion_len = SUGGESTION_SIZE + 1;
	path = shell->path + 5;
	while (1)
	{
		end = ft_strchr(path, ':');
		if (!end)
			end = path + ft_strlen(path);
		path_len = end - path;
		search_dir((struct s_str){ path, path_len },
				(struct s_str){ cmd, size }, suggestion, &suggestion_len);
		path = end + 1;
		if (*end == 0)
			break ;
	}
	return (suggestion_len);
}

void			complete_command(struct s_rl_state *state, char *cmd,
		size_t size)
{
	struct s_shell	*shell;
	char			suggestion[SUGGESTION_SIZE];
	size_t			suggestion_len;
	size_t			suggestion_start;
	char			*slash;

	suggestion_len = sizeof(suggestion) + 1;
	suggestion_start = 0;
	shell = state->user_data;
	if ((slash = ft_memrchr(cmd, '/', size)))
	{
		suggestion_start = slash + 1 - cmd;
		search_dir((struct s_str){ cmd, suggestion_start },
				(struct s_str){ slash + 1, size - (suggestion_start) },
				suggestion, &suggestion_len);
	}
	else if (shell->path)
		suggestion_len = search_in_path(shell, cmd, size, suggestion);
	if (suggestion_len > sizeof(suggestion))
		return ;
	ft_memcpy(state->buffer + suggestion_start, suggestion, suggestion_len);
	state->len += suggestion_len - (size - suggestion_start);
	state->index += suggestion_len - (size - suggestion_start);
}
