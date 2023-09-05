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

int	len_spaces(char *s)
{
	int	i;

	i = 0;
	while (s[i] == ' ')
		i++;
	return (i);
	}

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

static int	is_digit(char c)
{
	return (c >= '0' && c < '9');
}

static int	is_letter(char c)
{
	return ((c >= 'a' && c < 'z') || (c >= 'A' && c < 'Z'));
}

static int	len_alphanum(char *s)
{
	int	i;

	if (s == NULL || (!is_digit(s[0]) && !is_letter(s[0])))
		return (0);
	i = -1;
	while (is_digit(s[++i]) || is_letter(s[i]) || s[i] == '_')
		;
	return (i);
}

char	*alphanum_(char *s, t_data **d)
{
	int		i;
	char	*alphanum;

	if (s == NULL || (!is_digit(s[0]) && !is_letter(s[0])))
		return (NULL);
	alphanum = (char *)malloc_(len_alphanum(s) + 1, d);
	i = -1;
	while (is_digit(s[++i]) || is_letter(s[i]) || s[i] == '_')
		alphanum[i] = s[i];
	alphanum[i] = '\0';
	return (alphanum);
}

char	*option_(char *s, t_data **d)
{
	int		i;
	char	*option;

	if (s == NULL || s[0] != '-' || len_alphanum(&s[1]) == 0)
		return (NULL);
	option = (char *)malloc_(len_alphanum(&s[1]) + 2, d);
	option[0] = '-';
	i = 0;
	while (is_digit(s[++i]) || is_letter(s[i]) || s[i] == '_')
		option[i] = s[i];
	option[i] = '\0';
	return (option);
}

// dedollarize d
// exemples : "2&@$A$B^#", '2&@$A$B^#'
static char	*token_in_quotes(char *s, t_data **d)
{
	char	*token;
	int		i;

	if (s[0] != '\'' && s[0] != '\"')
		return (NULL);
	i = 0;
	while(s[i + 1] != s[0] && s[i + 1] != '\0')
		i++;
	if (s[i + 1] == '\0')
		return (NULL);
	token = (char *)malloc_(i, d);
	i = 0;
	while(s[i + 1] != s[0] && s[i + 1] != '\0')
	{
		token[i] = s[i + 1];
		i++;
	}
	token[i] = '\0';
	if (s[0] == '\'')
		return (token);
	return (dedollarized_(token, d));
}

// before dedollarization
int	len_token(char *s, t_data **d)
{
	int		i;

	if (s[0] != '\'' || s[0] != '\"')
	{
		i = 0;
		while(s[i + 1] != s[0] && s[i + 1] != '\0')
			i++;
		if (s[i + 1] == '\0')
			return (err_cmd("unclosed quotes", -1, d), FAILURE);
		return (i);
	}
	i = -1;
	while (s[++i] != ' ' && s[i] != '<' && s[i] != '>'  && s[i] != '\'' && s[i] != '\"'&& s[i] != '\0')
		;
	return (i);
}

// dedollarize d
// exemples tokens : abcd, ab_22, -n, "2&@$A$B^#", '2&@$A$B^#'
char	*token_(char *s, t_data **d)
{
	char	*token;
	int		i;

	token = token_in_quotes(s, d);
	if (token == NULL)
		return (err_cmd("unclosed quotes", -1, d), NULL);
	if (ft_strlen(token) > 0)
		return (token);
	i = -1;
	while (s[++i] != ' ' && s[i] != '<' && s[i] != '>'  && s[i] != '\'' && s[i] != '\"'&& s[i] != '\0')
		;
	if (i == 0)
		return (NULL);
	token = (char *)malloc_(i + 1, d); // malloc(len_token)
	i = -1;
	while (s[++i] != ' ' && s[i] != '<' && s[i] != '>'  && s[i] != '\'' && s[i] != '\"'&& s[i] != '\0')
		token[i] = s[i];
	token[i] = '\0';
	return (dedollarized_(token, d));
}

void calc_nb_args_and_outs(char *s, int len, t_data **d)
{
	int		i;
	char	*redir;
	
	printf("calc_nb_args_and_outs %s %d\n", s, len);
	(*d)->nb_args = 0;
	(*d)->nb_outs = 0;
	mod_(REINIT_QUOTES);
	i = -1;
	while (++i < len)
		if (mod_(s[i]) == QUOTES0)
		{
			printf("i = %d\n", i);
			i += len_spaces(&s[i]);
			redir = redir_(&s[i]);
			if (strcmp_(redir, ">") == 0 || strcmp_(redir, ">>") == 0)
				((*d)->nb_outs)++;
			else if (strcmp_(redir, "<") == 0 || strcmp_(redir, "<<") == 0)
				;
			else
				((*d)->nb_args)++;
			i += ft_strlen(redir);
			i += len_spaces(&s[i]); ////
			i += len_token(&s[i], d) - 1;
		}
	if ((*d)->nb_outs == 0)
		(*d)->nb_outs = 1;
	(*d)->arg = (char **)malloc_(((*d)->nb_args + 1) * sizeof(char *), d);
	(*d)->out = (int *)malloc_((*d)->nb_outs * sizeof(int), d);
	i = -1;
	while (++i < (*d)->nb_args + 1)
		(*d)->arg[i] = NULL;
	i = -1;
	while (++i < (*d)->nb_outs)
		(*d)->out[i] = 1;
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
