// #include "headers.h"

// static char	*ft_open_all_files(t_list *arg, t_cmd *cmd)
// {
// 	while (arg)
// 	{
// 		if (arg->type == REDIR_OUT || arg->type == REDIR_OUT2)
// 		{
// 			if (cmd->out_fd != 1)
// 				close(cmd->out_fd);
// 			cmd->out_fd = open(arg, O_WRONLY | O_CREAT | O_APPEND, 0666);
// 		}
// 		else if (arg->type == REDIR_IN)
// 		{
// 			if (fd != 0)
// 				close(fd);
// 			cmd->in_fd = open(arg, O_RDONLY);
// 		}
// 		else
// 		{
// 			if (fd != 0)
// 				close(fd);
// 			cmd->in_fd = *(arg);
// 		}
// 		if (cmd->in_fd == -1 || cmd->out_fd == -1)
// 			return (arg);
// 		arg = arg->next;
// 	}
// 	return (NULL);
// }

// static int	ft_prepare_pipe(t_node *n, int i_cmd)
// {
// 	int		fd[2];
// 	char	*err;

// 	if (i_cmd != 0 && n->cmds[i_cmd - 1].out_fd != 1)
// 		close(n->cmds[i_cmd - 1].out_fd);
// 	if (i_cmd < n->count_cmd - 1)
// 	{
// 		if (pipe(fd) == -1)
// 			return (exit_(-1), 1);
// 		n->cmds[i_cmd].out_fd = fd[1];
// 		n->cmds[i_cmd].out_pipe_fd = fd[0];
// 		n->cmds[i_cmd + 1].in_fd = fd[0];
// 	}
// 	err = ft_open_all_files(n->cmds[i_cmd].redir, n->cmds + i_cmd);
// 	if (err)
// 	{
// 		exit_(-1, err);
// 		if (n->cmds[i_cmd].in_fd != -1 && n->cmds[i_cmd].in_fd != 0)
// 			close(n->cmds[i_cmd].in_fd);
// 		if (n->cmds[i_cmd].out_fd != -1 && n->cmds[i_cmd].out_fd != 1)
// 			close(n->cmds[i_cmd].out_fd);
// 		return (1);
// 	}
// 	return (0);
// }
