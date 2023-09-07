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

char	*alphanum_(char *s, t_data **d)
{
	int		i;
	char	*alphanum;

	if (s == NULL || (!ft_isdigit(s[0]) && !ft_isalpha(s[0])))
		return (NULL);
	i = -1;
	while (ft_isdigit(s[++i]) || ft_isalpha(s[i]) || s[i] == '_')
		;
	alphanum = (char *)malloc_(i + 1, d);
	i = -1;
	while (ft_isdigit(s[++i]) || ft_isalpha(s[i]) || s[i] == '_')
		alphanum[i] = s[i];
	alphanum[i] = '\0';
	return (alphanum);
}

static int is_in(char c, char *s)
{
	int	i;

	// printf("is in [%c] [%s] ?\n", c == '\0' ? '0' : c, s);
	if (s == NULL)
		return (NO);
	i = -1;
	while (s[++i] != '\0')
		if (s[i] == c)
			return (YES);
	return (NO);
}

void	calc_token(char *stop, char *s, t_data **d)
{
	char	*token;
	int		i;

	i = (*d)->i;
	printf("calc token [%s] %d\n", s, (*d)->i);
	while (1)
	{
		if (s[i] == '\0' || is_in(s[i], stop))
			break ;
		i++;
	}
	token = (char *)malloc_(i - (*d)->i + 1, d);
	i = (*d)->i;
	while (1)
	{
		if (s[i] == '\0' || is_in(s[i], stop))
			break ;
		token[i - (*d)->i] = s[i];
		i++;
	}
	token[i - (*d)->i] = '\0';
	((*d)->i) += i - (*d)->i;
	(*d)->token = token;
	printf("calc token end, token = %s, %d\n", token, (*d)->i);
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

// dedollarize d
// exemples : "2&@$A$B^#", '2&@$A$B^#'
// static char	*token_in_quotes(char *s, t_data **d)
// {
// 	char	*token;
// 	int		i;

// 	if (s[0] != '\'' && s[0] != '\"')
// 		return (NULL);
// 	i = 0;
// 	while(s[i + 1] != s[0] && s[i + 1] != '\0')
// 		i++;
// 	if (s[i + 1] == '\0')
// 		return (NULL);
// 	token = (char *)malloc_(i, d);
// 	i = 0;
// 	while(s[i + 1] != s[0] && s[i + 1] != '\0')
// 	{
// 		token[i] = s[i + 1];
// 		i++;
// 	}
// 	token[i] = '\0';
// 	if (s[0] == '\'')
// 		return (token);
// 	return (dedollarized_(token, d));
// }

// before dedollarization
// int	len_token(char *s, t_data **d)
// {
// 	int		i;

// 	if (s[0] != '\'' || s[0] != '\"')
// 	{
// 		i = 0;
// 		while(s[i + 1] != s[0] && s[i + 1] != '\0')
// 			i++;
// 		if (s[i + 1] == '\0')
// 			return (err_cmd("unclosed quotes", -1, d), FAILURE);
// 		return (i);
// 	}
// 	i = -1;
// 	while (s[++i] != ' ' && s[i] != '<' && s[i] != '>'  && s[i] != '\'' && s[i] != '\"'&& s[i] != '\0')
// 		;
// 	return (i);
// }

// dedollarize d
// exemples tokens : abcd, ab_22, -n, "2&@$A$B^#", '2&@$A$B^#'
// char	*token_(char *s, t_data **d)
// {
// 	char	*token;
// 	int		i;

// 	token = token_in_quotes(s, d);
// 	if (token == NULL)
// 		return (err_cmd("unclosed quotes", -1, d), NULL);
// 	if (ft_strlen(token) > 0)
// 		return (token);
// 	i = -1;
// 	while (s[++i] != ' ' && s[i] != '<' && s[i] != '>'  && s[i] != '\'' && s[i] != '\"'&& s[i] != '\0')
// 		;
// 	if (i == 0)
// 		return (NULL);
// 	token = (char *)malloc_(i + 1, d); // malloc(len_token)
// 	i = -1;
// 	while (s[++i] != ' ' && s[i] != '<' && s[i] != '>'  && s[i] != '\'' && s[i] != '\"'&& s[i] != '\0')
// 		token[i] = s[i];
// 	token[i] = '\0';
// 	return (dedollarized_(token, d));
// }

