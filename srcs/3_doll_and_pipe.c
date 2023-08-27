#include "headers.h"

// " $ " is ok ?

// should we treat this ?
// a="s -la"   > l$a -> ls -la
// $a=" "      > ls$a-la$a"Makefile" -> ls -la Makefile 

static char	*new_s_(int i, char *old_s, t_data **d)
{
	int		j;
	char	*key;
	char	*val;
	char	*new_s;
	int		len_new_s;

	key = alphanum_(&(old_s[i + 1]), d);
	val = get_value_from_env(key, d);
	len_new_s = ft_strlen(old_s) - ft_strlen(key) + ft_strlen(val);
	new_s = (char*)malloc_(len_new_s + 1, d);
	j = -1;
	while (++j < i)
		new_s[j] = old_s[j];
	j--;
	while (++j < i + (int)ft_strlen(val))
		new_s[j] = val[j - i];
	j--;
	while(++j < len_new_s)
		new_s[j] = old_s[j + (int)ft_strlen(key) - (int)ft_strlen(val) + 1];
	new_s[j] = '\0';
	free(key);
	free(val);
	return (new_s);
}

static void	put_doll_conversions_1(char **s, t_data **d)
{
	int		i;
	char	*new_s;

	i = -1;
	while ((*s)[++i] != '\0' && (*s)[i + 1] != '\0')
	{
		// if (s[i] == '$' && s[i + 1] == '?')
		// 	(ft_itoa(exit_code));
		if ((*s)[i] == '$')
		{
			new_s = new_s_(i, *s, d);
			free(*s);
			*s = new_s;
		}
	}
}

void	calc_doll_conversions(t_data **d)
{
	t_cmds	*cmd;
	int		i;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		i = 0;
		while(++i < cmd->nb_args)
			if (cmd->args[i][0] != '\'')
				put_doll_conversions_1(&(cmd->args[i]), d);
		cmd = cmd->nxt;
	}
}

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

// 	if (i_cmd != 0 && ns[i_cmd - 1].out_fd != 1)
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
