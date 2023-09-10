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

	printf("dedollarize_1 %s\n", s);
	// key = alphanum_(&(s[i + 1]), d);
	key = &(s[i + 1]);
	val = get_val_from_env(key, d);
	printf("key = %s, val = %s\n", key, val);
	if (val == NULL)
		return (err_cmd("doll conversion not found", -1, d), NULL);
	len = ft_strlen(s) - ft_strlen(key) + ft_strlen(val);
	dedollarized = (char*)malloc_(len + 1, d);
	k = -1;
	while (++k < i)
		dedollarized[k] = s[k];
	k--;
	while (++k < i + ft_strlen(val))
		dedollarized[k] = val[k - i];
	k--;
	while(++k < len)
		dedollarized[k] = s[k + ft_strlen(key) - ft_strlen(val) + 1];
	dedollarized[k] = '\0';
	//free_(key); NO NEED
	printf("dedollarize_1 return %s\n", dedollarized);
	return (dedollarized);
}

// if fails ?
char	*dedollarized_(char *s, t_data **d)
{
	char	*dedollarized;
	int		mod;
	int		i;

	mod_(REINIT_QUOTES);
	i = -1;
	// printf("s[0] = %c !!!\n",s[0]);
	while (s[++i] != '\0')
	{
		mod = mod_(s[i]);
		// printf("s[%d]=%c, mod = %d\n", i, s[i], mod);
		if (mod != QUOTES1)
		{
			// printf("[%c] == $ ?\n", s[i]);
			// if (s[i] == '$' && s[i + 1] == '?')
			// 	{} // (ft_itoa(exit_code));
			if (s[i] == '$')
			{
				// printf("OUI [%c] == $\n", s[i]);
				// printf("call dedollarize_1\n");
				dedollarized = dedollarized_1(s, i, d);
				free_(s);
				s = dedollarized;
			}
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
	while (i < ft_strlen(paths))
	{
		path = calc_token_str(":\0", &(paths[i_beg]), d);
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
