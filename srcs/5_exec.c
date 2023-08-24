#include "minishell.h"

// static void	ft_wait_child_processes(int *is_success, int size, int pid)
// {
// 	int	i;
// 	int	status;
// 	i = 0;
// 	while (i < size)
// 	{
// 		if (wait(&status) == pid)
// 		{
// 			if (WIFEXITED(status))
// 				*is_success = WEXITSTATUS(status);
// 			if (WIFSIGNALED(status))
// 			{
// 				*is_success = WTERMSIG(status) + 128;
// 				if (*is_success == 130)
// 					exit_(-1, "\n", NULL, NULL, NULL, NULL);
// 				if (*is_success == 131)
// 					exit_(-1, "Quit (core dumped)\n", NULL, NULL, NULL, NULL);
// 			}
// 		}
// 		i++;
// 	}
// }

// // Add a moment when there can be only one command and it is builtin
// int	ft_execute(t_cmd *cmd, t_data *d, t_node *n)
// {
// 	int	pid;
// 	int	exit_c;

// 	pid = fork();
// 	exit_c = 0;
// 	if (pid < -1)
// 		exit_(-1, NULL, NULL, NULL, NULL, NULL);
// 	if (pid == 0)
// 	{
// 		signal(SIGINT, &sig_handler_fork);
// 		signal(SIGQUIT, &sig_handler_fork);
// 		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
// 			exit_(-1, NULL, NULL, NULL, NULL, NULL);
// 		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
// 			exit_(-1, NULL, NULL, NULL, NULL, NULL);
// 		ft_clean_fds(cmd);
// 		if (cmd->params && ft_is_builtin(cmd->params) == 1)
// 			exit_c = ft_execute_builtin(cmd, d, n);
// 		else if (cmd->params)
// 			exit_c = ft_execute_program(cmd, d->env, n);
// 		return (ft_clean_tree(n), free_redirections(*(&(d->env))), exit(exit_c), 0);
// 	}
// 	else if (cmd->in_fd != 0)
// 		close(cmd->in_fd);
// 	return (signal(SIGINT, SIG_IGN), pid);
// }

// int	ft_exec_command(t_node *n, t_data *d)
// {
// 	int	i_cmd;
// 	int	num;
// 	int	pid;
// 	int	result;

// 	num = 0;
// 	result = -1;
// 	i_cmd = 0;
// 	pid = 0;
// 	while (i_cmd < n->count_cmd)
// 	{
// 		result = ft_prepare_pipe(n, i_cmd);
// 		if (check(&(n->cmds[i_cmd]), n->count_cmd, result) && num++ >= 0)
// 			pid = ft_execute(&n->cmds[i_cmd], d, n);
// 		else if (result == 0 && n->count_cmd == 1)
// 			result = ft_execute_builtin(&n->cmds[i_cmd], d, n);
// 		i_cmd++;
// 	}
// 	ft_wait_child_processes(&result, num, pid);
// 	signal(SIGINT, &sig_handler_fork); // mb sig_handler_mai n
// 	return (result);
// }

////////////////////////////////////////////////////////////////

// //  I have to think of exit and free strategy
// int	ft_execute_program(t_cmd *cmd, t_list *env, t_node *n)
// {
// 	char	*path;
// 	char	**params;
// 	int		code;
// 	path = NULL;
// 	if (!ft_strcmp((char *)cmd->params->content, "."))
// 		return (exit_(-1, "bash: .: filename arg required\n", NULL, NULL, NULL, NULL), -1);
// 	code = ft_find_path(cmd->params->content, env, &path);
// 	if (!path)
// 		return (code);
// 	params = ft_construct_command(cmd->params);
// 	if (!params)
// 		return (free(path), 255);
// 	execve(path, params, ft_construct_command(env));
// 	free(params);
// 	free(path);
// 	path = (char *)cmd->params->content;
// 	if (ft_strchr(path, '/'))
// 		exit_(-1, "bash: %s: no such file or directory\n", path, NULL, NULL, NULL);
// 	else
// 		exit_(-1, "bash: %s: command not found\n", path, NULL, NULL, NULL);
// 	return (code);
// }
