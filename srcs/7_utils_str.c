/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10_utils_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:52 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/04 02:16:23 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

char	*strndup_and_trim(char *src, int len, t_data **d)
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
		return (0); // ?
	i = 0;
	while (s1[i] != '\0'|| s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return( OK);
}

int	unclosed_quotes(char *s)
{
	int		mod;
	int		i;

	mod_(REINIT_QUOTES);
	i = -1;
	while (s[++i] != '\0')
		mod = mod_(s[i]);
	return (mod != QUOTES0);
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
