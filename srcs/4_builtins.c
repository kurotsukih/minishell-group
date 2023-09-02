/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_builtins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:39 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/02 15:22:40 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

void	exec_pwd(t_data **d)
{
	char	*s;

	s = getcwd(NULL, 0);
	if (s == NULL)
	{
		(*d)->curr_cmd->err = "getcwd failed"; // exit code ?
		return ;
	}
	printf("%s\n", s);
	free(s);
}

void	exec_cd(t_cmd *cmd, t_data **d)
{
	char	*dir;

	if (cmd->nb_args == 0)
		return;
	if (cmd->nb_args == 1)
	{
		dir = get_value_from_env("HOME", d);
		if (dir == NULL)
		{
			(*d)->curr_cmd->err = "env variable HOME not found"; // exit code ?
			return ;
		}
		if (chdir(dir) == -1)
			(*d)->curr_cmd->err = "chdir failure "; // exit code ? 
		return ;
	}
	if (chdir(cmd->arg[1]) == -1)
		(*d)->curr_cmd->err = "chdir failure "; // exit code ?
}

void	exec_env(t_data **d)
{
	t_env	*var;

	var = *((*d)->env);
	while (var != NULL)
	{
		printf("%s\n", var->var);
		var = var->nxt;
	}
}

void	exec_export(t_cmd *cmd, t_data **d)
{
	t_env	*new_var;
	int		i;

	if (cmd->nb_args == 0)
		exec_env(d);
	exec_unset(cmd, d);
	i = 0;
	while (++i < cmd->nb_args)
	{
		new_var = (t_env *)malloc_(sizeof(t_env), d);
		new_var->var = strdup_(cmd->arg[i], d);
		new_var->nxt = *((*d)->env);
		*((*d)->env) = new_var;
	}
}

void	exec_unset(t_cmd *cmd, t_data **d)
{
	t_env	*var;
	t_env	*prv;
	char	*key;
	int		i;

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
				return ;
			}
			prv = var;
			var = var->nxt;
		}
	}
}

