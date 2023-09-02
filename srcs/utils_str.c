/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:52 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/02 18:32:10 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	int		i_beg;
	int		i_end;
	int		new_len;
	char	*dest;

	i = 0;
	while (src[i] == ' ' && src[i] != '\0')
		i++;
	i_beg = i;
	i = len - 1;
	while (src[i] == ' ' && i >= 0)
		i--;
	i_end = i;
	new_len = i_end - i_beg + 1;
	dest = (char *)malloc_(new_len + 1, d);
	i = 0;
	while (i < new_len)
	{
		dest[i] = src[i_beg + i];
		i++;
	}
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

void	free_env_array(char **env, int len)
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

	dup = (char *)malloc_(ft_strlen(s) + 1, d);
	i = -1;
	while (s[++i] != '\0')
		dup[i] = s[i];
	dup[i] = '\0';
	return (dup);
}

int	strcmp_(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0'|| s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

