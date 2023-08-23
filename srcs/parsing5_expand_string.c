// #include "minishell.h"

// char	*strchr_alt(const char *s, int c)
// {
// 	int		i;
// 	char	mode;

// 	mode = '\0';
// 	i = 0;
// 	if (c < 0)
// 		return (NULL);
// 	while (s[i])
// 	{
// 		if (mode == 0 && s[i] == '\'')
// 			mode = '\'';
// 		else if (mode == '\'' && s[i] == '\'')
// 			mode = '\0';
// 		else if (mode == 0 && s[i] == '\"')
// 			mode = '\"';
// 		else if (mode == '\"' && s[i] == '\"')
// 			mode = '\0';
// 		else if (mode == '\0' || mode == '\"')
// 			if (s[i] == c % 256 && (ft_isalnum(s[i + 1]) || s[i + 1] == '?'))
// 				return ((char *)(s + i));
// 		i++;
// 	}
// 	if (c == 0)
// 		return ((char *)(s + i));
// 	return (NULL);
// }

// static int	strlen_alt(const char *str)
// {
// 	char	mode;
// 	int		size;
// 	int		i;

// 	size = 0;
// 	mode = '\0';
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (mode == 0 && str[i] == '\'')
// 			mode = '\'';
// 		else if (mode == '\'' && str[i] == '\'')
// 			mode = '\0';
// 		else if (mode == 0 && str[i] == '\"')
// 			mode = '\"';
// 		else if (mode == '\"' && str[i] == '\"')
// 			mode = '\0';
// 		else if (str[i] == '$' && (mode == '\0' || mode == '\"'))
// 			break ;
// 		size++;
// 		i++;
// 	}
// 	return (size);
// }

// int	size_expanded(char *str, char *value, char *end)
// {
// 	char	mode;
// 	int		size;
// 	int		i;

// 	size = 0;
// 	mode = '\0';
// 	i = 0;
// 	while (str)
// 	{
// 		if (mode == 0 && str[i] == '\'')
// 			mode = '\'';
// 		else if (mode == '\'' && str[i] == '\'')
// 			mode = '\0';
// 		else if (mode == 0 && str[i] == '\"')
// 			mode = '\"';
// 		else if (mode == '\"' && str[i] == '\"')
// 			mode = '\0';
// 		else if (str[i] == '$' && (mode == '\0' || mode == '\"'))
// 			break ;
// 		size++;
// 		i++;
// 	}
// 	size = size + ft_strlen(value) + ft_strlen(end);
// 	return (size);
// }

// int	strlcat_alt(char *dst, const char *src, int dstsize)
// {
// 	int	src_len;
// 	int	dst_len;
// 	int	i;

// 	if (!src && !dst)
// 		return (0);
// 	if (!dst && dstsize == 0)
// 		return (strlen_alt(src));
// 	i = 0;
// 	src_len = strlen_alt(src);
// 	dst_len = ft_strlen(dst);
// 	if (dst_len > dstsize)
// 		return (src_len + dstsize);
// 	while (dst_len + i + 1 < dstsize && i < src_len)
// 	{
// 		dst[dst_len + i] = src[i];
// 		i++;
// 	}
// 	if (dstsize != 0)
// 		dst[dst_len + i] = '\0';
// 	return (src_len + dst_len);
// }

// char	*strjoin_big(char *str, char *value, char *end)
// {
// 	char	*returner;
// 	int		size;

// 	size = size_expanded(str, value, end);
// 	returner = (char *)malloc(size + 1);
// 	ft_memset(returner, '\0', size + 1); /// было memset_
// 	if (!returner)
// 		return (NULL);
// 	if (str)
// 		strlcat_alt(returner, str, size + 1);
// 	if (value)
// 		ft_strlcat(returner, value, size + 1);
// 	if (end)
// 		ft_strlcat(returner, end, size + 1);
// 	free(str);
// 	return (returner);
// }

// static int	find_key(char *str)
// {
// 	int	i;

// 	i = 0;
// 	if (str[i] == '?' && str[i + 1] == '\0')
// 		return (i + 1);
// 	while (str[i])
// 	{
// 		if (ft_isalnum(str[i]) != 1 && str[i] != '_')
// 			break ;
// 		i++;
// 	}
// 	return (i);
// }

// static char	*find_value(char *key, int i_pos, t_lis t *env, t_data *d)
// {
// 	if ((*key) == '?')
// 	{
// 		return (ft_itoa(d->exit_code));
// 	}
// 	while (env)
// 	{
// 		if (ft_strncmp((char *)env->content, key, i_pos) == 0)
// 			if (((char *)env->content)[i_pos] == '=')
// 				return ((char *)env->content + i_pos + 1);
// 		env = env->next;
// 	}
// 	return (NULL);
// }

// char	*expand_string(char *str, t_lis t *env, t_data *d)
// {
// 	char	*temp;
// 	char	*value;
// 	int		pos_key;
// 	int		status;

// 	temp = strchr_alt(str, '$');
// 	status = 0;
// 	if (!temp)
// 		return (str);
// 	while (temp)
// 	{
// 		pos_key = find_key(temp + 1);
// 		value = find_value(temp + 1, pos_key, env, d);
// 		if (temp[1] == '?' && (temp[2] == ' ' || temp[2] == '\0'))
// 			status = 1;
// 		str = strjoin_big(str, value, temp + pos_key + 1);
// 		if (status == 1)
// 			free(value);
// 		if (!str)
// 			return (NULL);
// 		temp = strchr_alt(str, '$');
// 	}
// 	return (str);
// }
