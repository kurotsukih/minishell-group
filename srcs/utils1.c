#include "minishell.h"

void	sig_handler_main(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = 1;
	}
}

void	sig_handler_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_redisplay();
		close(STDIN_FILENO);
		g_signal = 1;
	}
}

void	sig_handler_fork(int num)
{
	if (num == SIGINT)
		exit(130);
	if (num == SIGQUIT)
		exit(131);
}


