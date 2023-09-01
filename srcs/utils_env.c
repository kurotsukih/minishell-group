#include "headers.h"

char *key_(char *s, t_data **d)
{
	int		i;
	char	*s_copy;

	s_copy = (char *)malloc_(ft_strlen(s), d);
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

char *val_(char *s, t_data **d)
{
	int		i;
	char	*s_dup;

	i = -1;
	while (s[++i] != '\0')
	{
		if (s[i] == '=')
		{
			s_dup = strdup_(&s[i + 1], d);
			if (s_dup == NULL)
				return (NULL);
			return (s_dup);
		}
	}
	return (NULL);
}

char	**env_to_array(t_data **d)
{
	int		i;
	char	**env_array;
	t_env	*var;

	env_array = (char **)malloc_(len_env_(d) + 1 *sizeof(char *), d);
	i = -1;
	var = *((*d)->env);
	while (var != NULL)
	{
		env_array[++i] = var->var;
		var = var->nxt;
	}
	env_array[++i] = NULL;
	return (env_array);
}

int len_env_(t_data **d)
{
	t_env	*var;
	int		len;

	len = 0;
	var = *((*d)->env);
	while (var != NULL)
	{
		len ++;
		var = var->nxt;
	}
	return (len);
}

char	*get_value_from_env(char *key, t_data **d)
{
	t_env	*var;

	var = *((*d)->env);
	while (var != NULL)
	{
		if (ft_strcmp(key_(var->var, d), key) == 0)
			return (val_(var->var, d));
		var = var->nxt;
	}
	(*d)->curr_cmd->err = "env variable not found";
	return (NULL);
}

void	put_paths_to_d(t_data **d)
{
	char	*paths_str;
	int		i;
	int		i_beg;
	int		k;

	paths_str = get_value_from_env("PATH", d);
	if (!paths_str || ft_strlen(paths_str) == 0)
		{
			return;
			// exit_code = 127)
		}
	(*d)->nb_paths = 1;
	i = -1;
	while (paths_str[++i] != '\0')
		if (paths_str[i] == ':')
			(*d)->nb_paths++;
	(*d)->paths = (char **)malloc_((*d)->nb_paths * sizeof(char *), d);
	i_beg = 0;
	i = -1;
	k = -1;
	while (paths_str[++i] != '\0')
		if (paths_str[i] == ':')
		{
			(*d)->paths[++k] = strndup_and_trim(&(paths_str[i_beg]), i - i_beg, d);
			i_beg = i + 1;
		}
	(*d)->paths[++k] = strndup_and_trim(&(paths_str[i_beg]), i - i_beg + 1, d);
}
