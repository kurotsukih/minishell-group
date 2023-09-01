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

static void	verif_args_1(t_cmd *cmd)
{
	if (there_are_unclosed_quotes(cmd) && ft_strlen(cmd->err) == 0)
		cmd->err = "unclosed quotes"; // exit_code ?
	if (!cmd->arg && ft_strlen(cmd->err) == 0)
		cmd->err = "empty commande"; // exit_code = 255
	if (ft_strcmp(cmd->arg[0], "env") == 0 && cmd->nb_args > 1 && ft_strlen(cmd->err) == 0)
		cmd->err = "env : Too many arguments"; // exit_code ?
	if (ft_strcmp(cmd->arg[0], "cd") == 0 && cmd->nb_args > 2 && ft_strlen(cmd->err) == 0)
		cmd->err = "cd : Too many arguments"; // exit_code ?
	if (ft_strcmp(cmd->arg[0], "exit") == 0 && cmd->nb_args > 2 && ft_strlen(cmd->err) == 0)
		cmd->err = "exit : Too many arguments"; // exit_code ?
	if (ft_strcmp(cmd->arg[0], "exit") == 0 && !ft_atoi(cmd->arg[1]) && ft_strlen(cmd->err) == 0)
		cmd->err = "exit: numeric argument required"; // exit_code = 2
}

void	verif_args(t_data **d)
{
	t_cmd	*cmd;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		// (*d)->curr_cmd = cmd; // not used 
		verif_args_1(cmd);
		cmd = cmd->nxt;
	}
}