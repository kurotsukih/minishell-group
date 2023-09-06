/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   7_utils_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:52 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/05 18:29:52 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

static int is_in(char c, char *s)
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

char	*substr_of(char *include, char *src, t_data **d)
{
	char	*dest;
	int		i;

	i = -1;
	while (src[++i] != '\0' && is_in(src[i], include))
		;
	dest = (char *)malloc_(i + 1, d);
	i = -1;
	while (src[++i] != '\0' && is_in(src[i], include))
		dest[i] = src[i];
	return (dest);
}

char	*substr_till(char *stop, char *src, t_data **d)
{
	char	*dest;
	int		i;

	i = -1;
	while (src[++i] != '\0' && !is_in(src[i], stop))
		;
	dest = (char *)malloc_(i + 1, d);
	i = -1;
	while (src[++i] != '\0' && !is_in(src[i], stop))
		dest[i] = src[i];
	dest[i] = '\0';
	return (dest);
}

int	skip_spaces(char *s)
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

// char	*option_(char *s, t_data **d)
// {
// 	int		i;
// 	char	*option;

// 	if (s == NULL || s[0] != '-' || len_alphanum(&s[1]) == 0)
// 		return (NULL);
// 	option = (char *)malloc_(len_alphanum(&s[1]) + 2, d);
// 	option[0] = '-';
// 	i = 0;
// 	while (is_digit(s[++i]) || is_letter(s[i]) || s[i] == '_')
// 		option[i] = s[i];
// 	option[i] = '\0';
// 	return (option);
// }

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

char	*strndup_and_trim(char *src, int len, t_data **d) // sunstr_till =
{
	int		i;
	int		i_beg;
	int		i_end;
	int		new_len;
	char	*dest;

	i = 0;
	while (src[i] == ' ' && src[i] != '\0')
		i++;
	i_beg = i;
	i = len - 1;
	while (src[i] == ' ' && i >= 0)
		i--;
	i_end = i;
	new_len = i_end - i_beg + 1;
	dest = (char *)malloc_(new_len + 1, d);
	i = 0;
	while (i < new_len)
	{
		dest[i] = src[i_beg + i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*strdup_(char *s, t_data **d)
{
	size_t	i;
	char	*dup;

	dup = (char *)malloc_(ft_strlen(s) + 1, d);
	i = -1;
	while (s[++i] != '\0')
		dup[i] = s[i];
	dup[i] = '\0';
	return (dup);
}

int	strcmp_(char *s1, char *s2)
{
	int	i;

	if (s1 == NULL && s2 != NULL)
		return (-1); // ?
	if (s1 != NULL && s2 == NULL)
		return (1); // ?
	if (s1 == NULL && s2 == NULL)
		return (0);
	i = 0;
	while (s1[i] != '\0'|| s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return( OK);
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
