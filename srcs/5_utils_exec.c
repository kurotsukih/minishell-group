/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_utils_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:47 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/12 15:29:19 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

char	*get_val_from_env(char *key, t_data **d)
{
	t_lst	*env;
	char	*key_env;
	char	*val_env;

	if (len_lst((*d)->env) == 0)
		return (NULL);
	val_env = NULL;
	env = *((*d)->env);
	while (env != NULL)
	{
		key_env = calc_token("=\0", env->val, d);
		if (ft_strcmp(key, key_env) == 0)
		{
			val_env = &((env->val)[ft_strlen(key) + 1]);
			if (val_env == NULL)
				return (err_cmd("env variable not found", -1, d), NULL);
			return (val_env);
		}
		env = env->nxt;
	}
	return (err_cmd("env variable not found", -1, d), NULL);
}

static char	*dedollarize_1_var(char *s, int i, t_data **d)
{
	char	*dedollarized;
	int		len;
	char	*key;
	char	*val;
	int		k;

	key = &(s[i + 1]);
	val = get_val_from_env(key, d);
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
	return (dedollarized);
}

// if fails 
char	*dedollarize_str(char *s, t_data **d)
{
	char	*dedollarized;
	int		i;

	i = -1;
	while (s[++i] != '\0')
		if (s[i] == '$')
		{
			if (s[i + 1] == '?')
				{} // dedollarized = ft_itoa(exit_code);
			else
				dedollarized = dedollarize_1_var(s, i, d);
			free_(s);
			s = dedollarized;
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
		path = calc_token(":\0", &(paths[i_beg]), d);
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
