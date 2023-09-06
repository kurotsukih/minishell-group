/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_utils_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:47 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/05 21:40:09 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

static char	*dedollarized_1(char *s, int i, t_data **d)
{
	char	*dedollarized;
	int		len;
	char	*key;
	char	*val;
	int		k;

	key = alphanum_(&(s[i + 1]), d);
	val = get_val_from_env(key, d);
	if (val == NULL)
		return (err_cmd("doll conversion not found", -1, d), NULL);
	len = ft_strlen(s) - ft_strlen(key) + ft_strlen(val);
	dedollarized = (char*)malloc_(len + 1, d);
	k = -1;
	while (++k < i)
		dedollarized[k] = s[k];
	k--;
	while (++k < i + (int)ft_strlen(val))
		dedollarized[k] = val[k - i];
	k--;
	while(++k < len)
		dedollarized[k] = s[k + (int)ft_strlen(key) - (int)ft_strlen(val) + 1];
	dedollarized[k] = '\0';
	free_(key);
	return (dedollarized);
}

char	*dedollarized_(char *s, t_data **d)
{
// if fails ?
	char	*dedollarized;
	int		i;

	mod_(REINIT_QUOTES);
	i = -1;
	while (s[++i] == '\0')
		if (mod_(s[i]) != QUOTES1)
		{
			if (s[i] == '$' && s[i + 1] == '?')
				{} // (ft_itoa(exit_code));
			else if (s[i] == '$')
			{
				dedollarized = dedollarized_1(s, i, d);
				free_(s);
				s = dedollarized;
			}
		}
	return (s);
}

static char	*full_path_(char *s1, char *s2, t_data **d)
{
	int		i;
	int		s1_len;
	int		s2_len;
	char	*full_path;

	s1_len = 0;
	if (s1 != NULL)
		s1_len = ft_strlen(s1); // 0 for NULL
	s2_len = ft_strlen(s2);
	full_path = (char *)malloc_(s1_len + s2_len + 2, d);
	i = -1;
	while (++i < s1_len)
		full_path[i] = s1[i];
	if(s1 != NULL)
		full_path[i] = '/';
	i = -1;
	while (++i < s2_len)
		full_path[s1_len + 1 + i] = s2[i];
	full_path[s1_len + 1 + i] = '\0';
	return (full_path);
}

char	*path_(t_data **d)
{
	char	*paths;
	char	*path;
	char	*full_path;
	int		i;
	int		i_beg;

	paths = get_val_from_env("PATH", d);
	i_beg = 0;
	i = 0;
	while (i < (int)ft_strlen(paths))
	{
		path = substr_till(":", &(paths[i_beg]), d);
		full_path = full_path_(path, (*((*d)->args))->val, d);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i += ft_strlen(path) + 1;
		i_beg = i;
		free_(full_path);
		free_(path);
	}
	return (err_cmd("cmd not found", 127, d), NULL); // 127? if (errno != 2) 126 ?
}
