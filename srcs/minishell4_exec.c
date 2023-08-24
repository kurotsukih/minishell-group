#include "minishell.h"

void	exec_echo(t_list *cmd)
{
	int	option_n;
	int	i;

	option_n = 0;
	i = -1;
	while (++i < cmd->nb_args)
	{
		printf("%s", cmd->args[i]);
		if (i != cmd->nb_args - 1)
			printf(" ");
		if (cmd->args[i][0] == '-' && cmd->args[i][1] == 'n' && cmd->args[i][2] == '\0')
			option_n = 1;
	}
	if (option_n == 0)
		printf("\n");
}

void	exec_env(t_env **env)
{
	t_env	*cur;

	cur = *env;
	while (cur != NULL)
	{
		printf("%s=%s\n", cur->key, cur->val);
		cur = cur->nxt;
	}
}

void	exec_pwd(void)
{
	char	*s;

	s = getcwd(NULL, 0);
	printf("%s\n", s);
	free(s);
}

int	exec_cd(t_list *cmd, t_env **env)
{
	char	*home_dir;
	t_env	*cur;

	if (cmd->nb_args == 0)
	{
		home_dir = NULL;
		cur = *env;
		while (cur != NULL)
		{
			if (ft_strncmp(cur->key, "HOME", 4) == 0)
				home_dir = cur->val;
			cur = cur->nxt;
		}
		if (home_dir == NULL)
			return (-1); // "bash: cd: HOME not set\n"
		if (chdir(home_dir) == -1)
			return (-1); // "bash: cd: HOME not set properly%s\n", home_dir);
	}
	else if (cmd->nb_args > 1)
		return (-1); // "bash: cd: Too many arguments\n" 
	else if (chdir(cmd->args[0]) == -1)
		return (-1);
	return (0);
}

int	exec_cmds(t_list **l, t_env **env)
{
	t_list *cmd;
	int	result;

	cmd = *l;
	while (cmd != NULL)
	{
		result = 0;
		if (ft_strcmp(cmd->cmd, "echo") == 0)
			exec_echo(cmd);
		else if (ft_strcmp(cmd->cmd, "env") == 0)
			exec_env(env);
		else if (ft_strcmp(cmd->cmd, "pwd") == 0)
			exec_pwd();
		else if (ft_strcmp(cmd->cmd, "cd") == 0)
			result = exec_cd(cmd, env);
		else if (ft_strcmp(cmd->cmd, "export") == 0)
		{
			result = exec_export(cmd, env); //// & ?
		}
		// else if (ft_strcmp(cmd->cmd, "unset") == 0)
		// 	exec_unset(&d->env, cmd->params->next);
		// else if (ft_strcmp(cmd->cmd, "exit") == 0)
		// 	result = exec_exit(d, n, cmd->params->next);
		cmd = cmd->nxt;
	}
	return (result);
}
