/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 15:02:47 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/01/05 14:08:15 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "shell.h"

typedef int (*t_builtin)(int argc, char **argv, struct s_shell *shell);

struct		s_builtin {
	char		*name;
	t_builtin	fn;
};

t_builtin	find_builtin(char *cmd);
int			builtin_echo(int argc, char **argv, struct s_shell *shell);
int			builtin_cd(int argc, char **argv, struct s_shell *shell);
int			builtin_setenv(int argc, char **argv, struct s_shell *shell);
int			builtin_unsetenv(int argc, char **argv, struct s_shell *shell);
int			builtin_env(int argc, char **argv, struct s_shell *shell);
int			builtin_exit(int argc, char **argv, struct s_shell *shell);

#endif
