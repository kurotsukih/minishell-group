#include "headers.h"

int	len_lst(t_lst **lst)
{
	t_lst	*cur;
	int		len;

	if (lst == NULL)
		return (0);
	len = 0;
	cur = *lst;
	while (cur != NULL)
	{
		len++;
		cur = cur->nxt;
	}
	return (len);
}

void	put_to_lst(void *val, t_lst ***lst, t_data **d)
{
	t_lst	*new;
	t_lst	*cur;

	new = (t_lst *)malloc_(sizeof(t_lst), d);
	new->val = val;
	new->nxt = NULL;
	if (*lst == NULL)
	{
		*lst = (t_lst **)malloc_(sizeof(t_lst *), d);
		**lst = new;
		return ;
	}
	cur = **lst;
	while (cur != NULL && cur->nxt != NULL)
		cur = cur->nxt;
	if (cur != NULL)
		cur->nxt = new;
}

t_lst	**arr_to_lst(char **arr, t_data **d)
{
	int		i;
	t_lst **lst;

	i = -1;
	while (1)
	{
		put_to_lst((void *)(arr[++i]), &lst, d);
		if (arr[i] == NULL)
			break ;
	}
	return (lst);
}

char	**lst_to_arr(t_lst **lst, t_data **d)
{
	int		i;
	char	**arr;
	t_lst	*cur;

	if (lst == NULL)
		return (NULL);
	arr = (char **)malloc_((len_lst(lst) + 1) *sizeof(char *), d);
	cur = *lst;
	i = -1;
	while (1)
	{
		arr[++i] = (char *)(cur->val);
		if (cur->val == NULL)
			break ;
		cur = cur->nxt;
	}
	arr[++i] = NULL;
	return (arr);
}

void	del_from_lst(t_lst *to_del, t_lst **lst)
{
	t_lst	*cur;
	t_lst	*prv;

	if (lst == NULL)
		return ;
	cur = *lst;
	prv = NULL;
	while (cur != NULL)
	{
		if (cur == to_del)
			{
				if (prv != NULL)
					prv->nxt = cur->nxt;
				else if (prv == NULL)
					*lst = cur->nxt;
				free_(cur->val);
				free_(cur);
				break ;
			}
		prv = cur;
		cur = cur->nxt;
	}
}

void	dell_all_from_lst(t_lst **lst)
{
	t_lst	*cur;
	t_lst	*to_del;

	if (lst == NULL)
		return ;
	cur = *lst;
	while (cur != NULL)
	{
		to_del = cur;
		del_from_lst(to_del, lst);
		cur = cur->nxt;
	}
}

void	free_lst(t_lst ***lst)
{
	if (*lst == NULL)
		return ;
	dell_all_from_lst(*lst);
	free(*lst);
}
