#include "minishell.h"

t_cmd	*ft_lstnew()
{
	t_cmd	*node;

	node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->nxt = NULL;
	node->prv = NULL;
	return (node);
}

void	ft_lstadd_front(t_cmd **lst, t_cmd *new_node)
{
	if (lst)
	{
		if (*lst)
		{
			new_node->nxt = *lst;
			(*lst)->prv = new_node;
		}
		*lst = new_node;
	}
}

void	ft_lstadd_back(t_cmd **lst, t_cmd *new_node)
{
	t_cmd	*lastnode;

	lastnode = ft_lstlast(*lst);
	if (!lastnode)
		*lst = new_node;
	else
	{
		lastnode->nxt = new_node;
		new_node->prv = lastnode;
	}
}

int	ft_lstsize(t_cmd *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->nxt;
		i++;
	}
	return (i);
}

t_cmd  *ft_lstretrieve(t_cmd **lst, t_cmd *node)
{
    t_cmd  *i_token;

    i_token = *lst;
    while (i_token && i_token != node)
        i_token = i_token->nxt;
    if (i_token)
    {
        if (i_token->prv)
            i_token->prv->nxt = i_token->nxt;
        else
            *lst = i_token->nxt;
        if (i_token->nxt)
            i_token->nxt->prv = i_token->prv;
        i_token->nxt = NULL;
        i_token->prv = NULL;
        return (i_token);
    }
    return (NULL);
}

int ft_lstremove(t_cmd **lst, t_cmd *node)
{
    t_cmd  *i_token;

    i_token = *lst;
    while (i_token && i_token != node)
        i_token = i_token->nxt;
    if (i_token)
    {
        if (i_token->prv)
            i_token->prv->nxt = i_token->nxt;
        else
            *lst = i_token->nxt;
        if (i_token->nxt)
            i_token->nxt->prv = i_token->prv;
        ft_lstdelone(i_token);
        return (1);
    }
    return (0);
}

void	ft_lstclear(t_cmd **lst)
{
	t_cmd	*temp;

	if (lst)
	{
		temp = *lst;
		while (temp)
		{
			temp = (*lst)->nxt;
			ft_lstdelone(*lst);
			*lst = temp;
		}
	}
}

void	ft_lstdelone(t_cmd *lst)
{
	if (lst)
	{
		free(lst);
	}
}
