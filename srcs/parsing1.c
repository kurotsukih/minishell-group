#include "minishell.h"

// " $ " is ok ?
// static int len_dollar_convers(char *s, int len)
// {
// 	int	len_alphanum;

// 	if (s[0] != '$')
// 		return (0);
// 	if (s[1] == ' ')
// 		return (1);
// 	if (s[1] == '?' && len >= 2 && s[2] == ' ')
// 		return (2);
// 	len_alphanum = ft_strlen(alphanum_(&s[1]));
// 	if (len_alphanum > 0 && len_alphanum + 1 < len && s[len_alphanum + 1] == ' ')
// 		return (len_alphanum + 1);
// 	return (-1);
// }

int treat_redirects(char *cmd_line, t_list **l)
{
	int		i_beg;
	int 	i;
	int		mod;
	char	*redirect;
	int		len_cmd;

	i_beg = 0;
	i = 0;
	mod_(REINIT_QUOTES_MOD);
	while (cmd_line[i] != '\0')
	{
		mod = mod_(cmd_line[i]);
		redirect = redirect_(&cmd_line[i]);
		if ((mod == OUTSIDE_QUOTES && ft_strlen(redirect) > 0) || cmd_line[i + 1] == '\0')
		{
			len_cmd = i - i_beg + (cmd_line[i + 1] == '\0');
			if (put_cmd_and_redirect_to_l(l, &cmd_line[i_beg], len_cmd, redirect) == -1)
				return (-1);
			if (cmd_line[i + 1] == '\0')
				break;
			i += ft_strlen(redirect) + 1;
			i_beg = i;
		}
		i++;
	}
	if (mod_(cmd_line[i]) != OUTSIDE_QUOTES)
		return (-1);
	return (0);
}

void calc_nb_args_for_each_cmd(t_list **l)
{
	int		i;
	int		mod;
	int		nb_args;
	t_list	*cur;

	cur = *l;
	while(cur != NULL)
	{
		i = 0;
		nb_args = 0;
		mod_(REINIT_QUOTES_MOD);
		while (cur->cmd[i] != '\0')
		{
			mod = mod_(cur->cmd[i]);
			while (mod == OUTSIDE_QUOTES && cur->cmd[i] == ' ')
				i++;
			if (cur->cmd[i] == '\0')
				break ;
			while (mod == OUTSIDE_QUOTES && cur->cmd[i] != ' ' && cur->cmd[i] != '\0')
				i++;
			nb_args++;
		}
		cur->nb_args = nb_args - 1;
		cur = cur->nxt;
	}
}

int	put_all_args_for_1_cmd(t_list *cmd)
{
	int		i;
	int		i_beg;
	int		num_arg;
	int		mod;
	int 	i_end_cmd;

	mod_(REINIT_QUOTES_MOD);
	i = 0;
	num_arg = -1;
	while (i < (int)ft_strlen(cmd->cmd))
	{
		mod = mod_(cmd->cmd[i]);
		while (mod == OUTSIDE_QUOTES && cmd->cmd[i] == ' ')
			i++;
		i_beg = i;
		while (mod == OUTSIDE_QUOTES && cmd->cmd[i] != ' ' && i < (int)ft_strlen(cmd->cmd))
			mod = mod_(cmd->cmd[i++]);
		if (i == i_beg)
			break ;
		if (num_arg == -1)
			i_end_cmd = i;
		if (num_arg != -1 && strdup_(&(cmd->cmd[i_beg]), &(cmd->args[num_arg]), i - i_beg) == -1)
			return (-1);
		num_arg++;
	}
	cmd->cmd[i_end_cmd] = '\0';
	return (0);
}

int	put_all_args_for_all_cmds(t_list **l)
{
	t_list	*cmd;

	cmd = *l;
	while(cmd != NULL)
	{
		cmd->args = (char **)malloc(cmd->nb_args * sizeof(char *));
		if (cmd->args == NULL)
			return (-1);
		put_all_args_for_1_cmd(cmd);
		cmd = cmd->nxt;
	}
	return (0);
}

int	minishell(char *cmd_line, char **env)
{
	t_list	**l;

	init_list(&l);
	if (treat_redirects(cmd_line, l) == -1)
		return (-1);
	calc_nb_args_for_each_cmd(l);
	if (put_all_args_for_all_cmds(l) == -1)
		return (-1);
	print_list(l);
	(void)env;
	return (0);
}
