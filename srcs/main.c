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
 * @brief 		Initilize the environment variables.
 * 
 * @example		They are going to be saved inside of linked list, where:
 * 
 * 					list->content	- full string (ex. "USER=akalimol")
 *        			list->type      - default or not
 * 
 * @param data_env 	Address of env data. &data->env
 * @param env 		environment variables passed from program
 * 
 * @return int 		0	- if everything ok
 * 					-1	- if malloc failed
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

void	ft_init_data(int argc, char **argv, char **env, t_data *data)
{
	(void)argv;
	(void)argc;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &ft_signal);
	data->env = NULL;
	ft_init_env(&data->env, env);
	data->node = NULL;
	data->exit_code = 0;
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	char	*command;

	command = NULL;
	ft_init_data(argc, argv, env, &data);
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
