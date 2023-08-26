#include "headers.h"

int	init_new_cmd(t_cmds **new)
{
	*new = (t_cmds *)malloc(sizeof(t_cmds));
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

void print_cmd(t_cmds *cmd)
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

void print_list(t_cmds **l)
{
	t_cmds	*cur;

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

int len_list(t_env **env)
{
	t_env	*var;
	int		len;

	len = 0;
	var = *env;
	while (var != NULL)
		len ++;
	return (len);
}

char	*get_value_from_env(char *key, t_env **env)
{
	t_env	*var;

	var = *env;
	while (var != NULL)
	{
		if (ft_strcmp(key_(var->var), key) == 0)
			return (val_(var->var));
		var = var->nxt;
	}
	return (NULL);
}

