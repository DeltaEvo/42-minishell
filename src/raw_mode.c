#include "shell.h"
#include "raw_mode.h"

bool	start_raw_mode(struct s_shell *shell)
{
	struct termios	termios;

	if (tcgetattr(STDIN_FILENO, &termios) == -1)
		return (false);
	shell->original = termios;
	termios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	termios.c_oflag &= ~(OPOST);
	termios.c_cflag |= (CS8);
	termios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	return (tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios) == 0);
}

bool	end_raw_mode(struct s_shell *shell)
{
	return (tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->original) == 0);
}
