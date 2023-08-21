#include "minishell.h"

// Do I have reallocate the memmory for this command? or can keep it like this?
char	**ft_construct_command(t_list *params)
{
	char	**returner;
	int		size;
	int		i;

	size = ft_lstsize(params);
	returner = (char **)malloc(sizeof(char *) * (size + 1));
	if (!returner)
		return (exit_(-1, NULL, NULL, NULL, NULL, NULL), NULL);
	i = 0;
	while (i < size)
	{
		returner[i] = (char *)params->content;
		params = params->next;
		i++;
	}
	returner[i] = NULL;
	return (returner);
}

//  I have to think of exit and free strategy
int	ft_execute_program(t_cmd *cmd, t_list *env, t_node *n)
{
	char	*path;
	char	**params;
	int		code;

	(void)n;
	path = NULL;
	if (!ft_strcmp((char *)cmd->params->content, "."))
		return (exit_(-1, "bash: .: filename arg required\n", NULL, NULL, NULL, NULL), -1);
	code = ft_find_path(cmd->params->content, env, &path);
	if (!path)
		return (code);
	params = ft_construct_command(cmd->params);
	if (!params)
		return (free(path), 255);
	execve(path, params, ft_construct_command(env));
	free(params);
	free(path);
	path = (char *)cmd->params->content;
	if (ft_strchr(path, '/'))
		exit_(-1, "bash: %s: no such file or directory\n", path, NULL, NULL, NULL);
	else
		exit_(-1, "bash: %s: command not found\n", path, NULL, NULL, NULL);
	return (code);
}
