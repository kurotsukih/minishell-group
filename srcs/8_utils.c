/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:55 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/05 20:41:32 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

// void	sig_handler_main(int signal)
// {
// 	if (signal == SIGINT)
// 	{
// 		write_fd(1, "\n");
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 		g_signal = 1;
// 	}
// }

// void	sig_handler_heredoc(int signal)
// {
// 	if (signal == SIGINT)
// 	{
// 		write_fd(1, "\n");
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 		clos e(STDIN_FILENO);
// 		g_signal = 1;
// 	}
// }

// void	sig_handler_fork(int signal)
// {
// 	if (signal == SIGINT)
// 		exit(130);
// 	if (signal == SIGQUIT)
// 		exit(131);
// }

void	init_d(t_data ***d, char **env)
{
	*d = (t_data **)malloc_(sizeof(t_data *), NULL);
	**d = (t_data *)malloc_(sizeof(t_data), *d);
	(**d)->saved_stdin = dup(STDIN_FILENO);
	(**d)->saved_stdout = dup(STDOUT_FILENO);
	(**d)->env = arr_to_lst(env, *d);
	// signal(SIGQUIT, SIG_IGN);
	// signal(SIGINT, &sig_handler_main);
}

void	init_cmd(t_data **d)
{
	(*d)->redir = NULL;
	(*d)->token = NULL;
	dell_all_from_lst((*d)->args);
	dell_all_from_lst((*d)->outs);
	//// (*d)->in = ;
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

void	free_all_and_exit(char *msg, int exit_c, t_data **d)
{
	err_cmd(msg, exit_c, d);
	//fre e list (*d)->env;
	close((*d)->saved_stdin); // ?
	close((*d)->saved_stdout); // ?
	exit(exit_c);
}

int		err_cmd(char *msg, int exit_c, t_data **d)
{
	if (msg == NULL)
		msg = "";
	write_fd_with_n(2, msg);
	//free_2_array((*d)->arg, (*d)->nb_args); error because string without malloc ?
	free_((*d)->outs);
	(*d)->exit_c = exit_c;
	return (FAILURE);
}

void	free_2_array(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i] != NULL)
		free_(arr[i]);
	free_(arr);
}

void	print_d(char *msg, t_data **d)
{
	t_lst	*cur;

	printf("d %s : ", msg);
	if ((*d)->args == NULL)
		printf("no args\n");
	else
	{
		// printf("D (%s) :\nargs: ", msg);
		cur = *((*d)->args);
		// printf("D (%s) :\nargs: ", msg);
		while (cur != NULL)
		{
			printf("%s ", (char *)(cur->val));
			cur= cur->nxt;
		}
	}
	printf(" : %d : ", (*d)->in);
	if ((*d)->outs == NULL)
		printf("no outs");
	else
	{
		cur = *((*d)->outs);
		while (cur != NULL)
		{
			printf("%d ", *((int *)(cur->val)));
			cur= cur->nxt;
		}
	}
	printf("\n  token = [%s], redir = [%s], i = %d\n", (*d)->token, (*d)->redir, (*d)->i);
}

int	write_fd(int fd, char *s){
	return (write(fd, (char *)s, ft_strlen((char *)s)));
}

int	write_fd_with_n(int fd, char *s)
{
	return (write(fd, (char *)s, ft_strlen((char *)s)) + write(fd, "\n", 1));
}