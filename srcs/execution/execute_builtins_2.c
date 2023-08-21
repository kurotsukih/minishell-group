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

int	ft_execute_exit(t_data *data, t_node *n, t_list *token)
{
	char	*str;
	int		code;

	if (!token)
	{
		code = data->exit_code;
		return (ft_clean_tree(n), free_redirections(*(&(data->env))), exit(code), 0);
	}
	str = (char *)token->content;
	if (ft_isnum(str) != 1)
	{
		exit_(-1, "bash: exit: %s: numeric argument required", str, NULL, NULL, NULL);
		return (ft_clean_tree(n), free_redirections(*(&(data->env))), exit(2), 0);
	}
	if (ft_lstsize(token) > 1)
		return (exit_(-1, "bash: exit: too many arguments\n", NULL, NULL, NULL, NULL), 1);
	code = ft_abs(ft_atoi(str) % 256);
	return (ft_clean_tree(n), free_redirections(*(&(data->env))), exit(code), 0);
}

void	ft_execute_pwd(void)
{
	char	*response;

	response = getcwd(NULL, 0);
	ft_printf("%s\n", response);
	free(response);
}

void	ft_execute_unset(t_list **env, t_list *head)
{
	t_list	*token;
	char	*str;
	int		len;
	char	*cmd;

	if (!head)
		return ;
	cmd = (char *)head->content;
	len = ft_strlen(cmd);
	token = *env;
	while (token)
	{
		str = (char *)token->content;
		if (ft_strncmp(str, cmd, len) == 0 && str[len] == '=')
			break ;
		token = token->next;
	}
	if (!token)
		return ;
	ft_lstremove(env, token, NULL);
}

