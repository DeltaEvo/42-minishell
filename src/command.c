#include "shell.h"
#include "exec.h"
#include "rl.h"
#include <stdlib.h>
#include <limits.h>

static void echo(struct s_rl_state *state, char *part, size_t size)
{
	struct s_shell	*shell;
	char	*space;
	bool	first;
	char	bin[PATH_MAX + 1];

	shell = state->user_data;
	first = true;
	while ((space = memchr(part, '\0', size)))
	{
		if (first)
		{
			if (lookup_path(part, space - part, shell->path + 5, bin, sizeof(bin)))
				write(STDOUT_FILENO, "\33[37m", 5);
			else
				write(STDOUT_FILENO, "\33[31m", 5);
			first = false;
		} else
			write(STDOUT_FILENO, "\33[36m", 5);
		write(STDOUT_FILENO, part, space - part);
		write(STDOUT_FILENO, " ", 1);
		size -= space - part + 1;
		part = space + 1;
	}
	if (first)
	{
		if (lookup_path(part, size, shell->path + 5, bin, sizeof(bin)))
			write(STDOUT_FILENO, "\33[37m", 5);
		else
			write(STDOUT_FILENO, "\33[31m", 5);
	} else
		write(STDOUT_FILENO, "\33[36m", 5);
	write(STDOUT_FILENO, part, size);
	write(STDOUT_FILENO, "\33[0m", 4);
}

static void	on_text(struct s_rl_state *state)
{
	char	*buffer;
	char	*space;

	buffer = state->buffer;
	while ((space = memchr(buffer, ' ', state->buffer_size - (buffer - state->buffer))))
	{
		*space = '\0';
		buffer = space + 1;
	}
}

#define PROMPT ("\xF0\x9F\xA6\x84  > ")
#define PROMPT_LEN 6

void	read_command(struct s_shell *shell)
{
	struct s_rl_state	state;
	ssize_t				r;

	state = (struct s_rl_state) {
		.user_data = shell,
		.prompt = PROMPT,
		.prompt_size = sizeof(PROMPT) - 1,
		.prompt_len = PROMPT_LEN,
		.buffer = shell->buffer + shell->env_size,
		.buffer_size = shell->buffer_size - shell->env_size,
		.hooks = {
			[RL_NONE] = on_text
		},
		.echo_hook = echo
	};
	r = readline(&state);
	write(1, "\n", 1);
	if (r > 0)
	{
		state.buffer[r] = '\0';
		exec_buffer(shell, r + 1);
	}
	else
		exit(0);
}
