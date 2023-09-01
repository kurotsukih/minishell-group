#include "headers.h"

char *alphanum_(char *s, t_data **d)
{
	int		i;
	char	*alphanum;

	if ((s[0] < 'a' || s[0] > 'z') && (s[0] < 'A' && s[0] > 'Z'))
		return (NULL);
	i = -1;
	while ((s[++i] >= '0' && s[i] < '9') || (s[i] >= 'a' && s[i] < 'z') || (s[i] >= 'A' && s[i] < 'Z') || s[i] == '_')
		;
	alphanum = (char *)malloc_(i + 1, d);
	i = -1;
	while ((s[++i] >= '0' && s[i] < '9') || (s[i] >= 'a' && s[i] < 'z') || (s[i] >= 'A' && s[i] < 'Z') || s[i] == '_')
		alphanum[i] = s[i];
	alphanum[i] = '\0';
	return (alphanum);
}

char	*strndup_and_trim(char *src, int len, t_data **d)
{
	int		i;
	char	*dest;

	while (src[0] == ' ' && src[0] != '\0')
	{
		src++;
		len--;
	}
	i = len;
	while (src[--i] == ' ' && i >= 0)
	{
		src[i] = '\0';
		len--;
	}
	dest = (char *)malloc_(len + 1, d);
	i = -1;
	while (++i < len)
		dest[i] = src[i];
	dest[i] = '\0';
	return (dest);
}

char	*strdup_and_erase_redirs(char *s0, t_data **d) // len useful
{
	int		i;
	char	*s;

	i = 0;
	mod_(REINIT_QUOTES);
	s = strdup_(s0, d);
	while (s[i] != '\0')
	{
		if (mod_(s[i]) == QUOTES0 && ft_strlen(redir_(&s[i])) > 0)
		{
			s[i++] = ' ';
			if (s[i] == '>' || s[i] == '<')
				s[i++] = ' ';
			while(s[i] == ' ')
				i++;
			while(mod_(s[i]) == QUOTES0 && (s[i] != ' ' && s[i] != '>' && s[i] != '<' && s[i] != '\0'))
				s[i++] = ' ';
		}
		else
			i++;
	}
	return (s);
}

char	*strdup_and_erase_args_except_redirs(char *s0, t_data **d) // enlever len
{
	int		i;
	char	*s;

	i = 0;
	mod_(REINIT_QUOTES);
	s = strdup_(s0, d);
	while (s[i] != '\0')
	{	
		if (mod_(s[i]) == QUOTES0 && ft_strlen(redir_(&s[i])) > 0)
		{
			if (s[i] == '>' || s[i] == '<')
				i++;
			while(s[i] == ' ')
				i++;
			while(mod_(s[i]) == QUOTES0 && (s[i] != ' ' && s[i] != '>' && s[i] != '<' && s[i] != '\0'))
				i++;
		}
		else
			s[i++] = ' ';
	}
	return (s);
}

void	free_array_env(char **env, int len)
{
	int	i;

	i = 0;
	while (i < len)
		free(env[i]);
	free(env);
}

char	*strdup_(char *s, t_data **d)
{
	size_t	i;
	char	*dup;

	dup = (char *)malloc_((ft_strlen(s) + 1) * sizeof(char), d);
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
