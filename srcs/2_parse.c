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

