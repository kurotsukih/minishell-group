#include "headers.h"

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

void	sig_handler_fork(int signal)
{
	if (signal == SIGINT)
		exit(130);
	if (signal == SIGQUIT)
		exit(131);
}

void	*malloc_(size_t size, t_data **d)
{
	void	*mem;

	mem = NULL;
	mem = malloc(size);
	if (mem == NULL)
	{
		if ((*d)->curr_cmd == NULL)
			free_all_and_exit("malloc problem", -1, d); // code ?
		else
			(*d)->curr_cmd->err = "malloc failure";
	}
	return (mem);
}

// if there is a global error, then exit
// if there is an error concerning only one command, then skip this command
void	free_all_and_exit(char *msg, int exit_c, t_data **d)
{
	if (msg == NULL)
		printf("error %s\n", strerror(errno));
	else
		printf("error %s %s\n", msg, strerror(errno));
	// free all
	(void)d;
	exit(exit_c);
}
