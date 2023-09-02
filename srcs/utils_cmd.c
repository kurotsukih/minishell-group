/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:47 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/02 17:57:37 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

void	init_cmd(t_cmd **new, t_data **d)
{
	*new = (t_cmd *)malloc_(sizeof(t_cmd), d);
	(*new)->arg = NULL;
	(*new)->fd_in = STDIN_FILENO;
	(*new)->fd_out = STDOUT_FILENO;
	(*new)->nxt = NULL;
	(*new)->prv = NULL;
	(*d)->curr_cmd = *new;
}

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

int	there_are_unclosed_quotes(t_cmd *cmd)
{
	int		mod;
	int		i;

	mod_(REINIT_QUOTES);
	i = -1;
	while (cmd->arg[0][++i] != '\0')
		mod = mod_(cmd->arg[0][i]);
	return (mod != QUOTES0);
}

char	*redir_(char *s)
{
	if (s[0] == '>' && s[1] == '>')
		return (">>");
	else if (s[0] == '<' && s[1] == '<')
		return ("<<");
	else if (s[0] == '>')
		return (">");
	else if (s[0] == '<')
		return ("<");
	else
		return (""); // NULL
}

int	nb_args_(char *s0, int len, t_data **d)
{
	int		nb_args;
	int		i;
	char	*s;

	s = strdup_and_erase_redirs(s0, d);
	mod_(REINIT_QUOTES);
	nb_args = 0;
	i = -1;
	while (++i < len)
		if (mod_(s[i]) == QUOTES0 && s[i] != ' ' && (s[i + 1] == ' ' || s[i + 1] == '\0' || s[i + 1] == '\'' || s[i + 1] == '\"' || i == len - 1))
				nb_args++;
	free(s);
	return (nb_args);
}

void print_cmds(char *msg, t_data **d)
{
	int		i;
	t_cmd	*cmd;

	printf("LIST %s %14p->%14p:\n  ", msg, (*d)->cmds, (*d)->cmds == NULL ? 0 : *((*d)->cmds));
	if ((*d)->cmds == NULL || *((*d)->cmds) == NULL)
	{
		printf("empty\n");
		return ;
	}
	cmd = *((*d)->cmds);
	while (cmd != NULL)
	{
		if (cmd->arg != NULL)
		{
			i = -1;
			while (++i < cmd->nb_args)
				printf("%s : ", cmd->arg[i]);
		}
		else
			printf("args = NULL");
		printf(" fd_in = %d, fd_out = %d\n", cmd->fd_in, cmd->fd_out);
		cmd = cmd->nxt;
	}
	printf("\n");
}

void del_cmd_from_lst(t_cmd *cmd, t_data **d)
{
	int		i;
	t_cmd	*to_free;

	if (cmd == NULL)
		return ;
	i = -1;
	while(++i < cmd->nb_args)
	{
		free(cmd->arg[i]);
		cmd->arg[i] = NULL;
	}
	free(cmd->arg);
	cmd->arg = NULL;
	to_free = cmd;
	if (cmd->nxt != NULL)
		cmd->nxt->prv = cmd->prv;
	if (cmd->prv == NULL)
		*((*d)->cmds) = cmd->nxt;
	else
		cmd->prv->nxt = cmd->nxt;
	free(to_free); // &to_free ?
	(*d)->curr_cmd = NULL;
	to_free = NULL;
}

void	del_cmds(t_data **d)
{
	t_cmd	*cmd;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		del_cmd_from_lst(cmd, d);
		cmd = cmd->nxt;
	}
}

