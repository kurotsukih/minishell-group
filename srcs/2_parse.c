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

static char	*redir_(char *s)
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

static int	len_alphanum(char *s)
{
	int	i;

	if ((s[0] < 'a' || s[0] > 'z') && (s[0] < 'A' && s[0] > 'Z'))
		return (0);
	i = -1;
	while ((s[++i] >= '0' && s[i] < '9') || (s[i] >= 'a' && s[i] < 'z') || (s[i] >= 'A' && s[i] < 'Z') || s[i] == '_')
		;
	printf("len alphanum %s retuen %d\n", s, i);
	return (i);
}

char	*alphanum_(char *s, t_data **d)
{
	int		i;
	char	*alphanum;

	if ((s[0] < 'a' || s[0] > 'z') && (s[0] < 'A' && s[0] > 'Z'))
		return (NULL);
	i = -1;
	while ((s[++i] >= '0' && s[i] < '9') \
		|| (s[i] >= 'a' && s[i] < 'z') || (s[i] >= 'A' && s[i] < 'Z') || s[i] == '_')
		;
	alphanum = (char *)malloc_(i + 1, d);
	i = -1;
	while ((s[++i] >= '0' && s[i] < '9') \
		|| (s[i] >= 'a' && s[i] < 'z') || (s[i] >= 'A' && s[i] < 'Z') || s[i] == '_')
		alphanum[i] = s[i];
	alphanum[i] = '\0';
	return (alphanum);
}

static int	nb_spaces(char *s)
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

	printf("calc_nb_args_ins_outs %s\n", s);
	mod_(REINIT_QUOTES);
	i = -1;
	while (++i < len)
		if (mod_(s[i]) == QUOTES0)
		{
			printf("1) i=%d\n", i);
			i += nb_spaces(&s[i]);
			redir = redir_(&s[i]);
			if (ft_strcmp(redir, "<") == 0 || ft_strcmp(redir, "<<") == 0)
				((*d)->nb_ins)++;
			else if (ft_strcmp(redir, ">") == 0 || ft_strcmp(redir, ">>") == 0)
				((*d)->nb_outs)++;
			else
			{
				((*d)->nb_args)++;
				printf("2) i=%d, nb_args = %d\n", i, (*d)->nb_args);
			}
			printf("3) strlen = %d\n", (int)ft_strlen(redir));
			i += ft_strlen(redir);
			printf("4) nb spaces = %d\n", nb_spaces(&s[i]));
			i += nb_spaces(&s[i]);
			printf("5) i = %d, len_alphanum = %d\n", i, len_alphanum(&s[i]));
			i += len_alphanum(&s[i]);
			printf("6) i=%d\n", i);
		}
	(*d)->arg = (char **)malloc_(((*d)->nb_args + 1) * sizeof(char *), d);
	(*d)->in = (int *)malloc_((*d)->nb_ins * sizeof(int), d);
	(*d)->out = (int *)malloc_((*d)->nb_outs * sizeof(int), d);
}

static void	heredoc_to_file(char *delim, int fd)
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
		free(line);
	}
}


int	parse(char *s, int len, t_data **d)
{
	int		i;
	int		i_args;
	int		i_ins;
	int		i_outs;
	char	*redir;
	char	*file;
	char	*delimitor;
	int		fd;
	int		mod;

	print_cmd("", d);
	calc_nb_args_ins_outs(s, len, d);
	print_cmd("", d);
	mod_(REINIT_QUOTES);
	i = -1;
	i_args = -1;
	i_ins = -1;
	i_outs = -1;
	while (++i < len)
	{
		mod = mod_(s[i]);
		if (mod == QUOTES0)
		{
			i += nb_spaces(&s[i]);
			redir = redir_(&s[i]);
			file = NULL;
			delimitor = NULL;
			if (ft_strcmp(redir, "<") == 0)
			{
				i += nb_spaces(&s[i]) + ft_strlen(redir);
				file = alphanum_(&s[i], d);
				(*d)->in[++i_ins] = open(file, O_RDONLY);
				//if (!(*d)->in[i_ins]) return (-1)
				free(file);
			}
			else if (ft_strcmp(redir, "<<") == 0)
			{
				i += nb_spaces(&s[i]) + ft_strlen(redir);
				fd = open(TMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
				if (!fd)
					return (printf("%s : tmp file pb\n", (*d)->arg[0]), -1);
				delimitor = alphanum_(&s[i], d);
				heredoc_to_file(delimitor, fd);
				(*d)->in[++i_ins] = open(TMP_FILE, O_RDONLY);
				//if (!(*d)->in[i_ins]) return (-1)
				free(delimitor);
			}
			else if (ft_strcmp(redir, ">") == 0)
			{
				i += nb_spaces(&s[i]) + ft_strlen(redir);
				file = alphanum_(&s[i], d);
				(*d)->out[++i_outs] = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
				//if (!(*d)->out[i_outs]) return (-1)
				free(file);
			}
			else if (ft_strcmp(redir, ">>") == 0)
			{
				i += nb_spaces(&s[i]) + ft_strlen(redir);
				file = alphanum_(&s[i], d);
				(*d)->out[++i_outs] = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
				//if (!(*d)->out[i_outs]) return (-1)
				free(file);
			}
			else
				(*d)->arg[i_args++] = alphanum_(&s[i], d);
		}
	}
	if(mod != QUOTES0)
		return (printf("%s : unclosed quotes\n", s), -1);
	if ((*d)->nb_args == 0)
		return (printf("empty command\n"), -1); // exit_code = 255
	return (0);
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

