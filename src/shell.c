/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/31 15:41:29 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/01/31 15:41:47 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "command.h"
#include "ft/mem.h"
#include "ft/str.h"

#include <limits.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>

static void	process_input(struct s_shell *shell)
{
	char	*env;
	size_t	i;

	shell->env = __builtin_alloca((shell->env_len + 1) * sizeof(char *));
	shell->env[shell->env_len] = 0;
	env = (char *)shell->buffer;
	i = 0;
	while (i < shell->env_len)
	{
		shell->env[i++] = env;
		env += ft_strlen(env) + 1;
	}
	shell->dirty_env = false;
	while (!shell->dirty_env)
		read_command(shell);
}

static bool	copy_env_to_buff(struct s_shell *shell, char *env[])
{
	size_t	len;

	while (*env)
	{
		len = ft_strlen(*env) + 1;
		if (shell->env_size + len > shell->buffer_size)
			return (false);
		if (ft_memcmp(*env, "PATH=", 5) == 0)
			shell->path = (char *)(shell->buffer + shell->env_size);
		ft_memcpy(shell->buffer + shell->env_size, *env, len);
		shell->env_size += len;
		shell->env_len++;
		env++;
	}
	return (true);
}

#define ENV_TOO_BIG "env is bigger than ARG_MAX, ARG_MAX is not well defined"

int			main(int ac, char *av[], char *env[])
{
	struct s_shell	shell;
	uint8_t			buffer[ARG_MAX];

	(void)ac;
	shell = (struct s_shell) {
		.buffer = buffer,
		.buffer_size = sizeof(buffer),
		.name = av[0],
		.path = NULL
	};
	if (!copy_env_to_buff(&shell, env))
	{
		write(1, ENV_TOO_BIG, sizeof(ENV_TOO_BIG) - 1);
		return (1);
	}
	while (true)
		process_input(&shell);
	return (0);
}
