/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_parse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:47 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/04 02:12:29 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

char	*redir_(char *s)
{
	if (s[0]== '\0')
		return (NULL);
	if (s[0] == '>' && s[1] != '>')
		return (">");
	if (s[0] == '<' && s[1] != '<')
		return ("<");
	if (s[0] == '>' && s[1] == '>')
		return (">>");
	if (s[0] == '<' && s[1] == '<')
		return ("<<");
	else
		return (NULL);
}

int	len_alphanum(char *s)
{
	int	i;

	if ((s[0] < 'a' || s[0] > 'z') && (s[0] < 'A' && s[0] > 'Z'))
		return (0);
	i = -1;
	while ((s[++i] >= '0' && s[i] < '9') || (s[i] >= 'a' && s[i] < 'z') || (s[i] >= 'A' && s[i] < 'Z') || s[i] == '_')
		;
	return (i);
}

static int	is_digit(char c)
{
	return (c >= '0' && c < '9');
}

static int	is_letter(char c)
{
	return ((c >= 'a' && c < 'z') || (c >= 'A' && c < 'Z'));
}

char	*alphanum_(char *s, t_data **d)
{
	int		i;
	char	*alphanum = NULL; //

	if (s == NULL || (!is_digit(s[0]) && !is_letter(s[0])))
		return (NULL);
	alphanum = (char *)malloc_(len_alphanum(s) + 1, d);
	i = -1;
	while (is_digit(s[++i]) || is_letter(s[i]) || s[i] == '_')
		alphanum[i] = s[i];
	alphanum[i] = '\0';
	return (alphanum);
}

int	nb_spaces(char *s)
{
	int	i;

	i = 0;
	while (s[i] == ' ')
		i++;
	return (i);
}

void calc_nb_args_ins_outs(char *s, int len, t_data **d)
{
	int		i;
	char	*redir;

	mod_(REINIT_QUOTES);
	i = -1;
	while (++i < len)
		if (mod_(s[i]) == QUOTES0)
		{
			i += nb_spaces(&s[i]);
			redir = redir_(&s[i]);
			if (ft_strcmp(redir, "<") == 0 || ft_strcmp(redir, "<<") == 0)
				((*d)->nb_ins)++;
			else if (ft_strcmp(redir, ">") == 0 || ft_strcmp(redir, ">>") == 0)
				((*d)->nb_outs)++;
			else
				((*d)->nb_args)++;
			i += ft_strlen(redir);
			i += nb_spaces(&s[i]);
			i += len_alphanum(&s[i]);
		}
	if ((*d)->nb_ins == 0)
		(*d)->nb_ins = 1;
	if ((*d)->nb_outs == 0)
		(*d)->nb_outs = 1;
	(*d)->arg = (char **)malloc_(((*d)->nb_args + 1) * sizeof(char *), d);
	(*d)->in = (int *)malloc_((*d)->nb_ins * sizeof(int), d);
	(*d)->out = (int *)malloc_((*d)->nb_outs * sizeof(int), d);
}

void	heredoc_to_file(char *delim, int fd)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline(">");
		if (line == NULL || strcmp_(line, delim) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free_(line);
	}
}

void print_cmd(char *msg, t_data **d)
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
	printf(" : %s\n", msg);
}

// void rmv_cmd(t_cmd *cmd, t_data **d)
// {
// 	int		i;
// 	t_cmd	*to_free;

// 	if (cmd == NULL)
// 		return ;
// 	i = -1;
// 	while(++i < cmd->nb_args)
// 	{
// 		free(cmd->arg[i]);
// 		cmd->arg[i] = NULL;
// 	}
// 	free(cmd->arg);
// 	cmd->arg = NULL;
// 	to_free = cmd;
// 	if (cmd->nxt != NULL)
// 		cmd->nxt->prv = cmd->prv;
// 	if (cmd->prv == NULL)
// 		*((*d)->cmds) = cmd->nxt;
// 	else
// 		cmd->prv->nxt = cmd->nxt;
// 	free(to_free); // &to_free ?
// 	close(cmd->fd_in);
// 	close(cmd->fd_out); // close pipe
// 	(*d)->curr_cmd = NULL;
// 	to_free = NULL;
// }

// void	rmv_cmds(t_data **d)
// {
// 	t_cmd	*cmd;

// 	cmd = *((*d)->cmds);
// 	while(cmd != NULL)
// 	{
// 		rmv_cmd(cmd, d);
// 		cmd = cmd->nxt;
// 	}
// }

// int	nb_args_(char *s0, int len, t_data **d)
// {
// 	int		nb_args;
// 	int		i;
// 	char	*s;

// 	s = strdup_and_erase_redirs(s0, d);
// 	mod_(REINIT_QUOTES);
// 	nb_args = 0;
// 	i = -1;
// 	while (++i < len)
// 		if (mod_(s[i]) == QUOTES0 && s[i] != ' ' && (s[i + 1] == ' ' || s[i + 1] == '\0' || s[i + 1] == '\'' || s[i + 1] == '\"' || i == len - 1))
// 				nb_args++;
// 	free(s);
// 	return (nb_args);
// }

