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

#include "minishell.h"

int	g_signal = 0;

/**
 * @example		They are going to be saved inside of linked list, where:
 * 					list->content	- full string (ex. "USER=akalimol")
 *        			list->type      - default or not
 */
int	ft_init_env(t_list **data_env, char **env)
{
	t_list	*token;
	char	*str;
	int		i;

	i = 0;
	while (env[i])
	{
		str = ft_strdup(env[i]);
		if (!str)
		{
			ft_lstclear(data_env, &free);
			ft_error();
			return (-1);
		}
		token = ft_lstnew(str, 0);
		if (!token)
		{
			free(str);
			ft_lstclear(&token, &free);
			return (ft_error(), -1);
		}
		ft_lstadd_back(data_env, token);
		i++;
	}
	return (0);
}

void	ft_signal(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = 1;
	}
}

void	ft_init_data(char **env, t_data *data)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &ft_signal);
	data->env = NULL;
	ft_init_env(&data->env, env);
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
		return (data->exit_code = 2, ft_clean_tokens(&head, &free), 2);
	// code = ft_extend_wildcard(&head);
	// if (code == 0)
		// return (data->exit_code = code, ft_clean_tokens(&head, &free), code);
	code = ft_open_heredocs(head, env);
	if (code != 0)
		return (data->exit_code = code, ft_clean_tokens(&head, &free), code);
	data->node = ft_make_tree(head, NULL);
	if (!data->node)
		return (data->exit_code = 255, ft_clean_tokens(&head, &free), 255);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	char	*command;

	(void)argc;
	(void)argv;
	command = NULL;
	ft_init_data(env, &data);
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
	ft_clean_env(data.env);
	return (data.exit_code);
}
