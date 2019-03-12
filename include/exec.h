/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/31 15:36:38 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/03/11 15:43:43 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "shell.h"

int		exec_binary(char *path, char *argv[], char **env);
void	exec_buffer(struct s_shell *shell, size_t buffer_size);
char	*lookup_path(char *name, size_t name_len, char *path);
bool	check_binary(char *bin, bool print);

#endif
