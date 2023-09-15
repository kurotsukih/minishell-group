/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_parse_and_opens_fds.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:16 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/15 12:03:30 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

static int	put_token_or_fd_to_d(t_data **d)
{
	if (ft_strcmp((*d)->redir, "<<") == 0)
	{
		heredoc_to_file((*d)->token, d);
		(*d)->fd_in = open(TMP_FILE_HEREDOC, O_RDONLY); //-
	}
	else if (ft_strcmp((*d)->redir, "<") == 0)
		(*d)->fd_in = open((*d)->token, O_RDONLY); //-
	else if (ft_strcmp((*d)->redir, ">>") == 0)
		(*d)->fd_out = open((*d)->token, O_WRONLY | O_CREAT | O_APPEND, 0666); //-
	else if (ft_strcmp((*d)->redir, ">") == 0)
		(*d)->fd_out = open((*d)->token, O_WRONLY | O_CREAT | O_TRUNC, 0666); //-
	else
		put_to_lst((*d)->token, &((*d)->args), d);
	// if (((*d)->redir)[0] == '<' && (*d)->in == -1) || ((*d)->redir)[0] == '>' && out == -1)
	// 	return (FAILURE);
	if (((*d)->redir)[0] == '>')
		(*d)->there_are_redirs_out = YES;
	return (OK);
}

// no matter what this func returns
int	parse_nxt_token_and_put_to_d(char *cmd_line, t_data **d)
{
	skip_spaces(cmd_line, d);
	if (cmd_line[(*d)->i] == '\'') // to verify !!!
	{
		(*d)->token = calc_token("\'\0", &cmd_line[(*d)->i + 1], d);
		(*d)->i += ft_strlen((*d)->token) + 2;
	}
	else if (cmd_line[(*d)->i] == '\"') // to verify !!!
	{
		(*d)->token = calc_token("\"\0", &cmd_line[(*d)->i + 1], d);
		(*d)->i += ft_strlen((*d)->token) + 2;
		(*d)->token = dedollarize_str((*d)->token, d);
	}
	else
	{
		calc_redir(cmd_line, d);
		skip_spaces(cmd_line, d);
		(*d)->token = calc_token(" \"\'<>|", &cmd_line[(*d)->i], d);
		((*d)->i) += ft_strlen((*d)->token);
		(*d)->token = dedollarize_str((*d)->token, d);
	}
	if (ft_strlen((*d)->token) > 0 && put_token_or_fd_to_d(d) == FAILURE)
		return (err_cmd("put token pb", 1, d)); // code 1 ?
	if (skip_spaces(cmd_line, d) == YES && ft_strcmp(((*d)->args)[0]->val, "echo") == 0 && len_lst((*d)->args) > 1) // for spaces in echo
		{
			(*d)->token=" "; // can we free it in the end ?
			if (put_token_or_fd_to_d(d) == FAILURE)
				return (err_cmd("get token pb", 1, d)); // code 1 ?
		} 
	return (OK);
}
