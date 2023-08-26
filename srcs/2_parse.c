#include "headers.h"

int	verify_unclosed_quotes(t_cmds **l)
{
	int		mod;
	int		i;
	t_cmds	*cmd;

	cmd = *l;
	while(cmd != NULL)
	{
		mod_(REINIT_QUOTES);
		i = -1;
		while (cmd->args[0][++i] != '\0')
			mod = mod_(cmd->args[0][i]);
		if (mod != QUOTES0)
			return (-1);
		cmd = cmd->nxt;
	}
	return (0);
}

// " $ " is ok ?
static int	put_doll_conversions_1(char **s, t_env **env)
{
	int		i;
	int		j;
	char	*key;
	char	*val;
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
			val = get_value_from_env(key, env);
			new_size = ft_strlen(*s) - ft_strlen(key) + ft_strlen(val);
			new_s = NULL;
			new_s = (char*)malloc(new_size + 1);
			if (new_s == NULL)
				return (-1);
			ft_memset(new_s, '\0', new_size + 1);
			j = 0;
			while (j < i)
			{
				new_s[j] = (*s)[j];
				j++;
			}
			while (j < i + (int)ft_strlen(val))
			{
				new_s[j] = val[j - i];
				j++;
			}
			while(j < new_size)
			{
				new_s[j] = (*s)[j + (int)ft_strlen(key) - (int)ft_strlen(val) + 1];
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

int	put_doll_conversions(t_cmds **l, t_env **env)
{
	t_cmds	*cmd;
	int		i;

	cmd = *l;
	while(cmd != NULL)
	{
		i = 0;
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
