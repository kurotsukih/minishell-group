#include "headers.h"

void	exec_echo(t_cmds *cmd)
{
	int	option_n;
	int	i;

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

	s = getcwd(NULL, 0);
	printf("%s\n", s);
	free(s);
}

int	exec_cd(t_cmds *cmd, t_env **env)
{
	char	*home_dir;

	// printf("exec cd %s %s %s\n", cmd->args[0], )
	if (cmd->nb_args >= 3)
		return (-1); // "bash: cd: Too many arguments\n" 
	if (cmd->nb_args == 1)
	{
		home_dir = get_value_from_env("HOME", env);
		if (home_dir == NULL)
			return (-1); // "bash: cd: HOME not set\n"
		if (chdir(home_dir) == -1)
			return (-1); // "bash: cd: HOME not set properly%s\n", home_dir);
	}
	else if (chdir(cmd->args[1]) == -1)
		return (-1);
	return (0);
}

// int	exec_exit(t_data *d, t_node *n, t_cmd *cmd)
// {
// 	char	*str;

// 	if (!cmd)
// 		return (exit(d->exit_code), 0);
// 	if (ft_isnum(cmd->cmd) != 1)
// 	{
// 		exit_(-1, "bash: exit: %s: numeric argument required");
// 		return (exit(2), 0);
// 	}
// 	if (ft_lstsize(cmd) > 1)
// 		return (exit_(-1, "bash: exit: too many arguments\n"), 1);
// 	return (exit(ft_atoi(str)), 0); // ft_abs(ft_atoi(str) % 256)
// }

int	exec_cmds(t_cmds **l, t_env **env)
{
	t_cmds *cmd;
	int	result;

	cmd = *l;
	while (cmd != NULL)
	{
		result = 0;
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1 || dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			{} //exit_();
		if (ft_strcmp(cmd->args[0], "echo") == 0)
			exec_echo(cmd);
		else if (ft_strcmp(cmd->args[0], "env") == 0 && env != NULL)
			exec_env(env);
		else if (ft_strcmp(cmd->args[0], "pwd") == 0)
			exec_pwd();
		else if (ft_strcmp(cmd->args[0], "cd") == 0)
			result = exec_cd(cmd, env);
		else if (ft_strcmp(cmd->args[0], "export") == 0)
			result = exec_export(cmd, &env);
		else if (ft_strcmp(cmd->args[0], "unset") == 0)
			exec_unset(cmd, &env);
		// else if (ft_strcmp(cmd->args[0], "exit") == 0)
		// 	result = exec_exit(d, n, cmd->params->next);
		// else
		// 	exec_external_cmd(cmd, env);
		cmd = cmd->nxt;
	}
	return (result);
}
