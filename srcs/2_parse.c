#include "headers.h"

static void	put_1_cmd_and_redirect(char *cmd_txt, int len, char *redirect, t_data **d)
{
	t_cmds	*new;
	t_cmds	*last;
	int		i;

	init_cmd(&new, redirect, d);
	mod_(REINIT_QUOTES);
	new->nb_args = nb_args_(cmd_txt, len);
	new->args = (char **)malloc_((new->nb_args + 1)* sizeof(char *), d);
	new->args[0] = (char *)malloc_(len + 1, d);
	i = -1;
	while (++i < len)
		new->args[0][i] = cmd_txt[i];
	new->args[0][i] = '\0';
	if (*((*d)->cmds) == NULL)
		*((*d)->cmds) = new;
	else
	{
		last = *((*d)->cmds);
		while (last != NULL && last->nxt != NULL)
			last = last->nxt;
		last->nxt = new;
	}
}

void	put_cmd_line_and_redirects(char *cmd_line, t_data **d)
{
	int		i_beg;
	int 	i;
	char	*redirect;
	int		len_cmd;

	i_beg = 0;
	i = 0;
	mod_(REINIT_QUOTES);
	while (cmd_line[i] != '\0') // while 1
	{
		redirect = redirect_(&cmd_line[i]);
		if ((mod_(cmd_line[i]) == QUOTES0 && ft_strlen(redirect) > 0) || cmd_line[i + 1] == '\0')
		{
			len_cmd = i - i_beg + (cmd_line[i + 1] == '\0');
			put_1_cmd_and_redirect(&cmd_line[i_beg], len_cmd, redirect, d);
			if (cmd_line[i + 1] == '\0')
				break;
			i += ft_strlen(redirect) + 1;
			i_beg = i;
		}
		i++;
	}
}

static void	calc_args_1(t_cmds *cmd, t_data **d)
{
	int		i;
	int		i_beg;
	int		k;
	int		len;
	int		to_put_EOL;

	if (cmd->nb_args <= 1)
		return ;
	mod_(REINIT_QUOTES);
	i_beg = 0;
	k = 0;
	len = (int)ft_strlen(cmd->args[0]);
	i = -1;
	while (++i < len)
		if (mod_(cmd->args[0][i]) == QUOTES0 && cmd->args[0][i] != ' ' && (cmd->args[0][i + 1] == ' ' || cmd->args[0][i + 1] == '\0' || cmd->args[0][i + 1] == '\'' || cmd->args[0][i + 1] == '\"'))
		{
			if (k == 0)
				to_put_EOL = i + 1;
			else strdup_and_trim(&(cmd->args[0][i_beg]), &(cmd->args[k]), i - i_beg + 1, d);
			i_beg = i + 1;
			k++;
		}
	cmd->args[0][to_put_EOL] = '\0';
}

void	calc_args(t_data **d)
{
	t_cmds	*cmd;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		calc_args_1(cmd, d);
		cmd->args[cmd->nb_args] = NULL;
		cmd = cmd->nxt;
	}
}

int	there_are_unclosed_quotes(t_data **d)
{
	int		mod;
	t_cmds	*cmd;
	int		i;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		mod_(REINIT_QUOTES);
		i = -1;
		while (cmd->args[0][++i] != '\0')
			mod = mod_(cmd->args[0][i]);
		if (mod != QUOTES0)
			return ((*d)->err = "unclodes quotes", 1);
		cmd = cmd->nxt;
	}
	return (0);
}


