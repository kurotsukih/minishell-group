/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_make_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akalimol <akalimol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:25:54 by akalimol          #+#    #+#             */
/*   Updated: 2023/06/01 18:44:54 by akalimol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*ft_find_next_parenthesis(t_list *first)
{
	t_list	*next_p;
	int		count_p;

	count_p = 1;
	next_p = first->next;
	while (next_p)
	{
		if (next_p->type == RIGHT_P && count_p == 1)
			break ;
		if (next_p->type == RIGHT_P)
			count_p--;
		if (next_p->type == LEFT_P)
			count_p++;
		next_p = next_p->next;
	}
	return (next_p);
}

//    ret: 1 = cleaned, 0 = no
static int	ft_clean_onion(t_list **token)
{
	t_list	*first;
	t_list	*i_token;
	int		res;

	first = *token;
	res = 0;
	while (first->type == LEFT_P)
	{
		i_token = ft_find_next_parenthesis(first);
		if (!i_token->next)
		{
			res = 1;
			i_token->prev->next = NULL;
			free(i_token);
			*token = (*token)->next;
			free(first);
			first = *token;
			first->prev = NULL;
		}
		else
			break ;
	}
	return (res);
}

// Before adding the: n->elems = token; i have to remove all onion layers
t_node	*ft_treenode_new(t_list *t, t_node *parent, int type)
{
	t_node	*n;
	int		is_micro;

	n = (t_node *)malloc(sizeof(t_node));
	if (!n)
		return (NULL);
	is_micro = ft_clean_onion(&t);
	if (!parent)
		n->is_micro = 0;
	else if (is_micro == 1)
		n->is_micro = parent->is_micro + 1;
	else
		n->is_micro = parent->is_micro;
	n->elems = t;
	n->parent = parent;
	n->left = NULL;
	n->right = NULL;
	n->type = type;
	n->cmds = NULL;
	n->count_cmd = 0;
	n->exit_code = 0;
	return (n);
}

void	ft_cut_token(t_list *token)
{
	t_list	*temp;

	if (token)
	{
		temp = token->prev;
		if (temp)
			temp->next = NULL;
		token->prev = NULL;
	}
}

t_list	*ft_retrieve_left_tokens(t_list **t)
{
	t_list	*left;
	int		p_count;

	p_count = 0;
	left = *t;
	while (*t && (*t)->type != OR && (*t)->type != AND)
	{
		if ((*t)->type == LEFT_P)
		{
			p_count = 1;
			*t = (*t)->next;
			while ((*t)->type != RIGHT_P || p_count != 1)
			{
				if ((*t)->type == LEFT_P)
					p_count++;
				if ((*t)->type == RIGHT_P)
					p_count--;
				*t = (*t)->next;
			}
		}
		*t = (*t)->next;
	}
	ft_cut_token(*t);
	return (left);
}

t_list	*ft_retrieve_right_tokens(t_list **token)
{
	t_list	*right;

	if (!(*token))
		return (NULL);
	right = (*token)->next;
	if (right)
		right->prev = NULL;
	(*token)->next = NULL;
	return (right);
}

// where I am saying if the left or right side is the whole
// Also, the whole is not working!
t_node	*make_tree(t_list *token, t_node *parent)
{
	t_node	*n;
	t_list	*left;
	t_list	*right;
	int		is_micro;

	is_micro = ft_clean_onion(&token);
	left = ft_retrieve_left_tokens(&token);
	if (!token)
		return (ft_treenode_new(left, parent, 0));
	right = ft_retrieve_right_tokens(&token);
	n = ft_treenode_new(token, parent, 1);
	if (!n)
		return (NULL);
	if (!parent)
		n->is_micro = 0;
	else
		n->is_micro = parent->is_micro + is_micro;
	n->left = make_tree(left, n);
	if (!n->left)
		return (ft_clean_tree(n), NULL);
	n->right = make_tree(right, n);
	if (!n->right)
		return (ft_clean_tree(n), NULL);
	return (n);
}
