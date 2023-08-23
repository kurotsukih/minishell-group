#include "minishell.h"

/// double quotes insdide simple ones ?
int	mod_(char c, int reinitialize)
{
	static char	mod = OUTSIDE_QUOTES;

	if (reinitialize)
	{
		mod = OUTSIDE_QUOTES;
		return (mod);
	}
	if (c == 0)
		return (mod);
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

// static int	add_cmd(t_cmd **cmds, char *s, size_t s_len)
// {
// 	t_cmd	*new;
// 	size_t	i;
// 	size_t	i_beg;
// 	size_t	j;
// 	t_cmd	*cur;

// 	new = NULL;
// 	new = (t_cmd *)malloc(sizeof(t_cmd));
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

int	parse(char *s, char **env)
{
	// t_cmd	**cmds;
	int		i_beg;
	int 	i;
	int		mod;
	int		len_redirect;
	int k;

	(void)env;
	i_beg = 0;
	i = 0;
	while (s[i] != '\0')
	{
		mod = mod_(s[i], 0);
		len_redirect = ft_strlen(redirect_(&s[i]));
		printf("i = %d, len_redirect = %d, mod = %d\n", i, len_redirect, mod);
		if ((mod == OUTSIDE_QUOTES && len_redirect > 0) || s[i + 1] == '\0')
		{
			printf("add_cmd (%s, %d)\n", &s[i_beg], i - i_beg + len_redirect + (s[i + 1] == '\0'));
		// 	// k = i_beg;
		// 	// while (k < i - i_beg + len_redirect)
		// 	// 	printf("%c", s[k++]);
		// 	printf(")\n");
		// 	// if (add_cmd(cmds, &s[i_beg], i - i_beg + len_redirect) == -1)
		// 	// 	return (-1);
		// 	// print_cmds(cmds);
			i += len_redirect;
			i_beg = i;
		}
		i++;
	}
	if (mod_(s[i], 0) != OUTSIDE_QUOTES)
	{
		// delete cdm
		return (-1); // error msg
	}
	(void)k;
	(void)i_beg;
	(void)mod;
	return (0);
}
