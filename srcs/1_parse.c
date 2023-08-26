#include "headers.h"

int	put_cmd_and_redirect_1(t_cmds **l, char *s, int len_s, char *redirect)
{
	t_cmds	*new;
	t_cmds	*cmd_list;
	int		i;

	if (init_new_cmd(&new, redirect) == -1)
		return (-1);
	mod_(REINIT_QUOTES);
	new->nb_args = nb_args_(s, len_s);
	new->args = (char **)malloc((new->nb_args + 1)* sizeof(char *));
	if (new->args == NULL)
		return (-1);
	// ft_memset(new->args, NULL, new->nb_args + 1); ///
	new->args[0] = (char *)malloc(len_s + 1);
	if (new->args[0] == NULL)
		return (-1);
	i = -1;
	while (++i < len_s)
		new->args[0][i] = s[i];
	new->args[0][i] = '\0';
	if (*l == NULL)
		*l = new;
	else
	{
		cmd_list = *l;
		while (cmd_list != NULL && cmd_list->nxt != NULL)
			cmd_list = cmd_list->nxt;
		cmd_list->nxt = new;
		new->prv = cmd_list;
	}
	return (0);
}

int put_cmd_and_redirect(char *cmd_line, t_cmds **l)
{
	int		i_beg;
	int 	i;
	char	*redirect;
	int		len_cmd;

	i_beg = 0;
	i = 0;
	mod_(REINIT_QUOTES);
	while (cmd_line[i] != '\0')
	{
		redirect = redirect_(&cmd_line[i]);
		if ((mod_(cmd_line[i]) == QUOTES0 && ft_strlen(redirect) > 0) || cmd_line[i + 1] == '\0')
		{
			len_cmd = i - i_beg + (cmd_line[i + 1] == '\0');
			if (put_cmd_and_redirect_1(l, &cmd_line[i_beg], len_cmd, redirect) == -1)
				return (-1);
			if (cmd_line[i + 1] == '\0')
				break;
			i += ft_strlen(redirect) + 1;
			i_beg = i;
		}
		i++;
	}
	return (0);
}

static int	put_args_1(t_cmds *cmd)
{
	int		i;
	int		i_beg;
	int		k;
	int		len;
	int		to_put_EOL;

	if (cmd->nb_args <= 1)
		return (0);
	mod_(REINIT_QUOTES);
	i_beg = 0;
	k = 0;
	len = (int)ft_strlen(cmd->args[0]);
	i = -1;
	while (++i < len)
	{
		if (mod_(cmd->args[0][i]) == QUOTES0 && cmd->args[0][i] != ' ' && (cmd->args[0][i + 1] == ' ' || cmd->args[0][i + 1] == '\0' || cmd->args[0][i + 1] == '\'' || cmd->args[0][i + 1] == '\"'))
		{
			if (k == 0)
				to_put_EOL = i + 1;
			else if (strdup_and_trim(&(cmd->args[0][i_beg]), &(cmd->args[k]), i - i_beg + 1) == -1)
				return (-1);
			i_beg = i + 1;
			k++;
		}
	}
	cmd->args[0][to_put_EOL] = '\0';
	return (0);
}

int	put_args(t_cmds **l)
{
	t_cmds	*cmd;

	cmd = *l;
	while(cmd != NULL)
	{
		if (put_args_1(cmd) == -1)
			return (-1);
		cmd->args[cmd->nb_args] = NULL;
		cmd = cmd->nxt;
	}
	return (0);
}
