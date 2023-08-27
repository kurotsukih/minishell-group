#include "headers.h"

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

void	calc_dollar_conversions(t_data **d)
{
	t_cmds	*cmd;
	char	*new_s;
	int		i;
	int		j;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		i = 0;
		while(++i < cmd->nb_args)
			if (cmd->args[i][0] != '\'')
			{
				j = -1;
				while (cmd->args[i][++j] != '\0' && cmd->args[i][j + 1] != '\0')
				{
					// if (cmd->args[i][j] == '$' && cmd->args[i][j + 1] == '?')
					// 	(ft_itoa(exit_code));
					if (cmd->args[i][j] == '$')
					{
						new_s = new_s_(j, cmd->args[i], d);
						free(cmd->args[i]);
						cmd->args[i] = new_s;
					}
				}

			}
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

// static void	pipe_(int i_cmd, t_data **d)
// {
// 	int		fd[2];

// 	if (i_cmd != 0 && cmds[i_cmd - 1].out_fd != 1)
// 		close(n->cmds[i_cmd - 1].out_fd);
// 	if (i_cmd < n->count_cmd - 1)
// 	{
// 		if (pipe(fd) == -1)
// 			return (exit_(-1), 1);
// 		n->cmds[i_cmd].out_fd = fd[1];
// 		n->cmds[i_cmd].out_pipe_fd = fd[0];
// 		n->cmds[i_cmd + 1].in_fd = fd[0];
// 	}
// 	if (ft_open_all_files(n->cmds[i_cmd].redir, n->cmds + i_cmd))
// 	{
// 		exit_();
// 		if (n->cmds[i_cmd].in_fd != -1 && n->cmds[i_cmd].in_fd != 0)
// 			close(n->cmds[i_cmd].in_fd);
// 		if (n->cmds[i_cmd].out_fd != -1 && n->cmds[i_cmd].out_fd != 1)
// 			close(n->cmds[i_cmd].out_fd);
// 		return (1);
// 	}
// 	return (0);
// }
