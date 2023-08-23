// #include "minishell.h"

// char	*ft_find_all_paths(t_list *env)
// {
// 	char	*str;

// 	while (env)
// 	{
// 		str = (char *)env->content;
// 		if (ft_strncmp("PATH", str, 4) == 0 && str[4] == '=')
// 			break ;
// 		env = env->next;
// 	}
// 	if (!env)
// 		return (NULL);
// 	return (str + 5);
// }

// static void	ft_clean_darray(char **trash)
// {
// 	int	i;

// 	i = 0;
// 	while (trash[i])
// 	{
// 		free(trash[i]);
// 		i++;
// 	}
// 	free(trash);
// }

// int	ft_free_find_path(char **paths, char *cmd, int exit_code)
// {
// 	ft_clean_darray(paths);
// 	exit_(-1, "bash: %s: command not found\n", cmd, NULL, NULL, NULL);
// 	return (exit_code);
// }

// int	ft_find_path2(char *cmd, char **place)
// {
// 	if (access(cmd, X_OK) == 0)
// 		return ((*place = cmd), 0);
// 	exit_(-1, cmd, NULL, NULL, NULL, NULL);
// 	if (errno == 2)
// 		return (127);
// 	else
// 		return (126);
// }

// static int	ft_find_n_word2(char const *s, char c)
// {
// 	int	i;
// 	int	n;
// 	int	checker;

// 	n = 0;
// 	i = 0;
// 	checker = 1;
// 	while (s[i])
// 	{
// 		if (s[i] != c && checker == 1)
// 		{
// 			checker = 0;
// 			n++;
// 		}
// 		else if (s[i] == c && checker == 0)
// 		{
// 			checker = 1;
// 		}
// 		i++;
// 	}
// 	return (n);
// }

// static char	*add_word(char const *s, char c, int *pos)
// {
// 	char	*word;
// 	int		word_len;
// 	int		i;

// 	word_len = 0;
// 	while (s[*pos] == c)
// 		*pos = *pos + 1;
// 	while (s[*pos + word_len] && s[*pos + word_len] != c)
// 		word_len++;
// 	word = (char *)malloc(sizeof(char) * (word_len + 2));
// 	if (!word)
// 		return (NULL);
// 	i = 0;
// 	while (s[*pos] && s[*pos] != c)
// 	{
// 		word[i] = s[*pos];
// 		*pos = *pos + 1;
// 		i++;
// 	}
// 	word[i] = '/';
// 	word[i + 1] = '\0';
// 	return (word);
// }

// char	**ft_split_alt2(char const *s, char c)
// {
// 	char	**returner;
// 	int		n_word;
// 	int		i;
// 	int		i_word;

// 	if (!s)
// 		return (NULL);
// 	n_word = ft_find_n_word2(s, c);
// 	returner = (char **)malloc(sizeof(char *) * (n_word + 1));
// 	if (!returner)
// 		return (NULL);
// 	i = 0;
// 	i_word = 0;
// 	while (i_word < n_word)
// 	{
// 		returner[i_word] = add_word(s, c, &i);
// 		if (!returner[i_word])
// 			return (free_charchar(returner));
// 		i_word++;
// 	}
// 	returner[i_word] = 0;
// 	return (returner);
// }

// int	ft_find_path(char *cmd, t_list *env, char **place)
// {
// 	char	*str;
// 	char	**paths;
// 	int		i;

// 	if (cmd && ft_strchr(cmd, '/'))
// 		return (ft_find_path2(cmd, place));
// 	str = ft_find_all_paths(env);
// 	if (!str)
// 		return (exit_(-1, "bash: %s: command not found\n", cmd, NULL, NULL, NULL), 127);
// 	paths = ft_split_alt2(str, ':');
// 	if (!paths)
// 		return (exit_(-1, "bash: %s: command not found\n", cmd, NULL, NULL, NULL), 255); // str to free ?
// 	i = -1;
// 	while (paths[++i])
// 	{
// 		str = ft_strjoin(paths[i], cmd);
// 		if (!str)
// 			return (ft_free_find_path(paths, cmd, 255));
// 		if (access(str, X_OK) == 0)
// 			return (ft_clean_darray(paths), (*place = str), 0);
// 		free(str);
// 		if (errno != 2)
// 			return (exit_(-1, cmd, NULL, NULL, NULL, NULL), ft_clean_darray(paths), 126);
// 	}
// 	return (ft_free_find_path(paths, cmd, 127));
// }