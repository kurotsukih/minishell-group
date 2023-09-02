/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:55 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/02 17:40:56 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	mem = NULL;
	mem = (void *)malloc(size * sizeof(char));
	if (mem == NULL)
			free_all_and_exit("malloc failure", -1, d); // code ?
	return (mem);
}

void	free_all_and_exit(char *msg, int exit_c, t_data **d)
{
	if (msg == NULL)
		msg = "";
	printf("%s %s\n", msg, strerror(errno));
	// free (env)
	close((*d)->saved_stdin);
	close((*d)->saved_stdout);
	exit(exit_c);
}
