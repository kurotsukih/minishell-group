/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8_utils_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:49 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/03 00:56:46 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

char *key_(char *s, t_data **d)
{
	char	*key;
	int		i;
	int		len_key;

	len_key = 0;
	i = -1;
	while (s[++i] != '=' && s[i] != '\0')
		len_key ++;
	key = (char *)malloc_(len_key + 1, d);
	i = 0;
	while (i < len_key)
	{
		key[i] = s[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

char *val_(char *s, t_data **d)
{
	int		i;
	char	*val;
	int		len_key;
	int		len_val;

	i = -1;
	while (s[++i] != '\0')
		if (s[i] == '=')
			break ;
	if (s[i] != '=')
		return (NULL);
	len_key = i;
	len_val = 0;
	while (s[++i] != '\0')
		len_val++ ;
	val = (char *)malloc_(len_val + 1, d);
	i = 0;
	while (s[len_key + i + 1] != '\0')
	{
		val[i] = s[len_key + i + 1];
		i++;
	}
	val[i] = '\0';
	return (val);
}

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
		key_env = calc_token_str("=\0", (char *)(env->val), d);
		if (ft_strcmp(key, key_env) == 0)
		{
			val_env = &(((char *)(env->val))[ft_strlen(key) + 1]);
			if (val_env == NULL)
				return (err_cmd("env variable not found", -1, d), NULL);
			return (val_env);
		}
		env = env->nxt;
	}
	return (err_cmd("env variable not found", -1, d), NULL);
}

