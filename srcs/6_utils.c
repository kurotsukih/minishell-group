/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:55 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/04 02:13:58 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

void	sig_handler_main(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1); // printf ?
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

void	*malloc_(int size, t_data **d)
{
	void	*mem;

	mem = NULL;
	mem = (void *)malloc(size * sizeof(char));
	if (mem == NULL)
			free_all_and_exit("malloc failure", -1, d); // code ?
	return (mem);
}

void free_(void *mem)
{
	if (mem != NULL)
		free(mem);
}

void	free_all_and_exit(char *msg, int exit_c, t_data **d)
{
	if (msg == NULL)
		msg = "";
	// printf("%s %s\n", msg, strerror(errno));
	// free (env)
	(void)d;
	exit(exit_c);
}

void	free_array(char **arr, int len)
{
	int	i;

	i = 0;
	while (i < len)
		free_(arr[i]);
	free_(arr);
}

void	print_cmd(char *msg, t_data **d)
{
	int		i;

	printf("%d args ", (*d)->nb_args);
	if ((*d)->arg != NULL)
	{
		i = -1;
		while (++i < (*d)->nb_args)
			printf("%s ", (*d)->arg[i]);
	}
	else
		printf("args = NULL");
	printf(" : %d ins ", (*d)->nb_ins);
	if ((*d)->in != NULL)
	{
		i = -1;
		while (++i < (*d)->nb_ins)
			printf("%d : ", (*d)->in[i]);
	}
	else
		printf("ins = NULL");
	printf(" : %d outs ", (*d)->nb_outs);
	if ((*d)->out != NULL)
	{
		i = -1;
		while (++i < (*d)->nb_outs)
			printf("%d : ", (*d)->out[i]);
	}
	else
		printf("outs = NULL");
	printf(" (%s)\n", msg);
}
