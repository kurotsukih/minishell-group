#include "minishell.h"

// each key word into token : cat || ls  -> ['cat', '|', 'ls']
static int	is_token(char c, int checker)
{
	static char	mode;

	if (checker == 1)
	{
		if (mode == 0)
			return (1);
		return (mode = 0);
	}
	else if (mode == 'd' || (c == '\"' && mode == 0))
	{
		if (mode == 0)
			mode = 'd';
		else if (c == '\"')
			mode = 0;
	}
	else if (mode == 's' || (c == '\'' && mode == 0))
	{
		if (mode == 0)
			mode = 's';
		else if (c == '\'')
			mode = 0;
	}
	else if (ft_isspace(c) == 1)
		return (0);
	return (1);
}

static t_list	*add_token(char *cdm_with_spaces, int i_beg, int i_end, t_data *d)
{
	char	*new_str;
	t_list	*n;
	int		i;

	if (i_beg == i_end)
		i_end++;
	new_str = (char *)malloc((i_end - i_beg + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < i_end - i_beg)
	{
		new_str[i] = cdm_with_spaces[i + i_beg];
		i++;
	}
	new_str[i] = '\0';
	if (ft_strchr(new_str, '$'))
		n = expand_token(new_str, d->env, d);
	else
		n = ft_lstnew(new_str, 0);
	if (!n)
		return (NULL);
	return (n);
}

t_list	*tokenization(char *cmd, t_data *d)
{
	t_list	*head;
	int		i_beg;
	int		i_end;
	t_list	*t;
	char	*cmd_with_spaces;

	cmd_with_spaces = add_spaces(cmd);
	if (!cmd_with_spaces)
		return (NULL);
	i_beg = 0;
	head = NULL;
	while (cmd_with_spaces[i_beg])
	{
		i_end = i_beg;
		while (cmd_with_spaces[i_end] && is_token(cmd_with_spaces[i_end], 0))
			i_end++;
		t = add_token(cmd_with_spaces, i_beg, i_end, d);
		if (!t)
			return (exit_(-1, NULL, NULL, &head, &free, NULL), NULL);
		ft_lstadd_back(&head, t);
		i_beg = i_end + 1;
	}
	if (is_token(0, 1) == 0)
		return (exit_(-1, "BASH: unclosed quotes\n", NULL, &head, &free, NULL), NULL);
	ft_remove_quotes_list(head);
	free(cmd_with_spaces);
	return (head);
}

static void	assign_type(t_list *n, int is_filename)
{
	if (!ft_strcmp(n->content, "|") && n->type == INDEF)
		n->type = PIPE;
	else if (!ft_strcmp(n->content, "||") && n->type == INDEF)
		n->type = OR;
	else if (!ft_strcmp(n->content, "&&") && n->type == INDEF)
		n->type = AND;
	else if (!ft_strcmp(n->content, "(") && n->type == INDEF)
		n->type = LEFT_P;
	else if (!ft_strcmp(n->content, ")") && n->type == INDEF)
		n->type = RIGHT_P;
	else if (!ft_strcmp(n->content, "<") && n->type == INDEF)
		n->type = REDIR_IN;
	else if (!ft_strcmp(n->content, "<<") && n->type == INDEF)
		n->type = HEREDOC;
	else if (!ft_strcmp(n->content, ">") && n->type == INDEF)
		n->type = REDIR_OUT;
	else if (!ft_strcmp(n->content, ">>") && n->type == INDEF)
		n->type = REDIR_OUT2;
	else if (is_filename == 1 && n->type == INDEF)
		n->type = FILENAME; // не используется
	else
		n->type = PARAM;
}

//    Go through each value and assign the values
//    if I see <. then no matter what the next token is infile
void	assign_types(t_list *n)
{
	int	is_filename;

	is_filename = 0;
	while (n)
	{
		assign_type(n, is_filename);
		is_filename = 0;
		if (n->type == REDIR_IN || n->type == REDIR_IN || n->type == HEREDOC || n->type == REDIR_OUT2)
			is_filename = 1;
		n = n->next;
	}
}

int	parse(char *cmd, t_list *env, t_data *d)
{
	t_list	*head;
	int		code;

	head = tokenization(cmd, d);
	if (!head)
		return (d->exit_code = 255, 255);
	assign_types(head);
	if (check_tokens(head) == 0)
		return (d->exit_code = 2, free_redirections(head), 2);
	code = open_heredocs(head, env);
	if (code != 0)
		return (d->exit_code = code, free_redirections(head), code);
	d->n = NULL;
	d->n = make_tree(head, NULL);
	if (d->n == NULL)
		return (d->exit_code = 255, free_redirections(head), 255);
	return (0);
}
