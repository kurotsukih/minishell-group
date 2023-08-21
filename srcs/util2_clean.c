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

static void	ft_clean_cmds(t_cmd *cmds, int size)
{
	t_cmd	*cmd;
	int		i;

	cmd = cmds;
	i = 0;
	while (i < size)
	{
		if (cmd[i].params)
			ft_lstclear(&cmd[i].params, &free);
		cmd[i].params = NULL;
		if (cmd[i].redir)
			free_redirections(cmd[i].redir);
		cmd[i].redir = NULL;
		ft_clean_fds(&cmd[i]);
		i++;
	}
	free(cmd);
}

void	ft_clean_tree(t_node *n)
{
	if (!n)
		return ;
	while (n->parent)
		n = n->parent;
	if (n->left)
		ft_clean_tree(n->left);
	if (n->right)
		ft_clean_tree(n->right);
	ft_lstclear(&n->elems, &free);
	n->elems = NULL;
	if (n->cmds)
	{
		ft_clean_cmds(n->cmds, n->count_cmd);
		n->cmds = NULL;
	}
	free(n);
}

