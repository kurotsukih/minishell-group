#include "minishell.h"

int	verify_unclosed_quotes(t_list **l)
{
	int		mod;
	int		i;
	t_list	*cmd;

	cmd = *l;
	while(cmd != NULL)
	{
		mod_(REINIT_QUOTES);
		i = -1;
		while (cmd->cmd[++i] != '\0')
			mod = mod_(cmd->cmd[i]);
		if (mod != QUOTES0)
			return (-1);
		cmd = cmd->nxt;
	}
	return (0);
}

static char	*get_value_from_env(char *key, t_env **env)
{
	t_env	*var;

	var = *env;
	while (var != NULL)
	{
		if (ft_strcmp(var->key, key) == 0)
			return (var->val);
		var = var->nxt;
	}
	return (NULL);
}

// " $ " is ok ?
static int	put_doll_conversions_1(char **s, t_env **env)
{
	int		i;
	int		j;
	char	*key;
	char	*value;
	int		new_size;
	char	*new_s;

	i = -1;
	while ((*s)[++i] != '\0' && (*s)[i + 1] != '\0')
	{
		// if (s[i] == '$' && s[i + 1] == '?')
		// 	return (ft_itoa(exit_code));
		if ((*s)[i] == '$')
		{
			key = alphanum_(&((*s)[i + 1]));
			if (key == NULL)
				return (-1);
			value = get_value_from_env(key, env);
			new_size = ft_strlen(*s) - ft_strlen(key) + ft_strlen(value);
			new_s = NULL;
			new_s = (char*)malloc(new_size + 1);
			if (new_s == NULL)
				return (-1);
			j = 0;
			while (j < i)
			{
				new_s[j] = (*s)[j];
				j++;
			}
			while (j < i + (int)ft_strlen(value))
			{
				new_s[j] = value[j - i];
				j++;
			}
			while(j < new_size)
			{
				new_s[j] = (*s)[j + (int)ft_strlen(key) - (int)ft_strlen(value) + 1];
				j++;
			}
			new_s[j] = '\0';
			free(*s);
			free(key);
			*s = new_s;
		}
	}
	return (0);
}

int	put_doll_conversions(t_list **l, t_env **env)
{
	t_list	*cmd;
	int		i;

	cmd = *l;
	while(cmd != NULL)
	{
		i = -1;
		while(++i < cmd->nb_args)
			if (cmd->args[i][0] != '\'')
				if (put_doll_conversions_1(&(cmd->args[i]), env) == -1)
					return (-1);
		cmd = cmd->nxt;
	}
	return (0);
}

//////////////////////////////////////////////////////////

// static char	*ft_check_operator(t_lis t *n)
// {
// 	if (n->prev == NULL)
// 		return ((char *)n->content);
// 	if (n->next == NULL)
// 		return ("newline");
// 	if (n->next->type == PIPE || n->next->type == OR || n->next->type == AND)
// 		return ((char *)n->next->content);
// 	return (NULL);
// }

// static char	*ft_check_redirection(t_lis t *n)
// {
// 	if (!n->next)
// 		return ("newline");
// 	if (n->next->type != FILENAME)
// 		return ((char *)n->next->content);
// 	return (NULL);
// }

// int	check_tokens(t_lis t *n)
// {
// 	char	*error;
// 	error = NULL;
// 	while (n)
// 	{
// 		if (n->type == PIPE || n->type == OR || n->type == AND )
// 			error = ft_check_operator(n);
// 		else if (n->type == REDIR_IN || n->type == REDIR_OUT ||  n->type == HEREDOC || n->type == REDIR_OUT2)
// 			error = ft_check_redirection(n);
// 		if (error)
// 			return (exit_(-1, "bash: syntax err unexpected token `%s'\n", error, NULL, NULL, NULL), 0);
// 		n = n->next;
// 	}
// 	return (1);
// }

// int	check(t_cmd *cmd, int count, int result)
// {
// 	char	*str;
// 	if (result == 0)
// 	{
// 		if (count != 1)
// 			return (1);
// 		if (!cmd->params)
// 			return (1);
// 		if (cmd->params && ft_is_builtin(cmd->params) != 1)
// 			return (1);
// 		else if (cmd->params && ft_is_builtin(cmd->params) == 1)
// 		{
// 			str = (char *)cmd->params->content;
// 			if (ft_strcmp(str, "cd") == 0 || ft_strcmp(str, "exit") == 0)
// 				return (0);
// 			else if (ft_strcmp(str, "export") == 0 || !ft_strcmp(str, "unset"))
// 				return (0);
// 			else
// 				return (1);
// 		}
// 	}
// 	return (0);
// }

//////////////////////////////////////////////

// Expands (env var expansion) the string and create tokens. 
// All key words were already tokenized, except if token contain 
// env var and env var contains a space,
// a="s -la"   > l$a -> ls -la
// $a=" "      > ls$a-la$a"Makefile" -> ls -la Makefile 

// t_lis t	*expand_token(char *str, t_lis t *env, t_data *d)
// {
// 	t_lis t	*head;
// 	t_lis t	*token;
// 	char	*exp_string;
// 	char	**words;
// 	int		i;
// 	exp_string = expand_string(str, env, d);
// 	if (!exp_string)
// 		return (NULL);
// 	if (find_n_word4(exp_string, ' ') <= 1)
// 		return (ft_lstnew(exp_string, 0));
// 	words = split_alt(exp_string, ' ');
// 	if (!words)
// 		return (NULL);
// 	i = 0;
// 	head = NULL;
// 	while (words[i])
// 	{
// 		token = ft_lstnew(words[i], 11);
// 		if (!token)
// 			return (free_expand_token(words, &head));
// 		ft_lstadd_back(&head, token);
// 		i++;
// 	}
// 	return (free(words), head);
// }
