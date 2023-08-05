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

/* SIGQUIT = SIGINT, except that it’s controlled by C-\ 
// and produces a core dump when it terminates the process, 
// just like a program error signal. You can think of 
// this as a program error condition “detected” by the user. */

/* readline read a line from the terminal and return it, using prompt as a prompt.
If prompt is NULL or the empty string, no prompt is issued.
The line is allocated with malloc, the caller must free it when finished.
The line has the final newline removed.
Returns the text of the line read ("" for a blank line, NULL for a blanc line + EOF)
If an EOF is read with a non-empty line, it is treated as a newline. */

/* linked list, where:
 * list->content	- full string (ex. "USER=akostrik")
 * list->type      - default or not */

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
			ft_error();
			return ;
		}
		token = ft_lstnew(str, 0);
		if (!token)
		{
			free(str);
			ft_lstclear(&token, &free);
			ft_error();
			return ;
		}
		ft_lstadd_back(&data->env, token);
		i++;
	}
	data->node = NULL;
	data->exit_code = 0;
}

int	ft_parsing(char *command, t_list *env, t_data *data)
{
	t_list	*head;
	char	*cmd_line;
	int		code;

	data->node = NULL;
	cmd_line = ft_add_spaces(command);
	if (!cmd_line)
		return (data->exit_code = 255, 255);
	head = ft_tokenization(cmd_line, env, data);
	free(cmd_line);
	if (!head)
		return (data->exit_code = 255, 255);
	ft_assign_types(head);
	if (ft_check_tokens(head) == 0)
		return (data->exit_code = 2, ft_clean_redirections(*(&head)), 2);
	code = ft_open_heredocs(head, env);
	if (code != 0)
		return (data->exit_code = code, ft_clean_redirections(*(&head)), code);
	data->node = ft_make_tree(head, NULL);
	if (!data->node)
		return (data->exit_code = 255, ft_clean_redirections(*(&head)), 255);
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
		if (!command)
			break ;
		if (g_signal == 1)
		{
			g_signal = 0;
			data.exit_code = 130;
			continue;
		}
		add_history(command);
		data.exit_code = ft_parsing(command, data.env, &data);
		if (data.exit_code == 0)
			ft_execution(&data);
		ft_clean_tree(data.node);
	}
	ft_clean_redirections(*(&(data.env)));
	return (data.exit_code);
}
