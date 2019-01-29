#include "shell.h"
#include "exec.h"
#include "rl.h"
#include <stdlib.h>

#define PROMPT "minishell > "

void	read_command(struct s_shell *shell)
{
	struct s_rl_state	state;
	ssize_t				r;

	state = (struct s_rl_state) {
		.prompt = PROMPT,
		.prompt_size = sizeof(PROMPT) - 1,
		.buffer = shell->buffer + shell->env_size,
		.buffer_size = shell->buffer_size - shell->env_size
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
