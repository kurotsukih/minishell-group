/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akalimol <akalimol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 15:07:09 by akalimol          #+#    #+#             */
/*   Updated: 2023/05/31 17:51:54 by akalimol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static char	*ft_find_home(t_list *env)
{
	while (env)
	{
		if (ft_strncmp((char *)env->content, "HOME=", 5) == 0)
			return ((char *)env->content + 5);
		env = env->next;
	}
	return (NULL);
}

int	ft_execute_cd(t_list *params, t_list *env)
{
	int		count;
	char	*str;

	count = ft_lstsize(params);
	if (count == 0)
	{
		str = ft_find_home(env);
		if (!str)
			return (error_(-1, "bash: cd: HOME not set\n", NULL), -1);
		if (chdir(str) == -1)
			return (error_(-1, "bash: cd: HOME not set properly%s\n", str), -1);
	}
	else if (count > 1)
		return (error_(-1, "bash: cd: Too many arguments\n", NULL), -1);
	else if (chdir((char *)params->content) == -1)
		return (error_(-1, (char *)params->content, NULL), -1);
	return (0);
}

int	ft_strcmp_alt(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (1);
	i = 1;
	while (str[i] && str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (0);
	return (1);
}

void	ft_execute_echo(t_list *token)
{
	int	is_n;

	is_n = 0;
	while (token && ft_strcmp_alt((char *)token->content) == 0)
	{
		is_n = 1;
		token = token->next;
	}
	while (token)
	{
		ft_printf("%s", (char *)token->content);
		if (token->next)
			ft_printf(" ");
		token = token->next;
	}
	if (is_n == 0)
		ft_printf("\n");
}

void	ft_execute_env(t_list *env)
{
	while (env)
	{
		if (ft_strchr((char *)env->content, '='))
			ft_printf("%s\n", (char *)env->content);
		env = env->next;
	}
}
