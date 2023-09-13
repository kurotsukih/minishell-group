/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_utils_parse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:47 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/12 16:15:20 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

int	init_minishell(int argc, char **argv, char **env, t_data **d) // **d ?
{
	(void)argc;
	(void)argv;
	*d = (t_data *)malloc_(sizeof(t_data), d);
	(*d)->env = arr_to_lst(env, d);
	(*d)->redir = ""; // no need ?
	(*d)->token = "";
	(*d)->saved_stdin = dup(STDIN_FILENO); // if fail s ?
	(*d)->saved_stdout = dup(STDOUT_FILENO);
	(*d)->exit_c = 0;
	if (signal(SIGINT, &sig_handler) == SIG_ERR) 
		free_all_and_exit("Could not set signal handler", -1, d);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR) 
		free_all_and_exit("Could not set signal handler", -1, d);
	return (OK);
}

int	init_new_line(char *s, t_data **d)
{
	if (all_quotes_are_closed(s) != OK)
		return (err_cmd("uncloses quotes", 1, d)); // 1 ?
	(*d)->i = 0;
	return (OK);
}

int	init_cmd(t_data **d)
{
	int	fd_tmp;

	del_all_from_lst((*d)->args);
	(*d)->there_are_redirs_out = NO;
	(*d)->fd_in = -1;
	fd_tmp = open(TMP_FILE, O_RDONLY);
	if (fd_tmp == -1)
		(*d)->fd_in = dup(STDIN); // close ?
	else
		(*d)->fd_in = fd_tmp;
	(*d)->fd_out = dup(STDOUT);
	if ((*d)->fd_in == -1 || (*d)->fd_out == -1)
		return (err_cmd("dup pb", -1, d));
	return (OK);
}

void	init_token(t_data **d)
{
	(*d)->redir = "";
	(*d)->token = ""; // free((*d)->token) will work ???
}

int	all_quotes_are_closed(char *cmd_line)
{
	int		mod;
	int		i;

	mod_(REINIT_QUOTES);
	i = -1;
	while (cmd_line[++i] != '\0')
		mod = mod_(cmd_line[i]);
	if (mod != QUOTES0)
		return (FAILURE);
	return (OK);
}

int	skip_spaces(char *cmd_line, t_data **d)
{
	int	there_are_skipped_spaces;

	there_are_skipped_spaces = NO;
	while (cmd_line[(*d)->i] == ' ')
	{	
		there_are_skipped_spaces = YES;
		((*d)->i)++;
	}
	return (there_are_skipped_spaces);
}

void	calc_redir(char *cmd_line, t_data **d)
{
	char	*redir;

	if (cmd_line[(*d)->i]== '\0')
		redir = "";
	else if (cmd_line[(*d)->i] == '>' && cmd_line[(*d)->i + 1] != '>')
		redir = ">";
	else if (cmd_line[(*d)->i] == '<' && cmd_line[(*d)->i + 1] != '<')
		redir = "<";
	else if (cmd_line[(*d)->i] == '>' && cmd_line[(*d)->i + 1] == '>')
		redir = ">>";
	else if (cmd_line[(*d)->i] == '<' && cmd_line[(*d)->i + 1] == '<')
		redir = "<<";
	else
		redir = "";
	(*d)->redir = redir;
	((*d)->i) += ft_strlen((*d)->redir);
}

char	*calc_token(char *stop, char *s, t_data **d)
{
	char	*token;
	int		i;

	i = 0;
	while (1)
	{
		if (s[i] == '\0' || char_is_in_str(s[i], stop))
			break ;
		i++;
	}
	token = (char *)malloc_(i + 1, d);
	i = 0;
	while (1)
	{
		if (s[i] == '\0' || char_is_in_str(s[i], stop))
			break ;
		token[i] = s[i];
		i++;
	}
	token[i] = '\0';
	return (token);
}

int	heredoc_to_file(char *delim, t_data **d)
{
	char	*line;
	int		fd_write;

	fd_write = open(TMP_FILE_H, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (!fd_write)
		return (err_cmd("heredoc open tmp file pb", -1, d));
	line = NULL;
	while (1)
	{
		line = readline(">");
		if (line == NULL || ft_strcmp(line, delim) == 0)
			break ;
		write_fd(fd_write, line);
		write_fd(fd_write, " ");
		free_(line);
	}
	close(fd_write);
	return (OK);
}

int	put_fd_if_the_out_is_pipe(char *cmd_line, t_data **d)
{
	if ((*d)->there_are_redirs_out == NO && cmd_line[(*d)->i] == '|')
		(*d)->fd_out = open(TMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if ((*d)->fd_out == -1)
		return (err_cmd("dup pb", -1, d));
	return (OK);
}

int char_is_in_str(char c, char *s)
{
	int	i;

	if (s == NULL)
		return (NO);
	i = -1;
	while (s[++i] != '\0')
		if (s[i] == c)
			return (YES);
	return (NO);
}

// to detect if we are inside of quotes or not
int	mod_(char c)
{
	static char	mod = QUOTES0;

	if (c == REINIT_QUOTES)
		mod = QUOTES0;
	else if (mod == QUOTES0 && c == '\'')
		mod = QUOTES1;
	else if (mod == QUOTES0 && c == '\"')
		mod = QUOTES2;
	else if (mod == QUOTES1 && c == '\'')
		mod = QUOTES0;
	else if (mod == QUOTES2 && c == '\"')
		mod = QUOTES0;
	return (mod);
}
