// #include "minishell.h"

// int	ft_prepare_pipe(t_node *n, int i_cmd)
// {
// 	int		fd[2];
// 	char	*err;
// 	if (i_cmd != 0 && n->cmds[i_cmd - 1].out_fd != 1)
// 		close(n->cmds[i_cmd - 1].out_fd);
// 	if (i_cmd < n->count_cmd - 1)
// 	{
// 		if (pipe(fd) == -1)
// 			return (exit_(-1, NULL, NULL, NULL, NULL, NULL), 1);
// 		n->cmds[i_cmd].out_fd = fd[1];
// 		n->cmds[i_cmd].out_pipe_fd = fd[0];
// 		n->cmds[i_cmd + 1].in_fd = fd[0];
// 	}
// 	err = ft_open_all_files(n->cmds[i_cmd].redir, n->cmds + i_cmd);
// 	if (err)
// 	{
// 		exit_(-1, err, NULL, NULL, NULL, NULL);
// 		if (n->cmds[i_cmd].in_fd != -1 && n->cmds[i_cmd].in_fd != 0)
// 			close(n->cmds[i_cmd].in_fd);
// 		if (n->cmds[i_cmd].out_fd != -1 && n->cmds[i_cmd].out_fd != 1)
// 			close(n->cmds[i_cmd].out_fd);
// 		return (1);
// 	}
// 	return (0);
// }

// int	ft_open_in(t_list *token, int fd)
// {
// 	if (token->type == REDIR_IN)
// 	{
// 		if (fd != 0)
// 			close(fd);
// 		fd = open((char *)token->content, O_RDONLY);
// 	}
// 	else
// 	{
// 		if (fd != 0)
// 			close(fd);
// 		fd = *((int *)token->content);
// 	}
// 	return (fd);
// }

// int	ft_open_out(t_list *token, int fd)
// {
// 	char	*str;
// 	str = (char *)token->content;
// 	if (token->type == REDIR_OUT)
// 	{
// 		if (fd != 1)
// 			close(fd);
// 		fd = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0666);
// 	}
// 	else
// 	{
// 		if (fd != 1)
// 			close(fd);
// 		fd = open(str, O_WRONLY | O_CREAT | O_APPEND, 0666);
// 	}
// 	return (fd);
// }

// char	*ft_open_all_files(t_list *token, t_cmd *cmd)
// {
// 	while (token)
// 	{
// 		if (token->type == REDIR_IN)
// 			cmd->in_fd = ft_open_in(token, cmd->in_fd);
// 		else if (token->type == REDIR_OUT)
// 			cmd->out_fd = ft_open_out(token, cmd->out_fd);
// 		else if (token->type == REDIR_OUT2)
// 			cmd->out_fd = ft_open_out(token, cmd->out_fd);
// 		else
// 			cmd->in_fd = ft_open_in(token, cmd->in_fd);
// 		if (cmd->in_fd == -1 || cmd->out_fd == -1)
// 			return ((char *)token->content);
// 		token = token->next;
// 	}
// 	return (NULL);
// }

