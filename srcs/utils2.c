#include "minishell.h"

char *part_after_sign_equal(char *s) //// malloc ?
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

char *part_before_sign_equal(char *s) //// malloc ?
{
	int		i;
	char	*s_dup;

	i = -1;
	while (s[++i] != '\0')
	{
		if (s[i] == '=')
			{
				s_dup = ft_strdup(s);
				if (s_dup == NULL)
					return (NULL);
				s_dup[i] = '\0';
				return (s_dup);
			}
	}
	return (NULL);
}