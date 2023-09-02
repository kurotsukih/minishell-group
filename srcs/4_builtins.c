/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_builtins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:39 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/02 21:56:44 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

void	*exec_echo(t_cmd *cmd)
{
	int	option_n;
	int	i;

	option_n = 0;
	i = 0;
	while (++i < cmd->nb_args)
	{
		if (strcmp(cmd->arg[i], "-n") == 0)
			option_n = 1;
		else if (i < cmd->nb_args - 1)
			printf("%s ", cmd->arg[i]);
		else
			printf("%s", cmd->arg[i]);
	}
	if (option_n == 0)
		printf("\n");
	return (NULL);
}

void	*exec_cd(t_cmd *cmd, t_data **d)
{
	char	*dir;

	dir = NULL;
	if (cmd->nb_args == 1)
	{
		dir = get_value_from_env("HOME", d);
		if (dir == NULL)
			return (printf("%s : variable HOME not found\n", cmd->arg[0]), rmv_cmd(cmd, d), NULL); 	// exic code ?
		if (chdir(dir) == -1)
			return (printf("%s : chdir failure\n", cmd->arg[0]), rmv_cmd(cmd, d), NULL); 	// exic code ?
	}
	if (dir != NULL)
		free(dir);
	return (NULL);
}

void	*exec_pwd(t_cmd *cmd, t_data **d)
{
	char	*s;

	s = getcwd(NULL, 0);
	if (s == NULL)
		return (printf("%s : getcwd failed\n", cmd->arg[0]), rmv_cmd(cmd, d), NULL); 	// exic code ?
	printf("%s\n", s);
	free(s);
	return (NULL);
}

void	*exec_export(t_cmd *cmd, t_data **d)
{
	t_env	*new_var;
	int		i;

	if (cmd->nb_args == 0)
		return (exec_env(d), NULL);
	exec_unset(cmd, d);
	i = 0;
	while (++i < cmd->nb_args)
	{
		new_var = (t_env *)malloc_(sizeof(t_env), d);
		new_var->var = strdup_(cmd->arg[i], d);
		if ((*d)->env != NULL)
			new_var->nxt = *((*d)->env);
		else
			new_var->nxt = NULL;
		*((*d)->env) = new_var;
	}
	return (NULL);
}

void	*exec_unset(t_cmd *cmd, t_data **d)
{
	t_env	*var;
	t_env	*prv;
	char	*key;
	int		i;

	if ((*d)->env == NULL)
		return (NULL);
	i = 0;
	while (++i < cmd->nb_args)
	{
		prv = NULL;
		var = *((*d)->env);
		while (var != NULL)
		{
			key = key_(var->var, d);
			if (key == NULL)
				continue ;
			if (strcmp_(key, cmd->arg[i]) == 0)
			{
				free(var->var);
				if (prv != NULL)
					prv->nxt = var->nxt;
				else if (*((*d)->env) == var)
					*((*d)->env) = var->nxt;
				free(var);
				return (NULL);
			}
			prv = var;
			var = var->nxt;
		}
	}
	return (NULL);
}

