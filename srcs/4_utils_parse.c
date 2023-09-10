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

void	skip_spaces(char *s, t_data **d)
{
	while (s[(*d)->i] == ' ')
		((*d)->i)++;
}

void	calc_redir(char *s, t_data **d)
{
	if (s[(*d)->i]== '\0')
		(*d)->redir = "";
	else if (s[(*d)->i] == '>' && s[(*d)->i + 1] != '>')
		(*d)->redir = ">";
	else if (s[(*d)->i] == '<' && s[(*d)->i + 1] != '<')
		(*d)->redir = "<";
	else if (s[(*d)->i] == '>' && s[(*d)->i + 1] == '>')
		(*d)->redir = ">>";
	else if (s[(*d)->i] == '<' && s[(*d)->i + 1] == '<')
		(*d)->redir = "<<";
	else
		(*d)->redir = "";
	((*d)->i) += ft_strlen((*d)->redir);
}

int is_in(char c, char *s)
{
	int	i;

	if (s == NULL)
		return (NO);
	i = -1;
	while (s[++i] != '\0')
		if (s[i] == c)
			return (YES);
	return (NO);
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
		if (line == NULL || ft_strcmp(line, delim) == 0)
			break ;
		write_fd(fd, line);
		write_fd(fd, " ");
		free_(line);
	}
	close(fd);
	return (OK);
}

int	all_quotes_are_closed(char *s)
{
	int		mod;
	int		i;

	mod_(REINIT_QUOTES);
	i = -1;
	while (s[++i] != '\0')
		mod = mod_(s[i]);
	if (mod != QUOTES0)
		return (FAILURE);
	return (OK);
}

int	mod_(char c)
{
	static char	mod = QUOTES0;

	if (c == REINIT_QUOTES)
		mod = QUOTES0;
	else if (mod == QUOTES0 && c == '\'')
		mod = QUOTES1;
	else if (mod == QUOTES0 && c == '\"')
		mod = QUOTES2;
	else if (mod == QUOTES1 && c == '\'')
		mod = QUOTES0;
	else if (mod == QUOTES2 && c == '\"')
		mod = QUOTES0;
	return (mod);
}

char	*calc_token_str(char *stop, char *s, t_data **d)
{
	char	*token;
	int		i;

	i = 0;
	while (1)
	{
		if (s[i] == '\0' || is_in(s[i], stop))
			break ;
		i++;
	}
	token = (char *)malloc_(i + 1, d);
	i = 0;
	while (1)
	{
		if (s[i] == '\0' || is_in(s[i], stop))
			break ;
		token[i] = s[i];
		i++;
	}
	token[i] = '\0';
	return (token);
}

int	put_stdout_to_d(t_data **d)
{
	int *out;

	out = (int *)malloc(sizeof(int));
	out[0] = dup(STDOUT_FILENO);
	if (out[0] == -1)
		return (err_cmd("dup stdout pb", -1, d));
	put_to_lst((void *)(&out[0]), &((*d)->outs), d);
	return (OK);
}
