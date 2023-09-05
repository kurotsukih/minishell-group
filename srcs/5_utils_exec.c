/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8_utils_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:47 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/04 02:15:48 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

/*
// int	is_builtin(t_data **d)
// {
// 	return (\
// 	strcmp_((*d)->arg[0], "cd") == 0 || \
// 	strcmp_((*d)->arg[0], "exit") == 0 || \
// 	strcmp_((*d)->arg[0], "export") == 0 || \
// 	strcmp_((*d)->arg[0], "unset") == 0 || \
// 	strcmp_((*d)->arg[0], "echo") == 0 || \
// 	strcmp_((*d)->arg[0], "pwd") == 0 || \
// 	strcmp_((*d)->arg[0], "env") == 0);
// }
*/

static char	*doll_conversion_(char *old_s, int j, t_data **d)
{
	char	*doll_conversion;
	int		len;
	char	*key;
	char	*val;
	int		k;

	key = alphanum_(&(old_s[j + 1]), d);
	val = get_value_from_env(key, d);
	if (val == NULL)
		return (err_cmd("doll conversion not found", -1, d), NULL);
	len = ft_strlen(old_s) - ft_strlen(key) + ft_strlen(val);
	doll_conversion = (char*)malloc_(len + 1, d);
	k = -1;
	while (++k < j)
		doll_conversion[k] = old_s[k];
	k--; //
	while (++k < j + (int)ft_strlen(val))
		doll_conversion[k] = val[k - j];
	k--;
	while(++k < len)
		doll_conversion[k] = old_s[k + (int)ft_strlen(key) - (int)ft_strlen(val) + 1];
	doll_conversion[k] = '\0';
	free_(key);
	return (doll_conversion);
}

char	*doll_conversions_(char *s, t_data **d)
{
// if fails ?
	char	*doll_conversions;
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
				doll_conversions = doll_conversion_(s, i, d);
				free_(s);
				s = doll_conversions;
			}
		}
	return (s);
}

static char	*path2_(char *s1, char *s2, t_data **d)
{
	int		i;
	int		s1_len;
	int		s2_len;
	char	*path;

	s1_len = 0;
	if (s1 != NULL)
		s1_len = ft_strlen(s1); // 0 for NULL
	s2_len = ft_strlen(s2);
	path = (char *)malloc_(s1_len + s2_len + 2, d);
	i = -1;
	while (++i < s1_len)
		path[i] = s1[i];
	if(s1 != NULL)
		path[i] = '/'; //new_str[i] = '/'
	i = -1;
	while (++i < s2_len)
		path[s1_len + 1 + i] = s2[i];
	path[s1_len + 1 + i] = '\0';
	return (path);
}

char	*path_(t_data **d)
{
	char	*paths_str;
	char	*path;
	int		i;
	int		i_beg;

	paths_str = get_value_from_env("PATH", d);
	i_beg = 0;
	i = -1;
	while (++i < (int)ft_strlen(paths_str))
		if (paths_str[i] == ':' || paths_str[i] == '\0')
		{
			path = strndup_and_trim(&(paths_str[i_beg]), i - i_beg, d);
			path = path2_(path, (*d)->arg[0], d);
			if (access(path, X_OK) == 0)
				return (path);
			free_(path);
			i_beg = i + 1;
		}
	err_cmd("cmd not found", 127, d); // 127? if (errno != 2) 126 ?
	return (NULL);
}
