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

// global error                      => exit
// error concerning only one command => skip this command
void	*malloc_(int size, t_data **d)
{
	void	*mem;

	// printf("  f malloc_(%d) 1\n", size);
	mem = NULL;
	mem = (void *)malloc(size * sizeof(char));
	// printf("  f malloc_(%d) 2\n", size);
	if (mem == NULL)
	{
		if ((*d)->curr_cmd == NULL)
			free_all_and_exit("malloc failure", -1, d); // code ?
		else
			(*d)->curr_cmd->err = "malloc failure";
	}
	// printf("  f malloc_(%d) 3\n", size);
	return (mem);
}

void	free_all_and_exit(char *msg, int exit_c, t_data **d)
{
	if (msg == NULL)
		printf("error %s\n", strerror(errno));
	else
		printf("error %s %s\n", msg, strerror(errno));
	// free all
	// free (paths)
	close((*d)->saved_stdout);
	exit(exit_c);
}
