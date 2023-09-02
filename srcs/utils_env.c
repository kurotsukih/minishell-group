#include "headers.h"

char *key_(char *s, t_data **d)
{
	char	*key;
	int		i;
	int		len_key;

	len_key = 0;
	i = -1;
	while (s[++i] != '=' && s[i] != '\0')
		len_key ++;
	key = (char *)malloc_(len_key + 1, d);
	i = 0;
	while (i < len_key)
	{
		key[i] = s[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

char *val_(char *s, t_data **d)
{
	int		i;
	char	*val;
	int		len_key;
	int		len_val;

	i = -1;
	while (s[++i] != '\0')
		if (s[i] == '=')
			break ;
	if (s[i] != '=')
		return (NULL);
	len_key = i;
	len_val = 0;
	while (s[++i] != '\0')
		len_val++ ;
	val = (char *)malloc_(len_val + 1, d);
	i = 0;
	while (s[len_key + i + 1] != '\0')
	{
		val[i] = s[len_key + i + 1];
		i++;
	}
	val[i] = '\0';
	return (val);
}

char	*get_value_from_env(char *key, t_data **d)
{
	t_env	*var;

	var = *((*d)->env);
	while (var != NULL)
	{
		if (ft_strncmp(key, var->var, ft_strlen(key)) == 0)
			return (&(var->var[(int)ft_strlen(key) + 1]));
		var = var->nxt;
	}
	(*d)->curr_cmd->err = "env variable not found";
	return (NULL);
}

char	**env_to_array(t_data **d)
{
	int		i;
	char	**env_array;
	t_env	*var;

	env_array = (char **)malloc_((len_env_(d) + 1) *sizeof(char *), d);
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
