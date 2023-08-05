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

#include "minishell.h"
void	ft_put(char *string, int *j, int *i, char *meta)
{
	int	size;

	size = 0;
	string[*j] = ' ';
	*j = *j + 1;
	while (meta && meta[size])
	{
		string[*j] = meta[size];
		*j = *j + 1;
		size++;
	}
	string[*j] = ' ';
	*j = *j + 1;
	if (size == 2)
		*i = *i + 1;
}

size_t	ft_strlcpy_alt(char *restrict dst, const char *restrict src,
		size_t dstsize)
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

void	*ft_memset2(void *b, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
		((unsigned char *)b)[i++] = c;
	return (b);
}

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
	ft_memset2(str + j, '\0', size - j);
}

/**
 * @brief Add space between meta characters
 * 
 * @example:
 *          Before:  (cat||ls)|pwd
 *          After:   ( cat || ls ) | pwd
 * 
 * helper functions: ./utils/ft_add_spaces_utils.c
 *  - ft_size()					
						- Calculates the overall size of the string with 
 * 									spaces
 *  - ft_is_metacharacter()    - checks if the character is meta
 *  - ft_put()                 - concantenates the meta characters with spaces
 *  - ft_remove_extra...()     - removes preeceding, succeeding, and duplicating
 *                       			spaces  
 * 
 * @param str   string to proceed
 * @return      string with spaces
 */

void	ft_set_mode_add_space(char *mode, char c1, char c2)
{
	if (*mode == 0 && c1 == '\'')
		*mode = '\'';
	else if (*mode == '\'' && c1 == '\'')
		*mode = '\0';
	else if (*mode == 0 && c1 == '\"')
		*mode = '\"';
	else if (*mode == '\"' && c1 == '\"')
		*mode = '\0';
	else if (c2 == '\0')
		*mode = '\0';
}

char	*ft_is_metacharacter(char c1, char c2)
{
	static char	mode;

	ft_set_mode_add_space(&mode, c1, c2);
	if (mode != '\0')
		return (NULL);
	if (c1 == '|' && c2 == '|')
		return ("||");
	if (c1 == '|')
		return ("|");
	if (c1 == '&' && c2 == '&')
		return ("&&");
	if (c1 == '(')
		return ("(");
	if (c1 == ')')
		return (")");
	if (c1 == '<' && c2 == '<')
		return ("<<");
	if (c1 == '<')
		return ("<");
	if (c1 == '>' && c2 == '>')
		return (">>");
	if (c1 == '>')
		return (">");
	return (NULL);
}

int	ft_size(char *str)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (str[i])
	{
		if (ft_is_metacharacter(str[i], str[i + 1]) != 0)
			size++;
		i++;
	}
	return (i + size * 2);
}

// The problem with a quotes. All metas inside of them, should be skipped.
char	*ft_add_spaces(char *str)
{
	char	*string;
	char	*meta;
	int		i;
	int		j;
	int		size;

	size = ft_size(str) + 1;
	string = (char *)ft_calloc(sizeof(char), size + 1);
	if (!string)
		return (ft_error(), free(str), NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		meta = ft_is_metacharacter(str[i], str[i + 1]);
		if (ft_isspace(str[i]) == 1)
			string[j++] = ' ';
		else if (meta)
			ft_put(string, &j, &i, meta);
		else
			string[j++] = str[i];
		i++;
	}
	ft_remove_extra_spaces(string, size + 1);
	return (free(str), string);
}
