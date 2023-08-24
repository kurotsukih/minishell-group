#include "minishell.h"

int	put_cmd_and_redirect_1(t_list **l, char *cmd_str, int len_cmd, char *redirect)
{
	t_list	*new;
	t_list	*cmd_list;
	int	i;

	if (init_new_cmd(&new) == -1)
		return (-1);
	new->redirect = redirect;
	new->cmd = (char *)malloc(len_cmd + 1);
	if (new->cmd == NULL)
		return (-1);
	i = -1;
	while (++i < len_cmd)
		new->cmd[i] = cmd_str[i];
	new->cmd[i] = '\0';
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

int put_cmd_and_redirect(char *cmd_line, t_list **l)
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

void put_nb_args(t_list **l)
{
	int		i;
	int		nb_args;
	t_list	*cmd;

	cmd = *l;
	while(cmd != NULL)
	{
		mod_(REINIT_QUOTES);
		nb_args = 0;
		i = -1;
		while (cmd->cmd[++i] != '\0')
			if (mod_(cmd->cmd[i]) == QUOTES0 && cmd->cmd[i] != ' ' && (cmd->cmd[i + 1] == ' ' || cmd->cmd[i + 1] == '\0' || cmd->cmd[i + 1] == '\'' || cmd->cmd[i + 1] == '\"'))
				nb_args++;
		cmd->nb_args = nb_args - 1;
		cmd = cmd->nxt;
	}
}

static int	put_args_1(t_list *cmd)
{
	int		i;
	int		i_beg;
	int		num_arg;
	int		len;
	int		to_put_EOL;

	mod_(REINIT_QUOTES);
	i_beg = 0;
	num_arg = -1;
	len = (int)ft_strlen(cmd->cmd);
	i = -1;
	while (++i < len)
		if (mod_(cmd->cmd[i]) == QUOTES0 && cmd->cmd[i] != ' ' && (cmd->cmd[i + 1] == ' ' || cmd->cmd[i + 1] == '\0' || cmd->cmd[i + 1] == '\'' || cmd->cmd[i + 1] == '\"'))
		{
			if (num_arg == -1)
				to_put_EOL = i + 1;
			else if (strdup_and_trim(&(cmd->cmd[i_beg]), &(cmd->args[num_arg]), i - i_beg + 1) == -1)
				return (-1);
			i_beg = i + 1;
			num_arg++;
		}
	return (cmd->cmd[to_put_EOL] = '\0', 0);
}

int	put_args(t_list **l)
{
	t_list	*cmd;

	cmd = *l;
	while(cmd != NULL)
	{
		cmd->args = (char **)malloc(cmd->nb_args * sizeof(char *));
		if (cmd->args == NULL)
			return (-1);
		if (put_args_1(cmd) == -1)
			return (-1);
		cmd = cmd->nxt;
	}
	return (0);
}
