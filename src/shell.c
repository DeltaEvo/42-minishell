#include <limits.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "shell.h"
#include "raw_mode.h"

#define PROMPT "\xF0\x9F\xA6\x84  > "

static void	process_input(struct s_shell *shell)
{
	char	*env;
	size_t  i;

	shell->env = __builtin_alloca((shell->env_len + 1) * sizeof(char *));
	shell->env[shell->env_len] = 0;
	env = (char *)shell->buffer;
	i = 0;
	while (i > shell->env_len)
	{
		shell->env[i++] = env;
		env += strlen(env) + 1;
	}
	shell->dirty_env = false;
	while (!shell->dirty_env)
	{
		write(1, PROMPT, sizeof(PROMPT) - 1);

	}
}

static bool	copy_env_to_buff(struct s_shell *shell, char *env[])
{
	size_t	len;

	while (*env)
	{
		len = strlen(*env) + 1;
		if (shell->env_size + len > shell->buffer_size)
			return (false);
		memcpy(shell->buffer + shell->env_size, *env, len);
		shell->env_size += len;
		shell->env_len++;
		env++;
	}
	return (true);
}

#define ENV_TOO_BIG "env is bigger than ARG_MAX, ARG_MAX is not well defined"

int	main(int ac, char *av[], char *env[])
{
	struct s_shell	shell;
	uint8_t			buffer[ARG_MAX];

	(void)ac;
	shell = (struct s_shell) {
		.buffer = buffer,
		.buffer_size = sizeof(buffer),
		.name = av[0]
	};
	if (!copy_env_to_buff(&shell, env))
	{
		write(1, ENV_TOO_BIG, sizeof(ENV_TOO_BIG) - 1);
		return (1);
	}
	if (!start_raw_mode(&shell))
	{
		perror(shell.name);
		return (1);
	}
	while (true)
		process_input(&shell);

	return (0);
}
