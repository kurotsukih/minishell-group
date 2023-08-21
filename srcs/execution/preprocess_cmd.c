/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_preprocess_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akalimol <akalimol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 12:36:28 by akalimol          #+#    #+#             */
/*   Updated: 2023/05/31 17:57:15 by akalimol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*ft_preprocess_parameter(t_list *token, t_cmd *cmd, int i_cmd)
{
	t_list	*temp;

	temp = token->next;
	token = ft_lstretrieve(&token, token);
	ft_lstadd_back(&(cmd[i_cmd].params), token);
	return (temp);
}

t_list	*ft_preprocess_redirection(t_list *token, t_cmd *cmd, int i_cmd)
{
	t_list	*temp;

	free(token->content);
	token->content = token->next->content;
	ft_lstremove(&token, token->next, NULL);
	temp = token->next;
	token = ft_lstretrieve(&token, token);
	ft_lstadd_back(&(cmd[i_cmd].redir), token);
	return (temp);
}

void	ft_preprocess_cmd(t_cmd *cmds, t_list *token)
{
	t_list	*temp;
	int		i_cmd;

	i_cmd = 0;
	while (token)
	{
		if (token->type == PARAMETER)
			token = ft_preprocess_parameter(token, cmds, i_cmd);
		else if (6 <= token->type && token->type <= 9)
			token = ft_preprocess_redirection(token, cmds, i_cmd);
		else
		{
			i_cmd++;
			temp = token->next;
			ft_lstdelone(token, &free);
			token = temp;
			token->prev = NULL;
		}
	}
}
