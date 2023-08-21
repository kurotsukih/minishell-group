#include "minishell.h"

char	*find_dollar_conversion(const char *s) //// много меняла
{
	int		i;
	char	mode;

	mode = '\0';
	i = -1;
	while (s[++i])
	{
		if      (mode == 0 && s[i] == '\'')
			mode = '\'';
		else if (mode == '\'' && s[i] == '\'')
			mode = '\0';
		else if (mode == 0 && s[i] == '\"')
			mode = '\"';
		else if (mode == '\"' && s[i] == '\"')
			mode = '\0';
		else if (mode == '\0' || mode == '\"')
			if (s[i] == '$' && (ft_isalnum(s[i + 1]) || s[i + 1] == '?'))
				return ((char *)(s + i));
	}
	return (NULL);
}

static int	strlen_alt(const char *str)
{
	char	mode;
	int		size;
	int		i;

	size = 0;
	mode = '\0';
	i = 0;
	while (str[i])
	{
		if (mode == 0 && str[i] == '\'')
			mode = '\'';
		else if (mode == '\'' && str[i] == '\'')
			mode = '\0';
		else if (mode == 0 && str[i] == '\"')
			mode = '\"';
		else if (mode == '\"' && str[i] == '\"')
			mode = '\0';
		else if (str[i] == '$' && (mode == '\0' || mode == '\"'))
			break ;
		size++;
		i++;
	}
	return (size);
}

int	new_size(char *str, char *value, char *end)
{
	char	mode;
	int		size;
	int		i;

	size = 0;
	mode = '\0';
	i = 0;
	while (str)
	{
		if (mode == 0 && str[i] == '\'')
			mode = '\'';
		else if (mode == '\'' && str[i] == '\'')
			mode = '\0';
		else if (mode == 0 && str[i] == '\"')
			mode = '\"';
		else if (mode == '\"' && str[i] == '\"')
			mode = '\0';
		else if (str[i] == '$' && (mode == '\0' || mode == '\"'))
			break ;
		size++;
		i++;
	}
	size = size + ft_strlen(value) + ft_strlen(end);
	return (size);
}

int	strlcat_alt(char *dst, const char *src, int dstsize)
{
	int	src_len;
	int	dst_len;
	int	i;

	if (!src && !dst)
		return (0);
	if (!dst && dstsize == 0)
		return (strlen_alt(src));
	i = 0;
	src_len = strlen_alt(src);
	dst_len = ft_strlen(dst);
	if (dst_len > dstsize)
		return (src_len + dstsize);
	while (dst_len + i + 1 < dstsize && i < src_len)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	if (dstsize != 0)
		dst[dst_len + i] = '\0';
	return (src_len + dst_len);
}

char	*strjoin_big(char *str, char *value, char *end)
{
	char	*to_return;
	int		size;

	size = new_size(str, value, end);
	to_return = (char *)malloc(size + 1);
	if (!to_return)
		return (NULL);
	ft_memset(to_return, '\0', size + 1); /// было memset_
	if (str)
		strlcat_alt(to_return, str, size + 1);
	if (value)
		ft_strlcat(to_return, value, size + 1);
	if (end)
		ft_strlcat(to_return, end, size + 1);
	free(str);
	return (to_return);
}

static int	find_key(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '?' && str[i + 1] == '\0')
		return (i + 1);
	while (str[i])
	{
		if (ft_isalnum(str[i]) != 1 && str[i] != '_')
			break ;
		i++;
	}
	return (i);
}

static char	*find_value(char *key, int i_pos, t_data *d)
{
	t_list *env;

	if ((*key) == '?')
		return (ft_itoa(d->exit_code));
	env = d->env;
	while (env) //// был отдельный аргумент t_list *env
	{
		if (ft_strncmp((char *)env->content, key, i_pos) == 0)
			if (((char *)env->content)[i_pos] == '=')
				return ((char *)env->content + i_pos + 1);
		env = env->next;
	}
	return (NULL);
}

char	*expand_dollar_conversions(char *str, t_list *env, t_data *d)
{
	char	*c;
	char	*value;
	int		pos_key;
	int		to_free_value;

	c = find_dollar_conversion(str);
	if (!c)
		return (str);
	to_free_value = 0;
	while (c)
	{
		pos_key = find_key(c + 1);
		value = find_value(c + 1, pos_key, d);
		if (c[1] == '?' && (c[2] == ' ' || c[2] == '\0'))
			to_free_value = 1;
		str = strjoin_big(str, value, c + pos_key + 1);
		if (to_free_value == 1)
			free(value);
		if (!str)
			return (NULL);
		c = find_dollar_conversion(str);
	}
	return (str);
}
