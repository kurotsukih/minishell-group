#include "headers.h"

void	exec_env(t_data **d)
{
	t_env	*var;

	var = *((*d)->env);
	while (var != NULL)
	{
		printf("%s\n", var->var);
		var = var->nxt;
	}
}

void	exec_unset(t_cmds *cmd, t_data **d)
{
	t_env	*var;
	t_env	*prv;
	int		i;

	i = 0;
	while (++i < cmd->nb_args)
	{
		prv = NULL;
		var = *((*d)->env);
		while (var != NULL)
		{
			// printf("unset %s if equal to %s\n", var->var, cmd->args[i]);
			if (ft_strcmp(key_(var->var), cmd->args[i]) == 0)
			{
				// printf("found %s\n", cmd->args[i]);
				// printf("found %s\n", key_(var->var));
				free(var->var);
				if (prv != NULL)
					prv->nxt = var->nxt;
				else if (*((*d)->env) == var)
					*((*d)->env) = var->nxt;
				free(var);
				return ;
			}
			prv = var;
			var = var->nxt;
		}
	}
}

int	exec_export(t_cmds *cmd, t_data **d)
{
	t_env	*new_var;
	int		i;

	if (cmd->nb_args == 0)
		return (exec_env(d), 0);
	exec_unset(cmd, d);
	i = 0;
	while (++i < cmd->nb_args)
	{
		new_var = NULL;
		new_var = (t_env *)malloc(sizeof(t_env));
		if (new_var == NULL)
			exit_(d);
		new_var->var = ft_strdup(cmd->args[i]); ////
		new_var->nxt = *((*d)->env);
		*((*d)->env) = new_var;
	}
	return (0);
}
