/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_preprocess.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akadilkalimoldayev <akadilkalimoldayev@    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 12:31:09 by akalimol          #+#    #+#             */
/*   Updated: 2023/06/08 18:20:13 by akadilkalim      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
Preprocess the command. 
Before, I had a node with all tokens in one place. 
Now they are redistributed
 *                            typedef struct s_cmd
 *                            {
 *                                t_list  *params;
 *                                t_list  *redir;
 *                                int     in_fd;
 *                                int     out_fd;
 *                            }           t_cmd;
 * 
 * @param node      head node
 * @return int      0 if everything is ok, -1  if malloc fails
 */

void	ft_init_cmds(t_cmd *cmds, int num_cmds)
{
	int	i_cmd;

	i_cmd = 0;
	while (i_cmd < num_cmds)
	{
		cmds[i_cmd].params = NULL;
		cmds[i_cmd].redir = NULL;
		cmds[i_cmd].in_fd = 0;
		cmds[i_cmd].out_fd = 1;
		cmds[i_cmd].out_pipe_fd = -1;
		i_cmd++;
	}
}

int	ft_count_cmds(t_list *token)
{
	t_list	*i_token;
	int		count;

	count = 1;
	i_token = token;
	while (i_token)
	{
		if (i_token->type == PIPE)
			count++;
		i_token = i_token->next;
	}
	return (count);
}

int	ft_preprocess_node(t_node *node)
{
	node->count_cmd = ft_count_cmds(node->elems);
	node->cmds = (t_cmd *)malloc(sizeof(t_cmd) * node->count_cmd);
	if (!node->cmds)
		return (exit_(-1, NULL, NULL, NULL, NULL, NULL), -1);
	ft_init_cmds(node->cmds, node->count_cmd);
	ft_preprocess_cmd(node->cmds, node->elems);
	node->elems = NULL;
	return (0);
}

int	ft_preprocess(t_node *node)
{
	if (node->left)
	{
		if (ft_preprocess(node->left) == -1)
			return (-1);
		if (ft_preprocess(node->right) == -1)
			return (-1);
	}
	else 
		if (ft_preprocess_node(node) == -1)
			return (-1);
	return (0);
}

