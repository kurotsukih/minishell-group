#include "minishell.h"

t_list	*ft_lstnew()
{
	t_list	*node;

	node = (t_list *)malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->nxt = NULL;
	node->prv = NULL;
	return (node);
}

int	init_list(t_list ***l)
{
	*l = (t_list **)malloc(sizeof(t_list *));
	if (*l == NULL)
		return (-1);
	**l = NULL;
	return (0);
}

int	init_new_elt(t_list **new)
{
	*new = (t_list *)malloc(sizeof(t_list));
	if (*new == NULL)
		return (-1);
	(*new)->nxt = NULL;
	(*new)->prv = NULL;
	(*new)->cmd = NULL;
	(*new)->args = NULL;
	(*new)->nb_args = 0;
	(*new)->redirect = NULL;
	(*new)->is_filename = 0;
	return (0);
}

int	put_cmd_and_redirect_to_l(t_list **l, char *cmd, int len_cmd, char *redirect)
{
	t_list	*new;
	t_list	*cur;
	int	i;

	if (init_new_elt(&new) == -1)
		return (-1);
	new->redirect = redirect;
	new->cmd = (char *)malloc(len_cmd + 1);
	if (new->cmd == NULL)
		return (-1);
	i = -1;
	while (++i < len_cmd)
		new->cmd[i] = cmd[i];
	new->cmd[i] = '\0';
	if (*l == NULL)
		*l = new;
	else
	{
		cur = *l;
		while (cur != NULL && cur->nxt != NULL)
			cur = cur->nxt;
		cur->nxt = new;
		new->prv = cur;
	}
	return (0);
}

void print_cmd(t_list *cmd) /// ft_printf
{	
	if (cmd == NULL)
	{
		printf("  cmd = NULL\n");
		return ;
	}
	printf("  %p: [%s] %d args [%s]\n", cmd, cmd->cmd, cmd->nb_args, cmd->redirect);
}

void print_list(t_list **l) /// ft_printf
{
	t_list	*cur;

	printf("LIST %14p:\n", l);
	if (l == NULL || *l == NULL)
	{
		printf("  empty\n");
		return ;
	}
	cur = *l;
	while (cur != NULL)
	{
		print_cmd(cur);
		cur = cur->nxt;
	}
}

void	ft_lstadd_front(t_list **lst, t_list *new_node)
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

void	ft_lstadd_back(t_list **lst, t_list *new_node)
{
	t_list	*lastnode;

	lastnode = ft_lstlast(*lst);
	if (!lastnode)
		*lst = new_node;
	else
	{
		lastnode->nxt = new_node;
		new_node->prv = lastnode;
	}
}

int	ft_lstsize(t_list *lst)
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

t_list  *ft_lstretrieve(t_list **lst, t_list *node)
{
    t_list  *i_token;

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

int ft_lstremove(t_list **lst, t_list *node)
{
    t_list  *i_token;

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

void	ft_lstclear(t_list **lst)
{
	t_list	*temp;

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

void	ft_lstdelone(t_list *lst)
{
	if (lst)
	{
		free(lst);
	}
}
