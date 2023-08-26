#include "headers.h"

void	exec_env(t_env **env)
{
	t_env	*var;

	var = *env;
	while (var != NULL)
	{
		printf("%s\n", var->var);
		var = var->nxt;
	}
}

void	exec_unset(t_cmds *cmd, t_env ***env)
{
	t_env	*var;
	t_env	*prv;
	int		i;

	i = -1;
	while (++i < cmd->nb_args)
	{
		prv = NULL;
		var = **env;
		while (var != NULL)
		{
			printf("unset %s if equal to %s\n", var->var, cmd->args[i]);
			if (ft_strcmp(key_(var->var), cmd->args[i]) == 0)
			{
				printf("found %s\n", cmd->args[i]);
				printf("found %s\n", key_(var->var));
				free(var->var);
				printf("1\n");
				if (prv != NULL)
					prv->nxt = var->nxt;
				else if (**env == var)
					**env = var->nxt;
				printf("2\n");
				free(var);
				return ;
			}
			prv = var;
			var = var->nxt;
		}
	}
}

int	exec_export(t_cmds *cmd, t_env ***env)
{
	t_env	*new;
	int		i;

	if (cmd->nb_args == 0)
		return (exec_env(*env), 0);
	exec_unset(cmd, env);
	i = -1;
	while (++i < cmd->nb_args)
	{
		new = NULL;
		new = (t_env *)malloc(sizeof(t_env));
		if (new == NULL)
			return (-1);
		new->var = cmd->args[i];
		new->nxt = **env;
		**env = new;
	}
	return (0);
}
