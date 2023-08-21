#include "minishell.h"

t_list	*ft_free_expand_token(char **words, t_list **head)
{
	int	i;

	i = 0;
	while (words[i])
		free(words[i++]);
	free(words);
	ft_lstclear(head, NULL);
	return (NULL);
}

static int	skip(char const *str, int i, char c)
{
	if (c == '\'')
	{
		i++;
		while (str[i] && str[i] != '\'')
			i++;
		if (str[i] == '\'')
			i++;
	}
	else if (c == '\"')
	{
		i++;
		while (str[i] && str[i] != '\"')
			i++;
		if (str[i] == '\"')
			i++;
	}
	else
		while (str[i] && str[i] == ' ')
			i++;
	return (i);
}

int	ft_find_n_word4(char const *s, char c)
{
	int	i;
	int	n;

	n = 0;
	i = skip(s, 0, ' ');
	if (s[i] != '\0')
		n++;
	while (s[i])
	{
		if (s[i] == '\'' && i++ >= 0)
			i = skip(s, i, '\'');
		else if (s[i] == '\"' && i++ >= 0)
			i = skip(s, i, '\"');
		else if (s[i] == c)
		{
			i = skip(s, i, ' ');
			if (s[i] != c)
				n++;
		}
		else
			i++;
	}
	return (n);
}

static int	ft_strlen_alt(const char *str, int i_pos)
{
	char	mode;
	int		size;
	int		i;

	size = 0;
	mode = '\0';
	i = i_pos;
	while (str[i])
	{
		if (mode == 0 && str[i] == '\'')
			mode = '\'';
		else if (mode == 0 && str[i] == '\"')
			mode = '\"';
		else if (mode == '\'' && str[i] == '\'')
			mode = '\0';
		else if (mode == '\"' && str[i] == '\"')
			mode = '\0';
		else if (str[i] == ' ' && mode == '\0')
			break ;
		size++;
		i++;
	}
	return (size);
}

static char	*ft_add_word(char const *s, char c, int *pos)
{
	char	*word;
	int		word_len;
	int		i;

	while (s[*pos] == c)
		*pos = *pos + 1;
	word_len = ft_strlen_alt(s, *pos);
	word = (char *)malloc(word_len + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (s[*pos] && i < word_len)
	{
		word[i] = s[*pos];
		*pos = *pos + 1;
		i++;
	}
	word[i] = '\0';
	return (word);
}

static char	**split_alt(char const *s, char c)
{
	char	**returner;
	int		n_word;
	int		i;
	int		i_word;

	if (!s)
		return (NULL);
	n_word = ft_find_n_word4(s, c);
	returner = (char **)malloc(sizeof(char *) * (n_word + 1));
	if (!returner)
		return (NULL);
	i = 0;
	i_word = 0;
	while (i_word < n_word)
	{
		returner[i_word] = ft_add_word(s, c, &i);
		if (!returner[i_word])
			return (free_charchar(returner));
		i_word++;
	}
	returner[i_word] = 0;
	return (returner);
}

// Expands (env var expansion) the string and create tokens. 
// All key words were already tokenized, except if token contain 
// env var and env var contains a space,
// a="s -la"   > l$a -> ls -la
// $a=" "      > ls$a-la$a"Makefile" -> ls -la Makefile 
t_list	*expand_token(char *str, t_list *env, t_data *data)
{
	t_list	*head;
	t_list	*token;
	char	*exp_string;
	char	**words;
	int		i;

	exp_string = expand_string(str, env, data);
	if (!exp_string)
		return (NULL);
	if (ft_find_n_word4(exp_string, ' ') <= 1)
		return (ft_lstnew(exp_string, 0));
	words = split_alt(exp_string, ' ');
	if (!words)
		return (NULL);
	i = 0;
	head = NULL;
	while (words[i])
	{
		token = ft_lstnew(words[i], 11);
		if (!token)
			return (ft_free_expand_token(words, &head));
		ft_lstadd_back(&head, token);
		i++;
	}
	return (free(words), head);
}
