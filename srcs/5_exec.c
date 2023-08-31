#include "headers.h"

void	exec_echo(t_cmds *cmd)
{
	int	option_n;
	int	i;

	// write(STDOUT_FILENO, "write echo\n", 12);
	option_n = 0;
	i = 0;
	while (++i < cmd->nb_args)
	{
		if (ft_strcmp(cmd->args[i], "-n") == 0)
			option_n = 1;
		else if (i == cmd->nb_args - 1)
			printf("%s", cmd->args[i]);
		else
			printf("%s ", cmd->args[i]);
	}
	if (option_n == 0)
		printf("\n");
}

void	exec_pwd(void)
{
	char	*s;

	// write(STDOUT_FILENO, "write pwd\n", 11);
	s = getcwd(NULL, 0);
	printf("%s\n", s);
	free(s);
}

void	exec_cd(t_cmds *cmd, t_data **d)
{
	char	*dir;

	if (cmd->nb_args == 0)
		return;
	if (cmd->nb_args == 1)
	{
		dir = get_value_from_env("HOME", d);
		if (dir == NULL)
		{
		//  free_all_and_go_to_next_cmd("cd " , exit_code = )
			return ;
		}
	}
	if (cmd->nb_args > 1)
		dir = cmd->args[1];
	if (chdir(dir) == -1)
	{
		//  free_all_and_go_to_next_cmd("cd " , exit_code = )
	}
	if(dir != NULL)
		free(dir);
}

void	exec_exit(t_data **d)
{
	//(*d)->exit_c = 
	free_all_and_exit("", d);
}

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
			if (ft_strcmp(key_(var->var, d), cmd->args[i]) == 0)
			{
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

void	exec_export(t_cmds *cmd, t_data **d)
{
	t_env	*new_var;
	int		i;

	if (cmd->nb_args == 0)
		exec_env(d);
	exec_unset(cmd, d);
	i = 0;
	while (++i < cmd->nb_args)
	{
		new_var = (t_env *)malloc_(sizeof(t_env), d);
		new_var->var = ft_strdup(cmd->args[i]);
		new_var->nxt = *((*d)->env);
		*((*d)->env) = new_var;
	}
}
