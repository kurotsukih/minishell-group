/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clean_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akalimol <akalimol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 15:30:59 by akalimol          #+#    #+#             */
/*   Updated: 2023/07/08 19:33:36 by akalimol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_clean_fds(t_cmd *cmd)
{
	if (cmd->in_fd != -1 && cmd->in_fd != 0)
		close(cmd->in_fd);
	if (cmd->out_fd != -1 && cmd->out_fd != 1)
		close(cmd->out_fd);
	if (cmd->out_pipe_fd != -1)
		close(cmd->out_pipe_fd);
}

static void	ft_clean_cmds(t_cmd *cmds_p, int size)
{
	t_cmd	*cmds;
	int		i;

	cmds = cmds_p;
	i = 0;
	while (i < size)
	{
		if (cmds[i].params)
			ft_lstclear(&cmds[i].params, &free);
		cmds[i].params = NULL;
		if (cmds[i].redir)
			free_redirections(cmds[i].redir);
		cmds[i].redir = NULL;
		ft_clean_fds(&cmds[i]);
		i++;
	}
	free(cmds);
}

void	ft_clean_tree(t_node *node)
{
	if (!node)
		return ;
	while (node->parent)
		node = node->parent;
	if (node->left)
		ft_clean_tree(node->left);
	if (node->right)
		ft_clean_tree(node->right);
	ft_lstclear(&node->elems, &free);
	node->elems = NULL;
	if (node->cmds)
	{
		ft_clean_cmds(node->cmds, node->count_cmd);
		node->cmds = NULL;
	}
	free(node);
}

