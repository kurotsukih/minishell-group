#include "minishell.h"

int	ft_is_builtin(t_list *token)
{
	if (ft_strcmp((char *)token->content, "cd") == 0)
		return (1);
	else if (ft_strcmp((char *)token->content, "pwd") == 0)
		return (1);
	else if (ft_strcmp((char *)token->content, "echo") == 0)
		return (1);
	else if (ft_strcmp((char *)token->content, "export") == 0)
		return (1);
	else if (ft_strcmp((char *)token->content, "unset") == 0)
		return (1);
	else if (ft_strcmp((char *)token->content, "env") == 0)
		return (1);
	else if (ft_strcmp((char *)token->content, "exit") == 0)
		return (1);
	return (0);
}

static void	ft_wait_child_processes(int *is_success, int size, int pid)
{
	int	i;
	int	status;

	i = 0;
	while (i < size)
	{
		if (wait(&status) == pid)
		{
			if (WIFEXITED(status))
				*is_success = WEXITSTATUS(status);
			if (WIFSIGNALED(status))
			{
				*is_success = WTERMSIG(status) + 128;
				if (*is_success == 130)
					exit_(-1, "\n", NULL, NULL, NULL, NULL);
				if (*is_success == 131)
					exit_(-1, "Quit (core dumped)\n", NULL, NULL, NULL, NULL);
			}
		}
		i++;
	}
}

// Add a moment when there can be only one command and it is builtin
int	ft_execute(t_cmd *cmd, t_data *d, t_node *n)
{
	int	pid;
	int	exit_c;

	pid = fork();
	exit_c = 0;
	if (pid < -1)
		exit_(-1, NULL, NULL, NULL, NULL, NULL);
	if (pid == 0)
	{
		signal(SIGINT, &sig_handler_fork);
		signal(SIGQUIT, &sig_handler_fork);
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
			exit_(-1, NULL, NULL, NULL, NULL, NULL);
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			exit_(-1, NULL, NULL, NULL, NULL, NULL);
		ft_clean_fds(cmd);
		if (cmd->params && ft_is_builtin(cmd->params) == 1)
			exit_c = ft_execute_builtin(cmd, d, n);
		else if (cmd->params)
			exit_c = ft_execute_program(cmd, d->env, n);
		return (ft_clean_tree(n), free_redirections(*(&(d->env))), exit(exit_c), 0);
	}
	else if (cmd->in_fd != 0)
		close(cmd->in_fd);
	return (signal(SIGINT, SIG_IGN), pid);
}

int	check(t_cmd *cmd, int count, int result)
{
	char	*str;

	if (result == 0)
	{
		if (count != 1)
			return (1);
		if (!cmd->params)
			return (1);
		if (cmd->params && ft_is_builtin(cmd->params) != 1)
			return (1);
		else if (cmd->params && ft_is_builtin(cmd->params) == 1)
		{
			str = (char *)cmd->params->content;
			if (ft_strcmp(str, "cd") == 0 || ft_strcmp(str, "exit") == 0)
				return (0);
			else if (ft_strcmp(str, "export") == 0 || !ft_strcmp(str, "unset"))
				return (0);
			else
				return (1);
		}
	}
	return (0);
}

int	ft_exec_command(t_node *n, t_data *d)
{
	int	i_cmd;
	int	num;
	int	pid;
	int	result;

	num = 0;
	result = -1;
	i_cmd = 0;
	pid = 0;
	while (i_cmd < n->count_cmd)
	{
		result = ft_prepare_pipe(n, i_cmd);
		if (check(&(n->cmds[i_cmd]), n->count_cmd, result) && num++ >= 0)
			pid = ft_execute(&n->cmds[i_cmd], d, n);
		else if (result == 0 && n->count_cmd == 1)
			result = ft_execute_builtin(&n->cmds[i_cmd], d, n);
		i_cmd++;
	}
	ft_wait_child_processes(&result, num, pid);
	signal(SIGINT, &sig_handler_fork); // mb sig_handler_mai n
	return (result);
}