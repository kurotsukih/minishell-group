/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtin_exit.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akalimol <akalimol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 22:52:03 by akalimol          #+#    #+#             */
/*   Updated: 2023/05/31 21:01:29 by akalimol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]) != 1)
			return (0);
		i++;
	}
	return (1);
}

int	ft_abs(int num)
{
	if (num < 0)
		return (num * -1);
	return (num);
}

int	ft_execute_exit(t_data *data, t_node *node, t_list *token)
{
	char	*str;
	int		code;

	if (!token)
	{
		code = data->exit_code;
		return (ft_clean_tree(node), ft_clean_tokens(&(data->env)), exit(code), 0);
	}
	str = (char *)token->content;
	if (ft_isnum(str) != 1)
	{
		ft_merror("bash: exit: %s: numeric argument required", str);
		return (ft_clean_tree(node), ft_clean_tokens(&(data->env)), exit(2), 0);
	}
	if (ft_lstsize(token) > 1)
		return (ft_merror("bash: exit: too many arguments\n", NULL), 1);
	code = ft_abs(ft_atoi(str) % 256);
	return (ft_clean_tree(node), ft_clean_tokens(&(data->env)), exit(code), 0);
}
