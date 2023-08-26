#include "headers.h"

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

// int	ft_free_find_path(char **paths, char *cmd, int exit_code)
// {
// 	ft_clean_darray(paths);
// 	exit_(-1, "bash: %s: command not found\n");
// 	return (exit_code);
// }

// int	ft_find_path(char *cmd, t_env *env, char **place)
// {
// 	char	*str;
// 	char	**paths;
// 	int		i;
// 	t_env	*cur;

// 	if (cmd && ft_strchr(cmd, '/'))
// 		return (ft_find_path2(cmd, place));
// 	cur = env;
// 	while (cur != NULL) // 	while (env != NULL) ?
// 	{
// 		if (ft_strcmp("PATH", env->key) == 0)
// 			break ;
// 		cur = cur->next;
// 	}
// 	if (cur == NULL)
// 		return (NULL);
// 	str = cur->val;
// 	if (!str)
// 		return (exit_(-1, "bash: %s: command not found\n"), 127);
// 	paths = ft_split_alt2(str, ':');
// 	if (!paths)
// 		return (exit_(-1, "bash: %s: command not found\n"), 255); // str to free ?
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
