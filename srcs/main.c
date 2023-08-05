/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akalimol <akalimol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 17:31:49 by akalimol          #+#    #+#             */
/*   Updated: 2023/07/08 19:32:59 by akalimol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
readline rl_clear_history, rl_on_new_line rl_replace_line rl_redisplay, add_history,
malloc free
printf write access open read close
fork wait waitpid wait3 wait4 
signal sigaction sigemptyset sigaddset kill
exit,
getcwd chdir stat lstat fstat unlink execve dup dup2 pipe
opendir readdir closedir
strerror perror
isatty ttyname ttyslot ioctl
getenv
tcsetattr tcgetattr tgetent tgetflag tgetnum tgetstr tgoto tputs

SIGQUIT = SIGINT, except that it’s controlled by C-\ 
and produces a core dump when it terminates the process, 
just like a program error signal. You can think of 
this as a program error condition “detected” by the user.

readline read a line from the terminal 
The line is allocated with malloc, the caller must free it.
Returns the line without \n, "" for "", NULL for "" + EOF
If an EOF is read with a non-empty line, it is treated as a newline.

linked list, where:
list->content	- full string (ex. "USER=akostrik")
list->type      - default or not */

#include "minishell.h"

int g_signal = 0;

void	init(char **env, t_data *data)
{
	t_list	*token;
	char	*str;
	int		i;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sig_handler_main);
	data->env = NULL;
	i = 0;
	while (env[i])
	{
		str = ft_strdup(env[i]);
		if (!str)
		{
			ft_lstclear(&data->env, &free);
			ft_error_exit(-1, NULL);
			return ;
		}
		token = ft_lstnew(str, 0);
		if (!token)
		{
			free(str);
			ft_lstclear(&token, &free);
			ft_error_exit(-1, NULL);
			return ;
		}
		ft_lstadd_back(&data->env, token);
		i++;
	}
	data->exit_code = 0;
}

t_list	*ft_token_error(t_list *token)
{
	ft_error_exit(-1, NULL);
	ft_lstclear(&token, &free);
	return (NULL);
}

t_list	*ft_token_merror(char *str, t_list *token)
{
	ft_merror(str, NULL);
	ft_lstclear(&token, &free);
	return (NULL);
}

static void	ft_remove_quotes(t_list *head)
{
	t_list	*token;
	char	*str;

	token = head;
	while (token)
	{
		str = (char *)token->content;
		ft_remove_quotes_string(str);
		token = token->next;
	}
}

t_list	*ft_add_token(char *str, int i_beg, int i_end, t_data *data)
{
	char	*new_str;
	t_list	*node;
	int		i;

	if (i_beg == i_end)
		i_end++;
	new_str = (char *)malloc(sizeof(char) * (i_end - i_beg + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < i_end - i_beg)
	{
		new_str[i] = str[i + i_beg];
		i++;
	}
	new_str[i] = '\0';
	if (ft_strchr(new_str, '$'))
		node = ft_expand_token(new_str, data->env, data);
	else
		node = ft_lstnew(new_str, 0);
	if (!node)
		return (NULL);
	return (node);
}

int	ft_is_token(char c, int checker)
{
	static char	mode;

	if (checker == 1)
	{
		if (mode == 0)
			return (1);
		return (mode = 0);
	}
	else if (mode == 'd' || (c == '\"' && mode == 0))
	{
		if (mode == 0)
			mode = 'd';
		else if (c == '\"')
			mode = 0;
	}
	else if (mode == 's' || (c == '\'' && mode == 0))
	{
		if (mode == 0)
			mode = 's';
		else if (c == '\'')
			mode = 0;
	}
	else if (ft_isspace(c) == 1)
		return (0);
	return (1);
}

// separate each key word into token
// cat || ls  -> ['cat', '|', 'ls']
t_list	*ft_tokenization(char *str, t_list *env, t_data *data)
{
	t_list	*head;
	int		i_beg;
	int		i_end;
	t_list	*token;

	(void)env;
	i_beg = 0;
	i_end = 0;
	head = NULL;
	while (str[i_beg])
	{
		i_end = i_beg;
		while (str[i_end] && ft_is_token(str[i_end], 0))
			i_end++;
		token = ft_add_token(str, i_beg, i_end, data);
		if (!token)
			return (ft_token_error(head));
		ft_lstadd_back(&head, token);
		i_beg = i_end + 1;
	}
	if (ft_is_token(0, 1) == 0)
		return (ft_token_merror("BASH: unclosed quotes\n", head));
	ft_remove_quotes(head);
	return (head);
}

int	parse(char *command, t_list *env, t_data *data)
{
	t_list	*head;
	char	*cmd_line;
	int		code;

	cmd_line = ft_add_spaces(command);
	if (!cmd_line)
		return (data->exit_code = 255, 255);
	head = ft_tokenization(cmd_line, env, data);
	free(cmd_line);
	if (!head)
		return (data->exit_code = 255, 255);
	ft_assign_types(head);
	if (ft_check_tokens(head) == 0)
		return (data->exit_code = 2, free_redirections(*(&head)), 2);
	code = ft_open_heredocs(head, env);
	if (code != 0)
		return (data->exit_code = code, free_redirections(*(&head)), code);
	data->node = NULL;
	data->node = ft_make_tree(head, NULL);
	if (data->node == NULL)
		return (data->exit_code = 255, free_redirections(*(&head)), 255);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	char	*command;

	(void)argc;
	(void)argv;
	command = NULL;
	init(env, &data);
	while (1)
	{
		command = readline("$");
		if (command == NULL) // EOF
			break ;
		if (g_signal == 1)
		{
			g_signal = 0;
			data.exit_code = 130;
			continue;
		}
		add_history(command);
		data.exit_code = parse(command, data.env, &data);
		if (data.exit_code == 0)
			ft_execution(&data);
		ft_clean_tree(data.node);
	}
	free_redirections(*(&(data.env)));
	return (data.exit_code);
}
