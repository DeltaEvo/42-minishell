/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/31 15:36:52 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/01/31 15:37:52 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <stdint.h>
# include <unistd.h>
# include <termios.h>
# include <stdbool.h>

/*
** buffer:
** ------------------------------------------------
** | env        | arguments                       |
** ------------------------------------------------
** env_size          buffer_size - env_size
**
** buffer: main buffer that will be passed to execve
** buffer_size: sizeof(buffer)
** env: list of pointer to the beginning of env variables stored in buffer
** env_size: size of all env variables in buffer
** env_len: number of env variables
** dirty_env: true if env is dirty it need to be recreated
** path: PATH env variable
*/

struct	s_shell {
	char			*name;
	uint8_t			*buffer;
	size_t			buffer_size;
	char			**env;
	size_t			env_size;
	size_t			env_len;
	bool			dirty_env;
	char			*path;
};
#endif
