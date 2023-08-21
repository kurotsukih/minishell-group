/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akalimol <akalimol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 14:42:20 by akalimol          #+#    #+#             */
/*   Updated: 2023/06/01 19:08:22 by akalimol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_check_parenthesis_left(t_list *n, int *p_count)
{
	(*p_count)++;
	if (!n->next)
		return ("newline");
	if (n->next->type == RIGHT_P)
		return ((char *)n->next->content);
	if (n->prev && n->prev->type == PARAM)
		return ((char *)n->next->content);
	if (n->prev && (n->prev->type == FILENAME || n->prev->type == PIPE))
		return ((char *)n->content);
	if (n->next->type == PIPE || n->next->type == OR || n->next->type == AND)
		return ((char *)n->next->content);
	return (NULL);
}

char	*ft_check_parenthesis_right(t_list *n, int *p_count)
{
	(*p_count)--;
	if (*p_count == -1)
		return ((char *)n->content);
	if (!n->prev)
		return ((char *)n->content);
	if (n->next && n->next->type == LEFT_P)
		return ((char *)n->next->content);
	if (n->next && n->next->type !=OR && n->next->type != AND && n->next->type != RIGHT_P)
		return ((char *)n->next->content);
	if (n->next && n->next->type == PIPE)
		return ((char *)n->next->content);
	return (NULL);
}

char	*ft_check_parenthesis(t_list *n, int *p_count)
{
	if (n->type == LEFT_P)
		return (ft_check_parenthesis_left(n, p_count));
	else
		return (ft_check_parenthesis_right(n, p_count));
	return (NULL);
}
char	*ft_check_operator2(t_list *n)
{
	if (n->prev == NULL)
		return ((char *)n->content);
	if (n->next == NULL)
		return ("newline");
	if (n->next->type == PIPE || n->next->type == OR || n->next->type == AND)
		return ((char *)n->next->content);
	if (n->next->type == RIGHT_P)
		return ((char *)n->next->content);
	if (n->type == PIPE && n->next->type == LEFT_P)
		return (n->next->content);
	return (NULL);
}

char	*ft_check_redirection(t_list *n)
{
	if (!n->next)
		return ("newline");
	if (n->next->type != FILENAME)
		return ((char *)n->next->content);
	return (NULL);
}

int	check_tokens(t_list *n)
{
	char	*error;
	int		p_count;

	p_count = 0;
	error = NULL;
	while (n)
	{
		if (n->type == PIPE || n->type == OR || n->type == AND )
			error = ft_check_operator2(n);
		else if (n->type == LEFT_P || n->type == RIGHT_P)
			error = ft_check_parenthesis(n, &p_count);
		else if (n->type == REDIR_IN || n->type == REDIR_OUT ||  n->type == HEREDOC || n->type == REDIR_OUT2)
			error = ft_check_redirection(n);
		if (error)
			return (exit_(-1, "bash: syntax err unexpected token `%s'\n", error, NULL, NULL, NULL), 0);
		n = n->next;
	}
	if (p_count != 0)
		return (exit_(-1, "bash: unclosed parenthesis\n", NULL, NULL, NULL, NULL), 0);
	return (1);
}
