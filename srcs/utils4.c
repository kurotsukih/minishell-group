#include "minishell.h"

int	ft_isnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]) != 1)
			return (0);
		i++;
	}
	return (1);
}

int	ft_abs(int num)
{
	if (num < 0)
		return (num * -1);
	return (num);
}

static void	ft_swap(t_list *a, t_list *b)
{
	void	*temp;

	temp = a->content;
	a->content = b->content;
	b->content = temp;
}

void	ft_bubble_sort_list(t_list *head)
{
	int		swapped;
	t_list	*ptr1;
	t_list	*ptr2;

	swapped = 1;
	ptr2 = NULL;
	if (head == NULL)
		return ;
	while (swapped)
	{
		swapped = 0;
		ptr1 = head;
		while (ptr1->next != ptr2)
		{
			if (ft_strcmp(ptr1->content, ptr1->next->content) > 0)
			{
				ft_swap(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		ptr2 = ptr1;
	}
}

int	ft_strcmp_alt(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (1);
	i = 1;
	while (str[i] && str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (0);
	return (1);
}

void	*free_charchar(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		free(s[i++]);
	free(s);
	return (NULL);
}

