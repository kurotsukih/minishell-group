#include "headers.h"

int	args_are_correct(t_cmds *cmd, t_data **d)
{
	if (!cmd->args)
	{
		(*d)->exit_c = 255;
		return (0);
	}
	if (cmd->nb_args > cmd->nb_args_max)
	{
		printf("%s: Too many arguments", cmd->args[0]);
		// d->exit_code = 
		return (0);
	}
	if (ft_strcmp(cmd->args[0], "exit") == 0 && !ft_atoi(cmd->args[1])) ////
	{
		printf("exit: numeric argument required");
		(*d)->exit_c =2;
		return (0);
	}
	if (there_are_unclosed_quotes(cmd))
	{
		printf("%s: unclosed quotes", cmd->args[0]);
		// d->exit_code = 
		return (0);
	}
	return (1);
}

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

void	exec_cd(t_cmds *cmd, t_data **d)
{
	char	*dir;

	if (cmd->nb_args == 1)
	{
		dir = get_value_from_env("HOME", d);
		if (dir == NULL)
		{
			printf("cd: ...\n");
			//(*d)->exit_c = ;
			return ;
		}
	}
	else
		dir = cmd->args[1];
	if (chdir(dir) == -1)
	{
		printf("cd: ...");
		//(*d)->exit_c = ;
	}
	if(dir != NULL)
		free(dir);
}

void	exec_exit(t_data **d)
{
	//(*d)->exit_c = 
	exit_(d);
}

int	exec_cmds(t_data **d)
{
	t_cmds *cmd;
	int	result;

	cmd = *((*d)->cmds);
	while (cmd != NULL)
	{
		if (!args_are_correct(cmd, d))
			continue; //?
		result = 0;
		if (ft_strcmp(cmd->args[0], "echo") == 0)
			exec_echo(cmd);
		else if (ft_strcmp(cmd->args[0], "env") == 0 && (*d)->env != NULL)
			exec_env(d);
		else if (ft_strcmp(cmd->args[0], "pwd") == 0)
			exec_pwd();
		else if (ft_strcmp(cmd->args[0], "cd") == 0)
			exec_cd(cmd, d);
		else if (ft_strcmp(cmd->args[0], "export") == 0)
			result = exec_export(cmd, d);
		else if (ft_strcmp(cmd->args[0], "unset") == 0 && (*d)->env != NULL)
			exec_unset(cmd, d);
		else if (ft_strcmp(cmd->args[0], "exit") == 0)
			exec_exit(d);
		else
			exec_extern_cmd(cmd, d);
		cmd = cmd->nxt;
	}
	return (result);
}
