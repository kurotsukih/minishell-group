/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_utils_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:47 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/05 21:51:55 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

void calc_nb_args_and_outs(char *s, int len, t_data **d)
{
	(*d)->nb_args = 0;
	(*d)->nb_outs = 0;
	(*d)->i = 0;
	while ((*d)->i < len)
	{
		(*d)->token = NULL;
		(*d)->i += skip_spaces(&s[(*d)->i]);
		if (s[(*d)->i] == '\'')
			(*d)->token = substr_till("\'", &s[(*d)->i + 1], d);
		else if (s[(*d)->i] == '\"')
			(*d)->token = substr_till("\"", &s[(*d)->i + 1], d); // \0 ?
		else
		{
			(*d)->redir = redir_(&s[(*d)->i]);
			(*d)->i += ft_strlen((*d)->redir);
			(*d)->i += skip_spaces(&s[(*d)->i]);
			(*d)->token = substr_till(" ><\'\"\0", &s[(*d)->i], d);
		}
		(*d)->i += ft_strlen((*d)->token) - 1;
		if (strcmp_((*d)->redir, ">") == 0 || strcmp_((*d)->redir, ">>") == 0) ////
			((*d)->nb_outs)++;
		else if (strcmp_((*d)->redir, "<") == 0 || strcmp_((*d)->redir, "<<") == 0)
			; ////
		else
			((*d)->nb_args)++;
		((*d)->i)++;
	}
	if ((*d)->nb_outs == 0)
		(*d)->nb_outs = 1;
	(*d)->arg = (char **)malloc_(((*d)->nb_args + 1) * sizeof(char *), d);
	(*d)->out = (int *)malloc_((*d)->nb_outs * sizeof(int), d); /// after dup stdout ?
	(*d)->i = -1;
	while (++((*d)->i) < (*d)->nb_args + 1)
		(*d)->arg[(*d)->i] = NULL;
	(*d)->i = -1;
	while (++((*d)->i) < (*d)->nb_outs)
		(*d)->out[(*d)->i] = 1;
	(*d)->in = dup(STDIN_FILENO); // prv pipe if
	((*d)->out)[0] = dup(STDOUT_FILENO); // nxt pipe
	if ((*d)->in == -1 || ((*d)->out)[0] == -1)
		free_all_and_exit("dup failure", -1, d);
}

int	heredoc_to_file(char *delim, t_data **d)
{
	char	*line;
	int		fd;

	fd = open(TMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (!fd)
		return (err_cmd("heredoc open tmp file pb", -1, d));
	line = NULL;
	while (1)
	{
		line = readline(">");
		if (line == NULL || strcmp_(line, delim) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, " ", 1);
		free_(line);
	}
	close(fd);
	return (OK);
}
