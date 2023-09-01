#include "headers.h"

void put_redirs_1(t_cmd *cmd, t_data **d)
{
	char *s;
	int i;
	int i_beg;
	char *redir;
	char *redir_file;

	mod_(REINIT_QUOTES);
	s = strdup_and_erase_args_except_redirs(cmd->arg[0], d);
	i = -1;
	while (s[++i] != '\0')
	{
		redir = redir_(&s[i]);
		if (mod_(s[i]) == QUOTES0 && ft_strlen(redir) > 0)
		{
			i += ft_strlen(redir);
			i_beg = i;
			while (s[i] == ' ')
				i++;
			while (s[i] != ' ' && s[i] != '>' && s[i] != '<' && s[i] != '\0')
				i++;
			redir_file = strndup_and_trim(&s[i_beg], i_beg, d);
			open_file(redir, redir_file, d);
		}
	}
}

static void put_args_1(t_cmd *cmd, t_data **d)
{
	int i;
	int i_beg;
	int k;
	int here_put_EOL;
	char *s;

	if (cmd->nb_args == 0)
		return ;
	if (cmd->nb_args >= 2)
	{
		mod_(REINIT_QUOTES);
		i_beg = 0;
		k = 0;
		i = -1;
		s = strdup_and_erase_redirs(cmd->arg[0], d);
		while (s[++i] != '\0')
			if (mod_(s[i]) == QUOTES0 && s[i] != ' ' && (s[i + 1] == ' ' || s[i + 1] == '\0' || s[i + 1] == '\'' || s[i + 1] == '\"'))
			{
				if (k == 0)
					here_put_EOL = i + 1;
				else
					cmd->arg[k] = strndup_and_trim(&s[i_beg], i - i_beg + 1, d);
				i_beg = i + 1;
				k++;
			}
		free(s);
		cmd->arg[0][here_put_EOL] = '\0';
	}
	s = strndup_and_trim(cmd->arg[0], ft_strlen(cmd->arg[0]), d); // strdup_and_trim
	free(cmd->arg[0]);
	cmd->arg[0] = s;
	cmd->arg[cmd->nb_args] = NULL;
}

void put_redirs_and_args(t_data **d)
{
	t_cmd *cmd;

	cmd = *((*d)->cmds);
	while (cmd != NULL)
	{
		(*d)->curr_cmd = cmd;
		put_redirs_1(cmd, d);
		put_args_1(cmd, d);
		cmd = cmd->nxt;
	}
}

static char	*s_with_conversion_(char *old_s, int j, t_data **d)
{
	char	*new_s;
	int		len_new_s;
	char	*key;
	char	*val;
	int		k;

	key = alphanum_(&(old_s[j + 1]), d);
	val = get_value_from_env(key, d);
	len_new_s = ft_strlen(old_s) - ft_strlen(key) + ft_strlen(val);
	new_s = (char*)malloc_(len_new_s + 1, d);
	k = -1;
	while (++k < j)
		new_s[k] = old_s[k];
	k--; //
	while (++k < j + (int)ft_strlen(val))
		new_s[k] = val[k - j];
	k--;
	while(++k < len_new_s)
		new_s[k] = old_s[k + (int)ft_strlen(key) - (int)ft_strlen(val) + 1];
	new_s[k] = '\0';
	free(key);
	free(val);
	return (new_s);
}

void	calc_dollar_convers(t_data **d)
{
	t_cmd	*cmd;
	char	*s_with_conversion;
	int		i;
	int		j;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		(*d)->curr_cmd = cmd;
		i = 0;
		while(++i < cmd->nb_args)
			if (cmd->arg[i][0] != '\'')
			{
				j = -1;
				while (cmd->arg[i][++j] != '\0' && cmd->arg[i][j + 1] != '\0')
				{
					// if (cmd->arg[i][j] == '$' && cmd->arg[i][j + 1] == '?')
					// 	(ft_itoa(exit_code));
					if (cmd->arg[i][j] == '$')
					{
						s_with_conversion = s_with_conversion_(cmd->arg[i], j, d);
						free(cmd->arg[i]);
						cmd->arg[i] = s_with_conversion;
					}
				}
			}
		cmd = cmd->nxt;
	}
}
