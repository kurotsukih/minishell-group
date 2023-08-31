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

// void	put_redirect_out(t_data **d)
// {
// 	t_cmds	*cmd;
// 	int		i;

// 	cmd = *((*d)->cmds);
// 	while(cmd != NULL)
// 	{
// 		i = cmd->nb_args - 1;
// 		while(--i > 0)
// 		{
// 			if (ft_strcmp(cmd->args[i], ">") == 0)
// 				cmd->redir_out = cmd->args[i + 1];
// 			if (ft_strcmp(cmd->args[i], ">>") == 0)
// 				cmd->redir_out2 = cmd->args[i + 1];
// 			if (cmd->redir_out !=NULL || cmd->redir_out2 != NULL)
// 					break;
// 		}
// 		cmd = cmd->nxt;
// 	}
// }

// void	put_redirect_in(t_data **d)
// {
// 	t_cmds	*cmd;
// 	char	*cur;
// 	int		i;

// 	cmd = *((*d)->cmds);
// 	while(cmd != NULL)
// 	{
// 		i = cmd->nb_args - 1;
// 		while(--i > 0)
// 			if (ft_strcmp(cmd->args[i], "<") == 0)
// 			{
// 				cmd->redir_in = cmd->args[i + 1];
// 				break;
// 			}
// 		cmd = cmd->nxt;
// 	}
// }

void	del_all_redirs_from_list_args(t_data **d)
{
	t_cmds	*cmd;
	int		i;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		i = -1;
		while(++i < cmd->nb_args - 1)
			if (ft_strcmp(cmd->args[i], "<") == 0 ||ft_strcmp(cmd->args[i], ">") == 0 || ft_strcmp(cmd->args[i], ">>") == 0)
			{
				cmd->redir_in = cmd->args[i + 1];
			}
		cmd = cmd->nxt;
	}
}
