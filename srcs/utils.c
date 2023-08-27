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

void	exit_(t_data **d)
{
	(void)d;
}
// {
// 	if (msg == NULL)
// 		printf("%s\n", strerror(errno));
// 	else if (msg != NULL && msg_param == NULL)
// 		printf("bash: %s\n", msg);
// 	else if (msg != NULL && msg_param != NULL)
// 		printf(msg, msg_param);
// 	if (lst_to_clear != NULL)
// 	// ft_lstclear(lst_to_clear);
// 	if (str_to_free != NULL)
// 		free(*str_to_free);
// 	if (exit_code != -1)
// 		exit(exit_code);
// }
