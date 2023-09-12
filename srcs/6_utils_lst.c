#include "headers.h"

int	len_lst(t_lst **lst)
{
	t_lst	*cur;
	int		i;

	if (lst == NULL)
		return (0);
	i = 0;
	cur = *lst;
	while (cur != NULL)
	{
		i++;
		cur = cur->nxt;
	}
	return (i);
}

void	put_to_lst(char *val, t_lst ***lst, t_data **d)
{
	t_lst	*new;
	t_lst	*cur;

	//printf("put_to_lst %s\n", val);
	new = (t_lst *)malloc_(sizeof(t_lst), d);
	new->val = val;
	new->nxt = NULL;
	if (*lst == NULL)
	{
		*lst = (t_lst **)malloc_(sizeof(t_lst *), d);
		**lst = NULL;
	}	
	cur = **lst;
	if (cur == NULL)
	{
		**lst = new;
		return ;
	}
	while (cur != NULL && cur->nxt != NULL)
		cur = cur->nxt;
	cur->nxt = new;
}

t_lst	**arr_to_lst(char **arr, t_data **d)
{
	int		i;
	t_lst **lst;

	lst = NULL;
	i = -1;
	while (arr[++i] != NULL)
		put_to_lst(arr[i], &lst, d);
	return (lst);
}

char	**lst_to_arr(t_lst **lst, t_data **d)
{
	char	**arr;
	t_lst	*cur;
	int		len_arr;
	int		i;

	if (lst == NULL)
		return (NULL);
	len_arr = len_lst(lst) + 1;
	arr = (char **)malloc_(len_arr *sizeof(char *), d);
	cur = *lst;
	i = 0;
	while (i < len_arr - 1)
	{
		arr[i] = cur->val;
		cur = cur->nxt;
		i++;
	}
	arr[i] = NULL;
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
				//free_(cur->val);
				free_(cur);
				break ;
			}
		prv = cur;
		cur = cur->nxt;
	}
}

void	del_all_from_lst(t_lst **lst)
{
	t_lst	*cur;
	t_lst	*nxt;

	if (lst == NULL)
		return ;
	cur = *lst;
	while (cur != NULL)
	{
		nxt = cur->nxt;
		del_from_lst(cur, lst);
		cur = nxt;
	}
}

void	free_lst(t_lst ***lst)
{
	if (**lst == NULL || *lst == NULL)
		return ;
	del_all_from_lst(*lst);
	free_(*lst);
}
