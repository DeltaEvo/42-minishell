/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/31 13:31:32 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/01/31 15:15:30 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "shell.h"

char	*sh_getenv(struct s_shell *shell, char *name);
bool	sh_setenv(struct s_shell *shell, char *name, char *value);
bool	sh_unsetenv(struct s_shell *shell, char *name);

#endif
