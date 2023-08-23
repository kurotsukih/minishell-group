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
	mod_(REINIT_MOD);
	while (cmd_line[i] != '\0')
	{
		printf("i = %d, i_beg = %d\n", i, i_beg);
		mod = mod_(cmd_line[i]);
		redirect = redirect_(&cmd_line[i]);
		if ((mod == OUTSIDE_QUOTES && ft_strlen(redirect) > 0) || cmd_line[i + 1] == '\0')
		{
			len_cmd = i - i_beg + (cmd_line[i + 1] == '\0');
			if (put_cmd_and_redirect_to_l(l, &cmd_line[i_beg], len_cmd, redirect) == -1)
				return (-1);
			i += ft_strlen(redirect) + 1;
			i_beg = i;
			printf("i = %d, i_beg = %d *\n", i, i_beg);
		}
		i++;
	}
	if (mod_(cmd_line[i]) != OUTSIDE_QUOTES)
		return (-1);
	return (0);
}

//if $a="s -la" then l$a -> ls -la ???
// static int	is_correct_token_(char *s, int len)
// {
// 	int	i;

// 	if (s[0] == '\'' && s[len - 1] == '\'')
// 	{
// 		i = 0;
// 		while (++i < len - 1)
// 			if (s[i] == '\'' || s[i] == '\"')
// 				return (0);
// 		return (1);
// 	}
// 	if (s[0] == '\"' && s[len - 1] == '\"')
// 	{
// 		i = 0;
// 		while (++i < len - 1)
// 			if (s[i] == '\'' || s[i] == '\"' || len_dollar_convers(&s[i], len - i + 1) == -1)
// 				return (0);
// 		return (1);
// 	}
// 	i = -1;
// 	while (++i < len)
// 		if (s[i] == '\'' || s[i] == '\"' || len_dollar_convers(&s[i], len - i + 1) == -1 || (i < len - 1 && s[i] != '$' && s[i + 1] == '?'))
// 			return (0);
// 	return (1);
// }

int calc_nb_args_(char *s)
{
	int		i;
	int		mod;
	int		nb_args;

	i = 0;
	nb_args = 0;
	mod_(REINIT_MOD);
	while (s[i] != '\0')
	{
		mod = mod_(s[i]);
		while (mod == OUTSIDE_QUOTES && s[i] == ' ')
			i++;
		if (s[i] == '\0')
			break ;
		while (mod == OUTSIDE_QUOTES && s[i] != ' ' && s[i] != '\0')
			i++;
		nb_args++;
	}
	return (nb_args - 1);
}

int	minishell(char *cmd_line, char **env)
{
	t_list	**l;

	init_list(&l);
 add *	print_list(l);
	if (treat_redirects(cmd_line, l) == -1)
		return (-1);
	print_list(l);
	(void)env;
	return (0);
}
