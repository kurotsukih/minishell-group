/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_utils_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:47 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/12 14:19:12 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

int	skip_spaces(char *s, t_data **d)
{
	int	there_are_skipped_spaces;

	there_are_skipped_spaces = NO;
	while (s[(*d)->i] == ' ')
	{	
		there_are_skipped_spaces = YES;
		((*d)->i)++;
	}
	return (there_are_skipped_spaces);
}

void	calc_redir(char *s, t_data **d)
{
	char	*redir;

	if (s[(*d)->i]== '\0')
		redir = "";
	else if (s[(*d)->i] == '>' && s[(*d)->i + 1] != '>')
		redir = ">";
	else if (s[(*d)->i] == '<' && s[(*d)->i + 1] != '<')
		redir = "<";
	else if (s[(*d)->i] == '>' && s[(*d)->i + 1] == '>')
		redir = ">>";
	else if (s[(*d)->i] == '<' && s[(*d)->i + 1] == '<')
		redir = "<<";
	else
		redir = "";
	// if (ft_strlen(redir) > 0)
	(*d)->redir = redir;
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

	fd = open(TMP_FILE_H, O_WRONLY | O_CREAT | O_TRUNC, 0666);
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

char	*calc_token(char *stop, char *s, t_data **d)
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

int	init_new_cmd(t_data **d)
{
	int	fd_tmp;

	del_all_from_lst((*d)->args);
	(*d)->there_are_redirs_out = NO;
	(*d)->fd_in = -1;
	fd_tmp = open(TMP_FILE, O_RDONLY);
	if (fd_tmp == -1)
		(*d)->fd_in = dup(STDIN); // close ?
	else
		(*d)->fd_in = fd_tmp;
	(*d)->fd_out = dup(STDOUT);
	if ((*d)->fd_in == -1 || (*d)->fd_out == -1)
		return (err_cmd("dup pb", -1, d));
	return (OK);
}

int	init_new_cmd_line(char *s, t_data **d)
{
	if (all_quotes_are_closed(s) != OK)
		return (err_cmd("uncloses quotes", -1, d));
	(*d)->i = 0;
	return (OK);
}

void	init_new_token(t_data **d)
{
	(*d)->redir = "";
	(*d)->token = ""; // free((*d)->token); ?
}

int	put_pipe_redir_if_necessary(char *s, t_data **d)
{
	if ((*d)->there_are_redirs_out == NO && s[(*d)->i] == '|')
		(*d)->fd_out = open(TMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if ((*d)->fd_out == -1)
		return (err_cmd("dup pb", -1, d));
	return (OK);
}
