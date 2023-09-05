/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_parse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:47 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/04 02:12:29 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

char	*redir_(char *s)
{
	if (s[0]== '\0')
		return (NULL);
	if (s[0] == '>' && s[1] != '>')
		return (">");
	if (s[0] == '<' && s[1] != '<')
		return ("<");
	if (s[0] == '>' && s[1] == '>')
		return (">>");
	if (s[0] == '<' && s[1] == '<')
		return ("<<");
	else
		return (NULL);
}

int	len_alphanum(char *s)
{
	int	i;

	if ((s[0] < 'a' || s[0] > 'z') && (s[0] < 'A' && s[0] > 'Z'))
		return( OK);
	i = -1;
	while ((s[++i] >= '0' && s[i] < '9') || (s[i] >= 'a' && s[i] < 'z') || (s[i] >= 'A' && s[i] < 'Z') || s[i] == '_')
		;
	return (i);
}

static int	is_digit(char c)
{
	return (c >= '0' && c < '9');
}

static int	is_letter(char c)
{
	return ((c >= 'a' && c < 'z') || (c >= 'A' && c < 'Z'));
}

char	*alphanum_(char *s, t_data **d)
{
	int		i;
	char	*alphanum = NULL; //

	if (s == NULL || (!is_digit(s[0]) && !is_letter(s[0])))
		return (NULL);
	alphanum = (char *)malloc_(len_alphanum(s) + 1, d);
	i = -1;
	while (is_digit(s[++i]) || is_letter(s[i]) || s[i] == '_')
		alphanum[i] = s[i];
	alphanum[i] = '\0';
	return (alphanum);
}

int	nb_spaces(char *s)
{
	int	i;

	i = 0;
	while (s[i] == ' ')
		i++;
	return (i);
}

void calc_nb_args_and_outs(char *s, int len, t_data **d)
{
	int		i;
	char	*redir;

	(*d)->nb_args = 0;
	(*d)->nb_outs = 0;
	mod_(REINIT_QUOTES);
	i = -1;
	while (++i < len)
		if (mod_(s[i]) == QUOTES0)
		{
			i += nb_spaces(&s[i]);
			redir = redir_(&s[i]);
			if (ft_strcmp(redir, ">") == 0 || ft_strcmp(redir, ">>") == 0)
				((*d)->nb_outs)++;
			else if (ft_strcmp(redir, "<") == 0 || ft_strcmp(redir, "<<") == 0)
				;
			else
				((*d)->nb_args)++;
			i += ft_strlen(redir);
			i += nb_spaces(&s[i]);
			i += len_alphanum(&s[i]);
		}
	if ((*d)->nb_outs == 0)
		(*d)->nb_outs = 1;
	(*d)->arg = (char **)malloc_(((*d)->nb_args + 1) * sizeof(char *), d);
	(*d)->out = (int *)malloc_((*d)->nb_outs * sizeof(int), d);
	i = -1;
	while (++i < (*d)->nb_args + 1)
		(*d)->arg[i] = NULL;
	i = -1;
	while (++i < (*d)->nb_outs)
		(*d)->out[i] = 1;

}

int	heredoc_to_file(char *delim, t_data **d)
{
	char	*line;
	int		fd;

	fd = open(TMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	//if (!fd)
		// free all and exit ? return (printf("%s : tmp file pb\n", (*d)->arg[0]), -1); return OK
	line = NULL;
	while (1)
	{
		line = readline(">");
		if (line == NULL || strcmp_(line, delim) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, " ", 1);
		free_(line);
	}
	(void)d; //
	return (OK);
// void	ft_close_heredoc(int fd, int fd_cpy)
// {s
// 	close(fd);
// 	dup2(fd_cpy, STDIN_FILENO);
// 	close(fd_cpy);
// }
}
