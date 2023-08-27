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

int	exec_cd(t_cmds *cmd, t_data **d)
{
	char	*home_dir;

	if (cmd->nb_args >= 3)
		return ((*d)->err = "bash: cd: Too many arguments", -1);
	if (cmd->nb_args == 1)
	{
		home_dir = get_value_from_env("HOME", d);
		if (home_dir == NULL)
			return ((*d)->err = "bash: cd: HOME not set", -1);
		if (chdir(home_dir) == -1)
			return ((*d)->err = "bash: cd: HOME not set properly", -1);
	}
	else if (chdir(cmd->args[1]) == -1)
		return ((*d)->err = "...", -1);
	return (0);
}

// int	exec_exit(t_node *n, t_cmd *cmd, t_data **d)
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

int	exec_cmds(t_data **d)
{
	t_cmds *cmd;
	int	result;

	cmd = *((*d)->cmds);
	while (cmd != NULL)
	{
		result = 0;
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1 || dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			exit_(d);
		if (ft_strcmp(cmd->args[0], "echo") == 0) /// func name as variable
			exec_echo(cmd);
		else if (ft_strcmp(cmd->args[0], "env") == 0 && (*d)->env != NULL)
			exec_env(d);
		else if (ft_strcmp(cmd->args[0], "pwd") == 0)
			exec_pwd();
		else if (ft_strcmp(cmd->args[0], "cd") == 0)
			result = exec_cd(cmd, d);
		else if (ft_strcmp(cmd->args[0], "export") == 0)
			result = exec_export(cmd, d);
		else if (ft_strcmp(cmd->args[0], "unset") == 0 && (*d)->env != NULL)
			exec_unset(cmd, d);
		// else if (ft_strcmp(cmd->args[0], "exit") == 0)
		// 	result = exec_exit(d, n, cmd->params->next);
		else
			exec_extern_cmd(cmd, d);
		cmd = cmd->nxt;
	}
	return (result);
}
