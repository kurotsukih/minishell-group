#include "minishell.h"

int	ft_isnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]) != 1)
			return (0);
		i++;
	}
	return (1);
}

int	ft_abs(int num)
{
	if (num < 0)
		return (num * -1);
	return (num);
}

int	ft_strcmp_alt(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (1);
	i = 1;
	while (str[i] && str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (0);
	return (1);
}

void	*free_charchar(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		free(s[i++]);
	free(s);
	return (NULL);
}

