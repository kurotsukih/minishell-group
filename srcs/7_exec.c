#include "headers.h"

// exec_command = exec		/ exec_builtin
// exec         = execve	/ exec_builtin
// int	check(t_cmd *cmd, int count, int result)
// {
// 	char	*str;
// 	if (result == 0)
// 	{
// 		if (count != 1 || !cmd->params ||cmd->params && ft_is_builtin(cmd->params) != 1)
// 			return (1);
// 		else if (cmd->params && ft_is_builtin(cmd->params) == 1)
// 		{
// 			str = cmd->params;
// 			if (ft_strcmp(str, "cd") == 0 || ft_strcmp(str, "exit") == 0 || ft_strcmp(str, "export") == 0 || !ft_strcmp(str, "unset"))
// 				return (0);
// 			else
// 				return (1);
// 		}
// 	}
// 	return (0);
// }

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

// Add a moment when there can be only one command and it is builtin
//  exit and free strategy
// int	exec_command(t_node *n)
// {
// 	int		res;
// 	t_list	cmd;
// 	char	*path;
// 	int		exit_c;
// 	char	**args;

// 	res = -1;
// 	cmd = *l;
// 	while (cmd != NULL)
// 	{
// 		res = ft_prepare_pipe(n, cmd);
// 		if (result == 0)
// 			check(cmds);
// 		if ((check(cmds), n->count_cmd, res))
// 		{
// 			exit_c = 0;
// 			if (pid == 0)
// 			{
// 				signal(SIGINT, &sig_handler_fork);
// 				signal(SIGQUIT, &sig_handler_fork);
// 				if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
// 					exit_();
// 				if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
// 					exit_();
// 				if (cmd->params && ft_is_builtin(cmd->params) == 1)
// 					exit_c = exec_builtin(cmd, d, n);
// 				else if (cmd->params)
// 				{
// 					exit_c = exec_program(cmd, d->env, n);
// 					path = NULL;
// 					if (!ft_strcmp(cmd->ars, "."))
// 						return (exit_(-1, "bash: .: filename arg required\n"), -1);
// 					args = ft_construct_command(cmd->args);
// 					if (!args)
// 						return (free(path), 255);
// 					execve(path, args, ft_construct_command(env));
// 					free(args);
// 					free(path);
// 					path = cmd->args;
// 					if (ft_strchr(path, '/'))
// 						exit_(-1, "bash: %s: no such file or directory\n");
// 					else
// 						exit_(-1, "bash: %s: command not found\n");
// 				}
// 				(exit(exit_c), 0);
// 			}
// 			else if (cmd->in_fd != 0)
// 				close(cmd->in_fd);
// 			pid = signal(SIGINT, SIG_IGN);
// 		}
// 		else if (res == 0 && n->count_cmd == 1)
// 			res = execute_builtin(&n->cmds[i_cmd], d, n);
// 		cmd = cmd -> nxt;
// 	}
// 	signal(SIGINT, &sig_handler_fork); // mb sig_handler_mai n
// 	return (res);
// }

// int	exec_command(t_node *n)
// {
// 	int	num;
// 	int	pid;
// 	int	result;
// 	t_list cmd;
// 	char	*path;
// 	int	exit_c;
// 	char	**args;

// 	num = 0;
// 	result = -1;
// 	pid = 0;
// 	cmd = *l;
// 	while (cmd != NULL)
// 	{
// 		result = ft_prepare_pipe(n, cmd);
// 		if ((check(cmds), n->count_cmd, result) && num++ >= 0)
// 		{
// 			pid = fork();
// 			if (pid < -1)
// 				exit_();
// 			exit_c = 0;
// 			if (pid == 0)
// 			{
// 				signal(SIGINT, &sig_handler_fork);
// 				signal(SIGQUIT, &sig_handler_fork);
// 				if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
// 					exit_();
// 				if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
// 					exit_();
// 				ft_clean_fds(cmd);
// 				if (cmd->params && ft_is_builtin(cmd->params) == 1)
// 					exit_c = exec_builtin(cmd, d, n);
// 				else if (cmd->params)
// 				{
// 					exit_c = exec_program(cmd, d->env, n);
// 					path = NULL;
// 					if (!ft_strcmp(cmd->ars, "."))
// 						return (exit_(-1, "bash: .: filename arg required\n"), -1);
// 					exit_c = ft_find_path(cmd->args, env, &path);
// 					if (!path)
// 						exit_c;
// 					args = ft_construct_command(cmd->args);
// 					if (!args)
// 						return (free(path), 255);
// 					execve(path, args, ft_construct_command(env));
// 					free(args);
// 					free(path);
// 					path = cmd->args;
// 					if (ft_strchr(path, '/'))
// 						exit_(-1, "bash: %s: no such file or directory\n");
// 					else
// 						exit_(-1, "bash: %s: command not found\n");
// 				}
// 				pid = (exit(exit_c), 0);
// 			}
// 			else if (cmd->in_fd != 0)
// 				close(cmd->in_fd);
// 			pid = signal(SIGINT, SIG_IGN), pid;
// 		}
// 		else if (result == 0 && n->count_cmd == 1)
// 			result = execute_builtin(&n->cmds[i_cmd], d, n);
// 		cmd = cmd -> nxt;
// 	}
// 	ft_wait_child_processes(&result, num, pid);
// 	signal(SIGINT, &sig_handler_fork); // mb sig_handler_mai n
// 	return (result);
// }
