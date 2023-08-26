#include "headers.h"

int	init_new_cmd(t_cmds **new, char *redirect)
{
	*new = (t_cmds *)malloc(sizeof(t_cmds));
	if (*new == NULL)
		return (-1);
	(*new)->nxt = NULL;
	(*new)->prv = NULL;
	(*new)->nb_args = 0;
	(*new)->args = NULL;
	(*new)->redirect = redirect;
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
	printf("  %p [%s] %d agrs : ", cmd, cmd->args[0], cmd->nb_args);
	if (cmd->args != NULL)
	{
		i = -1;
		while (++i < cmd->nb_args)
			printf("[%s] ", cmd->args[i]);
	}
	printf(": [%s]\n", cmd->redirect);
	(void)i;
}

void print_list(t_cmds **cmds)
{
	t_cmds	*cmd;

	printf("LIST %14p:\n", cmds);
	if (cmds == NULL || *cmds == NULL)
	{
		printf("  empty\n");
		return ;
	}
	cmd = *cmds;
	while (cmd != NULL)
	{
		print_cmd(cmd);
		cmd = cmd->nxt;
	}
}

int len_list(t_env **env)
{
	t_env	*var;
	int		len;

	len = 0;
	var = *env;
	while (var != NULL)
	{
		len ++;
		var = var->nxt;
	}
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

