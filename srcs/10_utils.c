#include "headers.h"

int	init_new_cmd(t_list **new)
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

void print_cmd(t_list *cmd)
{
	int	i;

	if (cmd == NULL)
	{
		printf("  cmd = NULL\n");
		return ;
	}
	printf("  %p [%s] %d agrs : ", cmd, cmd->cmd, cmd->nb_args);
	i = 0;
	if (cmd->args != NULL)
		while (i < cmd->nb_args)
			printf("[%s] ", cmd->args[i++]);
	printf(": [%s]\n", cmd->redirect);
	(void)i;
}

void print_list(t_list **l)
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
