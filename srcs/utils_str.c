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

void strdup_and_trim(char *src, char **dest0, int len, t_data **d)
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
	*dest0 = dest;
}

char	*strdup_and_erase_redirs(char *s0, int len) // enlever len
{
	int		i;
	char	*s;

	i = 0;
	mod_(REINIT_QUOTES);
	s = ft_strdup(s0);
	while (i < len) // erase redirections
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

void	free_charchar(char **s, int len)
{
	int	i;

	i = 0;
	while (i < len)
		free(s[i]);
	free(s);
}