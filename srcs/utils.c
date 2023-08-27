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
		free_all_and_exit("malloc problem", d);
	return (mem);
}

void	free_all_and_exit(char *msg, t_data **d)
{
	if (msg == NULL)
		printf("%s %s\n", msg, strerror(errno));
	if ((*d)->to_free != NULL)
		free((*d)->to_free); ///
	exit((*d)->exit_c);
}

void	free_all_and_go_to_next_cmd(char *msg, t_data **d)
{
	if (msg == NULL)
		printf("%s %s\n", msg, strerror(errno));
	if ((*d)->to_free != NULL)
		free((*d)->to_free); ///
}
