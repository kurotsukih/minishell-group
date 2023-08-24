#include "headers.h"

void	exec_env(t_env **env)
{
	t_env	*var;

	var = *env;
	while (var != NULL)
	{
		printf("%s=%s\n", var->key, var->val);
		var = var->nxt;
	}
}

void	exec_unset(t_list *cmd, t_env ***env)
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
			if (ft_strcmp(var->key, part_before_sign_equal(cmd->args[i])) == 0)
			{
				free(var->key);
				free(var->val);
				if (prv != NULL)
					prv->nxt = var->nxt;
				else if (**env == var)
					**env = var->nxt;
				free(var);
			}
			prv = var;
			var = var->nxt;
		}
	}
}

int	exec_export(t_list *cmd, t_env ***env)
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
		new->key = part_before_sign_equal(cmd->args[i]);
		new->val = part_after_sign_equal(cmd->args[i]);
		new->nxt = **env;
		**env = new;
	}
	return (0);
}
