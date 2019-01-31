/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 15:02:47 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/01/31 15:36:23 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "shell.h"

typedef int	(*t_builtin)(int argc, char **argv, struct s_shell *shell);

struct		s_builtin {
	char		*name;
	size_t		name_size;
	t_builtin	fn;
};

t_builtin	find_builtin(char *cmd, size_t cmd_size);
int			builtin_echo(int argc, char **argv, struct s_shell *shell);
int			builtin_cd(int argc, char **argv, struct s_shell *shell);
int			builtin_setenv(int argc, char **argv, struct s_shell *shell);
int			builtin_unsetenv(int argc, char **argv, struct s_shell *shell);
int			builtin_env(int argc, char **argv, struct s_shell *shell);
int			builtin_exit(int argc, char **argv, struct s_shell *shell);

#endif
