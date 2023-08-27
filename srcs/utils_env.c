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

char	**env_to_array(t_data **d)
{
	int		i;
	char	**env_array;
	t_env	*var;

	env_array = (char **)malloc_(len_env(d) + 1 *sizeof(char *), d);
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

int len_env(t_data **d)
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
			return (val_(var->var));
		var = var->nxt;
	}
	return (NULL);
}
