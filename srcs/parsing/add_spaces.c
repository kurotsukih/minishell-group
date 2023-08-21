/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akalimol <akalimol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 00:12:17 by akalimol          #+#    #+#             */
/*   Updated: 2023/05/31 21:33:12 by akalimol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Add space between meta characters
// (cat||ls)|pwd  to  ( cat || ls ) | pwd

#include "minishell.h"

// concantenates the meta characters with spaces
 void	ft_put(char *str, int *j, int *i, char *metachar)
{
	int	size;

	size = 0;
	str[*j] = ' ';
	*j = *j + 1;
	while (metachar && metachar[size])
	{
		str[*j] = metachar[size];
		*j = *j + 1;
		size++;
	}
	str[*j] = ' ';
	*j = *j + 1;
	if (size == 2)
		*i = *i + 1;
}

size_t	ft_strlcpy_alt(char *restrict dst, const char *restrict src, size_t dstsize)
{
	size_t	src_len;
	size_t	i;

	i = 0;
	src_len = ft_strlen(src);
	while (i + 1 < dstsize && i < src_len)
	{
		dst[i] = src[i];
		i++;
	}
	if (dstsize != 0)
	{
		while (i < dstsize)
		{
			dst[i] = '\0';
			i++;
		}
	}
	return (src_len);
}

// removes preeceding, succeeding, and duplicating spaces
void	ft_remove_extra_spaces(char *str, int size)
{
	int	i;
	int	j;
	int	len;

	len = ft_strlen(str);
	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	ft_strlcpy_alt(str, &str[i], len + 1);
	i = len - 1;
	while (i >= 0 && str[i] == ' ')
	{
		str[i] = '\0';
		i--;
	}
	i = 0;
	j = 0;
	while (i < len)
	{
		if (!(str[i] == ' ' && str[i + 1] == ' '))
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
	ft_memset(str + j, '\0', size - j); ////
}

char	*metachar_(char c1, char c2)
{
	static char	mode = '\0'; ////

	if (mode == 0 && c1 == '\'')
		mode = '\'';
	else if (mode == '\'' && c1 == '\'')
		mode = '\0';
	else if (mode == 0 && c1 == '\"')
		mode = '\"';
	else if (mode == '\"' && c1 == '\"')
		mode = '\0';
	else if (c2 == '\0')
		mode = '\0';
	if (mode != '\0')
		return (NULL);
	if (c1 == '|')
		return ("|");
	if (c1 == '(')
		return ("(");
	if (c1 == ')')
		return (")");
	if (c1 == '<' && c2 == '<')
		return ("<<");
	if (c1 == '>' && c2 == '>')
		return (">>");
	if (c1 == '<')
		return ("<");
	if (c1 == '>')
		return (">");
	return (NULL);
}

int	new_size(char *str)
{
	int	i;
	int	size;

	i = 0;
	while (str[i])
	{
		if (metachar_(str[i], str[i + 1]) != 0)
			size++;
		i++;
	}
	return (i + size * 2);
}

// The problem with a quotes. All metas inside of them, should be skipped.
char	*add_spaces(char *str)
{
	char	*new_str;
	char	*metachar;
	int		i;
	int		j;
	int		size;

	size = new_size(str) + 1;
	new_str = (char *)malloc(size + 1);
	if (!new_str)
		return (exit_(-1, NULL, NULL, NULL, NULL), free(str), NULL);
	ft_memset(new_str, '\0', size + 1); /// было memset_
	i = 0;
	j = 0;
	while (str[i])
	{
		metachar = metachar_(str[i], str[i + 1]);
		if (ft_isspace(str[i]) == 1)
			new_str[j++] = ' ';
		else if (metachar)
			ft_put(new_str, &j, &i, metachar);
		else
			new_str[j++] = str[i];
		i++;
	}
	ft_remove_extra_spaces(new_str, size + 1);
	return (free(str), new_str);
}
