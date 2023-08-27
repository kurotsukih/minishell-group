#include "headers.h"

// exec_command = exec		/ exec_builtin
// exec         = execv e	/ exec_builtin
// Add a moment when there can be only one command and it is builtin
// 127 команда не найдена, дочерний процесс, созданный для ее выполнения, возвращает 127
// 126 команда найдена, но не может быть выполнена

static char	*path_and_cmd(char *s1, int len_s1, t_cmds *cmd, t_data **d)
{
	int		i;
	int		j;
	char	*dest;

	dest = (char *) malloc_(len_s1 + ft_strlen(cmd->args[0]) + 1, d);
	i = 0;
	while (i < len_s1)
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i++] = '/';
	j = 0;
	while (cmd->args[0][j] != '\0')
	{
		dest[i] = cmd->args[0][j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

static char	*path_(t_cmds *cmd, t_data **d)
{
	char	*paths;
	char	*path;
	int		i;
	int		i_beg;

	paths = get_value_from_env("PATH", d);
	if (!paths)
		return ((printf("command not found"), *d)->exit_c = 127, NULL);
	i = -1;
	i_beg = 0;
	while (++i < (int)ft_strlen(paths))
		if (paths[i] == '\0' || paths[i] == ':')
		{
			path = path_and_cmd(&(paths[i_beg]), i - i_beg, cmd, d);
			if (access(path, X_OK) == 0)
				return (path);
			if (errno != 2)
				(*d)->exit_c = 126;
			i_beg = i + 1;
		}
	return (printf("command not found"), (*d)->exit_c = 127, NULL);
}

void	exec_extern_cmd(t_cmds *cmd, t_data **d)
{
	char	*path;

	(*d)->exit_c = 0;
	if (!cmd->args)
	{
		(*d)->exit_c = 255;
		return ;
	}
	path = path_(cmd, d);	
	if (path != NULL)
		execve(path, cmd->args, env_to_array(d));
}

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