static void	put_heredoc_to_tmp_file(char *delimiter, char *filename, t_cmd *cmd, t_data **d)
{
	char	*line;
	int		fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (!fd)
		return (printf("error open file failed"), del_cmd_from_lst(cmd, d)); // exic code ?
	line = NULL;
	while (1)
	{
		line = readline(">");
		if (line == NULL || strcmp_(line, delimiter) != 0)
			break ;
		if (line == NULL)
			break ;
		// ft_remove_quotes_string(line);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

void	*open_file(char *redir, char *filename, t_cmd *cmd, t_data **d)
{
	//if ((**d)->saved_stdin == -1)
	if (ft_strlen(filename) == 0)
			return (printf("%s : erreur de syntaxe près du symbole inattendu « > »\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); 	// exic code ?
	if (strcmp_(redir, "<") == 0)
	{
		if (cmd->fd_in != STDIN_FILENO)
			close(cmd->fd_in);
		cmd->fd_in = open(filename, O_RDONLY);
		if (!cmd->fd_in)
			return (printf("%s : open file failed\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); 	// exic code ?
	}
	else if (strcmp_(redir, "<<") == 0)
	{
		put_heredoc_to_tmp_file(filename, "tmp_file", cmd, d); // delimitor = filename
		if (cmd->fd_in != STDIN_FILENO)
			close(cmd->fd_in);
		cmd->fd_in = open("tmp_file", O_RDONLY);
		if (!cmd->fd_in)
			return (printf("%s : open file failed\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); 	// exic code ?
	}
	else if (strcmp_(redir, ">") == 0)
	{
		if (cmd->fd_out != STDIN_FILENO)
			close(cmd->fd_out);
		cmd->fd_out = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (!cmd->fd_out)
			return (printf("%s : open file failed\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); 	// exic code ?
	}
	else if (strcmp_(redir, ">>") == 0)
	{
		if (cmd->fd_out != STDIN_FILENO)
			close(cmd->fd_out);
		cmd->fd_out = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (!cmd->fd_out)
			return (printf("%s : open file failed\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); 	// exic code ?
	}
	return (NULL);
}

static char	*s_with_conversion_(char *old_s, int j, t_data **d)
{
	char	*new_s;
	int		len_new_s;
	char	*key;
	char	*val;
	int		k;

	key = alphanum_(&(old_s[j + 1]), d);
	val = get_value_from_env(key, d);
	len_new_s = ft_strlen(old_s) - ft_strlen(key) + ft_strlen(val);
	new_s = (char*)malloc_(len_new_s + 1, d);
	k = -1;
	while (++k < j)
		new_s[k] = old_s[k];
	k--; //
	while (++k < j + (int)ft_strlen(val))
		new_s[k] = val[k - j];
	k--;
	while(++k < len_new_s)
		new_s[k] = old_s[k + (int)ft_strlen(key) - (int)ft_strlen(val) + 1];
	new_s[k] = '\0';
	free(key);
	return (new_s);
}

void	calc_dollar_conversions(t_cmd *cmd, t_data **d)
{
	char	*s_with_conversion;
	int		i;
	int		j;

	(*d)->curr_cmd = cmd;
	i = 0;
	while(++i < cmd->nb_args)
		if (cmd->arg[i][0] != '\'')
		{
			j = -1;
			while (cmd->arg[i][++j] != '\0' && cmd->arg[i][j + 1] != '\0')
			{
				// if (cmd->arg[i][j] == '$' && cmd->arg[i][j + 1] == '?')
				// 	(ft_itoa(exit_code));
				if (cmd->arg[i][j] == '$')
				{
					s_with_conversion = s_with_conversion_(cmd->arg[i], j, d);
					free(cmd->arg[i]);
					cmd->arg[i] = s_with_conversion;
				}
			}
		}
}

static void	*verif_args_1(t_cmd *cmd, t_data **d)
{
	if (!cmd->arg)
		return (printf("empty command\n"), del_cmd_from_lst(cmd, d), NULL); // exit_code = 255
	else if (there_are_unclosed_quotes(cmd))
		return (printf("%s : unclosed quotes\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); // exit_code
	else if (strcmp_(cmd->arg[0], "env") == 0 && cmd->nb_args >= 2)
		return (printf("%s : Too many arguments\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); // exit_code
	else if (strcmp_(cmd->arg[0], "cd") == 0 && cmd->nb_args >= 3)
		return (printf("%s : Too many arguments\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); // exit_code
	else if (strcmp_(cmd->arg[0], "exit") == 0 && cmd->nb_args >= 3)
		return (printf("%s : Too many arguments\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); // exit_code
	else if (strcmp_(cmd->arg[0], "exit") == 0 && cmd->nb_args == 2 && !ft_atoi(cmd->arg[1]))
		return (printf("%s : numeric argument required\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); // exit_code = 2
	return (NULL);
}

void	verif_args(t_data **d)
{
	t_cmd	*cmd;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		// (*d)->curr_cmd = cmd; // not used 
		verif_args_1(cmd, d);
		cmd = cmd->nxt;
	}
}

int	is_builtin(t_cmd *cmd)
{
	return (\
	strcmp_(cmd->arg[0], "cd") == 0 || \
	strcmp_(cmd->arg[0], "exit") == 0 || \
	strcmp_(cmd->arg[0], "export") == 0 || \
	strcmp_(cmd->arg[0], "unset") == 0 || \
	strcmp_(cmd->arg[0], "echo") == 0 || \
	strcmp_(cmd->arg[0], "pwd") == 0 || \
	strcmp_(cmd->arg[0], "env") == 0);
}

static char	*path2_(char *s1, char *s2, t_data **d)
{
	int		i;
	int		s1_len;
	int		s2_len;
	char	*new_str;

	s1_len = 0;
	if (s1 != NULL)
		s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new_str = (char *)malloc_(s1_len + s2_len + 2, d);
	if (new_str == NULL)
		return (NULL);
	i = 0;
	while (i < s1_len)
	{
		new_str[i] = s1[i];
		i++;
	}
	if(s1 != NULL)
	{
		new_str[i] = '/';
		i++;
	}
	i = 0;
	while (i < s2_len)
	{
		new_str[s1_len + 1 + i] = s2[i];
		i++;
	}
	new_str[s1_len + 1 + i] = '\0';
	return (new_str);
}

char	*path_(t_cmd *cmd, t_data **d)
{
	char	*paths_str;
	char	*path;
	int		i;
	int		i_beg;

	paths_str = get_value_from_env("PATH", d);
	i_beg = 0;
	i = -1;
	while (++i < (int)ft_strlen(paths_str))
		if (paths_str[i] == ':' || paths_str[i] == '\0')
		{
			path = strndup_and_trim(&(paths_str[i_beg]), i - i_beg, d);
			path = path2_(path, cmd->arg[0], d);
			if (access(path, X_OK) == 0)
				return (path);
			free(path);
			i_beg = i + 1;
		}
	return (printf("%s : command not found\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); // exit_code = 127, if (errno != 2) exit_c = 126;
}

void	*start_redirs(t_cmd *cmd, t_data **d)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1) // дубл. дескриптора => stdout в файл
			return (printf("%s : dup@ fqiled\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); // exit_code = 127, if (errno != 2) exit_c = 126;
		close(cmd->fd_in);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1) // дубл. дескриптора => stdout в файл
			return (printf("%s : dup@ fqiled\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); // exit_code = 127, if (errno != 2) exit_c = 126;
		close(cmd->fd_out);
	}
	return (NULL);
}

void	*stop_redirs(t_cmd *cmd, t_data **d)
{
	if (cmd->fd_in == STDIN_FILENO)
		if (dup2((*d)->saved_stdin, STDIN_FILENO) == -1) // восстановить исходный stdout
			return (printf("%s : dup@ fqiled\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); // exit_code = 127, if (errno != 2) exit_c = 126;
	if (cmd->fd_out == STDOUT_FILENO)
		if (dup2((*d)->saved_stdout, STDOUT_FILENO) == -1) // восстановить исходный stdout
			return (printf("%s : dup@ fqiled\n", cmd->arg[0]), del_cmd_from_lst(cmd, d), NULL); // exit_code = 127, if (errno != 2) exit_c = 126;
	return (NULL);
}
