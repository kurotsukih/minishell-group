#include "minishell.h"

/// double quotes insdide simple ones ?
int	mod_(char c, int to_reinitialize)
{
	static char	mod = OUTSIDE_QUOTES;

	if (to_reinitialize)
		return (mod = OUTSIDE_QUOTES, mod);
	if (mod == OUTSIDE_QUOTES && c == '\'')
		mod = INSIDE_SIMP_QUOTES;
	else if (mod == OUTSIDE_QUOTES && c == '\"')
		mod = INSIDE_DOUB_QUOTES;
	else if (mod == INSIDE_SIMP_QUOTES && c == '\'')
		mod = OUTSIDE_QUOTES;
	else if (mod == INSIDE_DOUB_QUOTES && c == '\"')
		mod = OUTSIDE_QUOTES;
	return (mod);
}

static char	*redirect_(char *s)
{
	if (s[0] == '>' && s[1] == '>')
		return (">>");
	else if (s[0] == '<' && s[1] == '<')
		return ("<<");
	else if (s[1] == '>' && s[1] != '\0' && s[2] != '>')
		return (">");
	else if (s[1] == '<' && s[1] != '\0' && s[2] != '<')
		return ("<");
	else if (s[1] == '|')
		return ("|");
	else
		return ("");
}

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

// if $a="s -la" then l$a -> ls -la ?
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

// static int nb_tokens_(char *s, int s_len)
// {
// 	int i;
// 	int i_beg;
// 	int nb_tokens;

// 	i = 0;
// 	nb_tokens = 0;
// 	while (i < s_len)
// 	{
// 		while (s[i] == ' ')
// 			i++;
// 		i_beg = i;
// 		while (is_correct_token_(&s[i_beg], i) == 1)
// 			i++;
// 		if (i == i_beg)
// 			break ;
// 		nb_tokens++;
// 	}
// 	return (nb_tokens);
// }

// static int	parse_and_add_cmd_to_list(t_list **cmds, char *s, size_t s_len)
// {
// 	t_list	*new;
// 	size_t	i;
// 	size_t	i_beg;
// 	size_t	j;
// 	t_list	*cur;

// 	new = NULL;
// 	new = (t_list *)malloc(sizeof(t_list));
// 	if (new == NULL)
// 		return (-1);
// 	new->nxt = NULL;
// 	new->prv = NULL;
// 	new->redirect = redirect_(&s[s_len - 2]);
// 	new->nb_tokens = nb_tokens_(s, s_len - ft_strlen(new->redirect)); ///
// 	if (new->nb_tokens == 0)
// 		return (-1); // error msg
// 	new->tokens = NULL;
// 	new->tokens = (char **)malloc(new->nb_tokens * sizeof(char *));
// 	if (new->tokens == NULL)
// 		return (-1); // error msg
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
// 	new->is_filename = 0; ///
// 	if (*cmds == NULL)
// 		*cmds = new;
// 	else
// 		{
// 			cur = *cmds;
// 			while (cur != NULL && cur->nxt != NULL)
// 				cur = cur->nxt;
// 			cur->nxt = new;
// 			new->prv = cur;
// 		}
// 	return (0);
// }

// ab <   cd<< ef <
int treat_redirects(char *cmd_line, t_list **l)
{
	int		i_beg;
	int 	i;
	int		mod;
	char	*redirect;
	int		len_cmd;

	i_beg = 0;
	i = 0;
	while (cmd_line[i] != '\0')
	{
		printf("i = %d, i_beg = %d\n", i, i_beg);
		mod = mod_(cmd_line[i], 0);
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
	if (mod_(cmd_line[i], 0) != OUTSIDE_QUOTES)
		return (-1);
	return (0);
}

int	process(char *cmd_line, char **env)
{
	t_list	**l;

	init_list(&l);
	print_list(l);
	if (treat_redirects(cmd_line, l) == -1)
		return (-1);
	print_list(l);
	(void)env;
	return (0);
}
