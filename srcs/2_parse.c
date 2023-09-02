/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_parse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:26 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/02 15:22:27 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

// >out1 >out1+ >> out2 < in >> out2+ < in+ <in++
static void put_full_cmd_to_arg0_1(char *full_cmd, int len, t_data **d)
{
	t_cmd *cmd;
	t_cmd *last;
	int i;

	init_cmd(&cmd, d); // deplace here
	(*d)->curr_cmd = cmd;
	cmd->nb_args = nb_args_(full_cmd, len, d);
	cmd->arg = (char **)malloc_((cmd->nb_args + 1) * sizeof(char *), d);
	cmd->arg[0] = (char *)malloc_(len + 1, d);
	i = 0;
	while (++i < cmd->nb_args + 1)
		cmd->arg[i] = NULL;
	i = -1;
	while (++i < len)
		cmd->arg[0][i] = full_cmd[i];
	cmd->arg[0][i] = '\0';
	if (*((*d)->cmds) == NULL)
		*((*d)->cmds) = cmd;
	else
	{
		last = *((*d)->cmds);
		while (last != NULL && last->nxt != NULL)
			last = last->nxt;
		last->nxt = cmd;
		cmd->prv = last;
	}
}

static void put_redirs(t_cmd *cmd, t_data **d)
{
	char *s;
	int i;
	int i_beg;
	char *redir;
	char *redir_file;

	mod_(REINIT_QUOTES);
	s = strdup_and_erase_args_except_redirs(cmd->arg[0], d);
	i = -1;
	while (s[++i] != '\0')
	{
		redir = redir_(&s[i]);
		if (mod_(s[i]) == QUOTES0 && ft_strlen(redir) > 0)
		{
			i += ft_strlen(redir);
			i_beg = i;
			while (s[i] == ' ')
				i++;
			while (s[i] != ' ' && s[i] != '>' && s[i] != '<' && s[i] != '\0') // alphanum ?
				i++;
			redir_file = strndup_and_trim(&s[i_beg], i - i_beg + 1, d);
			open_file(redir, redir_file, cmd);
		}
	}
}

static void put_args(t_cmd *cmd, t_data **d)
{
	int i;
	int i_beg;
	int k;
	int i_end_arg0;
	char *s;

	if (cmd->nb_args == 0)
		return ;
	mod_(REINIT_QUOTES);
	i_beg = 0;
	k = 0;
	i = -1;
	s = strdup_and_erase_redirs(cmd->arg[0], d);
	while (s[++i] != '\0')
		if (mod_(s[i]) == QUOTES0 && s[i] != ' ' && (s[i + 1] == ' ' \
			|| s[i + 1] == '\0' || s[i + 1] == '\'' || s[i + 1] == '\"'))
		{
			if (k == 0)
				i_end_arg0 = i + 1;
			else
				cmd->arg[k] = strndup_and_trim(&s[i_beg], i - i_beg + 1, d);
			i_beg = i + 1;
			k++;
		}
	free(s);
	cmd->arg[0][i_end_arg0] = '\0';
	s = strndup_and_trim(cmd->arg[0], ft_strlen(cmd->arg[0]), d); // strdup_and_trim
	free(cmd->arg[0]);
	cmd->arg[0] = s;
	cmd->arg[cmd->nb_args] = NULL;
}

static void	put_full_cmd_to_arg0(char *s, t_data **d)
{
	int i_beg;
	int i;

	mod_(REINIT_QUOTES);
	i_beg = 0;
	i = -1;
	while (1)
		if ((mod_(s[++i]) == QUOTES0 && s[i + 1] == '|') || s[i + 1] == '\0')
		{
			put_full_cmd_to_arg0_1(&s[i_beg], i - i_beg + 1, d);
			if (s[i + 1] == '\0') // ++i
				break;
			i++;
			i_beg = i + 1;
		}
	(*d)->curr_cmd = NULL;
}

void	parse(char *cmd_line, t_data **d)
{
	t_cmd *cmd;

	put_full_cmd_to_arg0(cmd_line, d);
	cmd = *((*d)->cmds);
	while (cmd != NULL)
	{
		(*d)->curr_cmd = cmd;
		put_redirs(cmd, d);
		put_args(cmd, d);
		cmd = cmd->nxt;
	}
}

