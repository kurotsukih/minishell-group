/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   7_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:55 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/12 13:04:37 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

int	init_d(t_data **d, char **env) // **d ?
{
	*d = (t_data *)malloc_(sizeof(t_data), d);
	(*d)->env = arr_to_lst(env, d);
	(*d)->redir = "";
	(*d)->token = "";
	(*d)->saved_stdin = dup(STDIN_FILENO); // if fail s ?
	(*d)->saved_stdout = dup(STDOUT_FILENO);
	return (OK);
}

void	*malloc_(int size, t_data **d)
{
	void	*mem;

	mem = NULL;
	mem = (void *)malloc(size * sizeof(char));
	if (mem == NULL)
		free_all_and_exit("malloc failure", -1, d);
	return (mem);
}

void free_(void *mem)
{
	if (mem != NULL)
		free(mem);
}

void	free_2_array(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i] != NULL)
		free_(arr[i]);
	free_(arr);
}

int		err_cmd(char *msg, int exit_c, t_data **d)
{
	if (msg == NULL)
		msg = "";
	if (ft_strlen(msg) > 0)
	write_fd_with_n(2, msg);
		(*d)->exit_c = exit_c;
	return (FAILURE);
}

void	free_all_and_exit(char *msg, int exit_c, t_data **d)
{
	err_cmd(msg, exit_c, d);
	//fre e list (*d)->args;
	//fre e list (*d)->env;
	close((*d)->saved_stdin);
	close((*d)->saved_stdout);
	// free(d);
	exit(exit_c);
}

void	print_d(char *msg, t_data **d)
{
	t_lst	*cur;

	printf("%s : ", msg);
	if ((*d)->args == NULL)
		printf("no args");
	else
	{
		printf("%d args ", len_lst((*d)->args));
		cur = *((*d)->args);
		while (cur != NULL)
		{
			printf("[%s] ", cur->val);
			cur= cur->nxt;
		}
	}
	printf(" : %d : %d ", (*d)->fd_in, (*d)->fd_out);
	printf(" (tok=[%s] redir=[%s] i=%d sav.in=%d sav.out=%d)\n", (*d)->token, (*d)->redir, (*d)->i, (*d)->saved_stdin, (*d)->saved_stdout);
}

int	write_fd(int fd, char *s){
	return (write(fd, s, ft_strlen(s)));
}

int	write_fd_with_n(int fd, char *s)
{
	return (write(fd, s, ft_strlen(s)) + write(fd, "\n", 1));
}

void	sig_handler_main(int signal) // ???
{
	if (signal == SIGINT)
	{
		write_fd(1, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = 1;
	}
}

void	sig_handler_heredoc(int signal) // ???
{
	if (signal == SIGINT)
	{
		write_fd(1, "\n");
		rl_replace_line("", 0);
		rl_redisplay();
		close(STDIN);
		g_signal = 1;
	}
}

void	sig_handler_fork(int signal) // ???
{
	if (signal == SIGINT)
		exit(130);
	if (signal == SIGQUIT)
		exit(131);
}