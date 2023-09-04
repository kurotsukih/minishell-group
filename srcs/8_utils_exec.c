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

static char	*s_with_conversion_(char *old_s, int j, t_data **d)
{
	char	*new_s;
	int		len_new_s;
	char	*key;
	char	*val;
	int		k;

	key = alphanum_(&(old_s[j + 1]), d);
	val = get_value_from_env(key, d);
	len_new_s = ft_strlen(old_s) - ft_strlen(key) + ft_strlen(val);
	new_s = (char*)malloc_(len_new_s + 1, d);
	k = -1;
	while (++k < j)
		new_s[k] = old_s[k];
	k--; //
	while (++k < j + (int)ft_strlen(val))
		new_s[k] = val[k - j];
	k--;
	while(++k < len_new_s)
		new_s[k] = old_s[k + (int)ft_strlen(key) - (int)ft_strlen(val) + 1];
	new_s[k] = '\0';
	free_(key);
	return (new_s);
}

void	calc_dollar_conversions(char *s, t_data **d)
{
	char	*s_with_conversion;
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
				s_with_conversion = s_with_conversion_(s, i, d);
				free_(s);
				s = s_with_conversion;
			}
		}
}

// int	calc_dollar_conversions(t_cmd *cmd, t_data **d)
// {
// 	char	*s_with_doll_conversion;
// 	int		i;
// 	int		j;

// 	(*d)->curr_cmd = cmd;
// 	i = 0;
// 	while(++i < cmd->nb_args)
// 		if (cmd->arg[i][0] != '\'')
// 		{
// 			j = -1;
// 			while (cmd->arg[i][++j] != '\0' && cmd->arg[i][j + 1] != '\0')
// 			{
// 				// if (cmd->arg[i][j] == '$' && cmd->arg[i][j + 1] == '?')
// 				// 	(ft_itoa(exit_code));
// 				if (cmd->arg[i][j] == '$')
// 				{
// 					s_with_doll_conversion = s_with_doll_conversion_(cmd->arg[i], j, d);
// 					free(cmd->arg[i]);
// 					cmd->arg[i] = s_with_doll_conversion;
// 				}
// 			}
// 		}
// }

// static void	remove_quotes_str(char *str)
// {
// 	char	mode;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	mode = QUOTES0;
// 	while (str[i])
// 	{
// 		if (mode == QUOTES0 && str[i] == '\'')
// 			mode = QUOTES1;
// 		else if (mode == QUOTES1 && str[i] == '\'')
// 			mode = QUOTES0;
// 		else if (mode == QUOTES0 && str[i] == '\"')
// 			mode = QUOTES2;
// 		else if (mode == QUOTES2 && str[i] == '\"')
// 			mode = QUOTES0;
// 		else
// 			str[j++] = str[i];
// 		i++;
// 	}
// 	while (j != i)
// 		str[j++] = '\0';
// }

// void	remove_quotes(t_cmd *cmd)
// {
// 	int		i;

// 	i = -1; // 0 ?
// 	while (++i < cmd->nb_args)
// 		remove_quotes_str(cmd->arg[i]);
// }

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
	if (path == NULL)
		return (NULL);
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
			path = strndup_and_trim(&(paths_str[i_beg]), i - i_beg, d); // alphanum
			path = path2_(path, (*d)->arg[0], d);
			if (access(path, X_OK) == 0)
				return (path);
			free_(path);
			i_beg = i + 1;
		}
	return (printf("%s : cmd not found\n", (*d)->arg[0]), NULL); 
	// exit_code = 127, if (errno != 2) exit_c = 126;
}
