#include "headers.h"

char *key_(char *s)
{
	int		i;
	char	*s_copy;

	s_copy = (char *)malloc(ft_strlen(s));
	if (s_copy == NULL)
		return (NULL);
	i = -1;
	while (s[++i] != '\0')
	{
		s_copy[i] = s[i];
		if (s[i] == '=')
		{
			s_copy[i] = '\0';
			return (s_copy);
		}
	}
	return (NULL);
}

char *val_(char *s)
{
	int		i;
	char	*s_dup;

	i = -1;
	while (s[++i] != '\0')
	{
		if (s[i] == '=')
		{
			s_dup = ft_strdup(&s[i + 1]);
			if (s_dup == NULL)
				return (NULL);
			return (s_dup);
		}
	}
	return (NULL);
}

int env_to_list(char **env_main, t_env ***env_list)
{
	int		i;
	t_env	*new;

	*env_list = NULL;
	*env_list = (t_env **)malloc(sizeof(t_env *));
	if (*env_list == NULL)
		return (-1);
	**env_list = NULL;
	i = -1;
	while (env_main[++i])
	{
		new = NULL;
		new = (t_env *)malloc(sizeof(t_env));
		if (new == NULL)
			return (-1);
		new->var = env_main[i];
		if (new->var == NULL)
			return (-1);
		new->nxt = **env_list;
		**env_list = new;
	}
	return (0);
}

char	**env_to_array(t_env **env_list)
{
	int		i;
	char	**env_array;
	t_env	*var;

	env_array = (char **)malloc((len_list(env_list) + 1) *sizeof(char *));
	if (*env_array == NULL)
		return (NULL);
	i = -1;
	var = *env_list;
	while (1)
	{
		env_array[++i] = var->var;
		if (var == NULL)
			break ;
		var = var->nxt;
	}
	return (env_array);
}
