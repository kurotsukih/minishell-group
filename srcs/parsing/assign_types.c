/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_assign_types.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akalimol <akalimol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 15:28:25 by akalimol          #+#    #+#             */
/*   Updated: 2023/06/01 20:00:34 by akalimol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_assign_type(t_list *n, int is_filename)
{
	if (!ft_strcmp(n->content, "|") && n->type == INDEF)
		n->type = PIPE;
	else if (!ft_strcmp(n->content, "||") && n->type == INDEF)
		n->type = OR;
	else if (!ft_strcmp(n->content, "&&") && n->type == INDEF)
		n->type = AND;
	else if (!ft_strcmp(n->content, "(") && n->type == INDEF)
		n->type = LEFT_P;
	else if (!ft_strcmp(n->content, ")") && n->type == INDEF)
		n->type = RIGHT_P;
	else if (!ft_strcmp(n->content, "<") && n->type == INDEF)
		n->type = REDIR_IN;
	else if (!ft_strcmp(n->content, "<<") && n->type == INDEF)
		n->type = HEREDOC;
	else if (!ft_strcmp(n->content, ">") && n->type == INDEF)
		n->type = REDIR_OUT;
	else if (!ft_strcmp(n->content, ">>") && n->type == INDEF)
		n->type = REDIR_OUT2;
	else if (is_filename == 1 && n->type == INDEF)
		n->type = FILENAME; // не используется
	else
		n->type = PARAM;
}

//    Go through each value and assign the values
//    if I see <. then no matter what the next token is infile
void	assign_types(t_list *n)
{
	int	is_filename;

	is_filename = 0;
	while (n)
	{
		ft_assign_type(n, is_filename);
		is_filename = 0;
		if (n->type == REDIR_IN || n->type == REDIR_IN || n->type == HEREDOC || n->type == REDIR_OUT2)
			is_filename = 1;
		n = n->next;
	}
}
