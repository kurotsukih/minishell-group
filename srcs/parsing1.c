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

// static int	parse_and_add_cmd_to_list(t_list **cmds, char *s, size_t s_len)
// {
// 	size_t	i;
// 	size_t	i_beg;
// 	size_t	j;
// 	t_list	*cur;

// 	i = 0;
// 	j = -1;
// 	while (i < s_len - ft_strlen(new->redirect))
// 	{
// 		while (s[i] == ' ')
// 			i++;
// 		i_beg = i;
// 		while (is_correct_token_(&s[i_beg], i) == 1)
// 			i++;
// 		if (i == i_beg)
// 			break ;
// 		new->tokens[++j] = NULL;
// 		new->tokens[j] = strdup_(&s[i_beg], i - i_beg + 1);
// 		if (new->tokens[j] == NULL)
// 			return (-1); // error msg
// 	}
// 	return (0);
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
	printf("cmd_line = [%s]\n", cmd_line);
	while (cmd_line[i] != '\0')
	{
		printf("while %s, i = %d\n", &cmd_line[i_beg], i);
		mod = mod_(cmd_line[i]);
		redirect = redirect_(&cmd_line[i]);
		if ((mod == OUTSIDE_QUOTES && ft_strlen(redirect) > 0) || cmd_line[i + 1] == '\0')
		{
			len_cmd = i - i_beg + (cmd_line[i + 1] == '\0');
			printf("put %s %d\n", &cmd_line[i_beg], len_cmd);
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

void calc_nb_args(t_list **l)
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

int	minishell(char *cmd_line, char **env)
{
	t_list	**l;

	init_list(&l);
	if (treat_redirects(cmd_line, l) == -1)
		return (-1);
	print_list(l);
	calc_nb_args(l);
	print_list(l);
	(void)env;
	return (0);
}
