/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:39 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/05 21:03:11 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

int	exec_pwd(t_data **d)
{
	char	*s;

	if ((*d)->nb_args > 1)
		return (err_cmd("pwd : too many arguments", -1, d));
	s = getcwd(NULL, 0);
	if (s == NULL)
		return (err_cmd("pwd : getcwd failed", -1, d));
	write(1, s, ft_strlen(s));
	write(1, "\n", 1);
	free_(s);
	return (OK);
}

int	exec_export(t_data **d)
{
	t_env	*new_var;
	int		i;

	print_cmd("exec export", d);
	if ((*d)->nb_args == 0)
		return (exec_env(d), OK);
	exec_unset(d);
	i = 0;
	while (++i < (*d)->nb_args)
	{
		new_var = (t_env *)malloc_(sizeof(t_env), d);
		new_var->var = strdup_((*d)->arg[i], d);
		if ((*d)->env != NULL)
			new_var->nxt = *((*d)->env);
		else
			new_var->nxt = NULL;
		*((*d)->env) = new_var;
	}
	return (OK);
}

int	exec_unset(t_data **d)
{
	t_env	*var;
	t_env	*prv;
	char	*key;
	int		i;

	if ((*d)->env == NULL)
		return( OK);
	if ((*d)->nb_args == 1)
		return (err_cmd("unset : too few arguments", -1, d));
	i = 0;
	while (++i < (*d)->nb_args)
	{
		prv = NULL;
		var = *((*d)->env);
		while (var != NULL)
		{
			key = key_(var->var, d);
			if (key == NULL)
				continue ;
			if (strcmp_(key, (*d)->arg[i]) == 0)
			{
				free_(var->var);
				if (prv != NULL)
					prv->nxt = var->nxt;
				else if (*((*d)->env) == var)
					*((*d)->env) = var->nxt;
				free_(var);
				return( OK);
			}
			prv = var;
			var = var->nxt;
		}
	}
	return (OK);
}

int	exec_env(t_data **d)
{
	t_env	*var;

	if ((*d)->env == NULL)
		return( OK);
	if ((*d)->nb_args > 1)
		return (err_cmd("env : too many arguments", -1, d));
	var = *((*d)->env);
	while (var != NULL)
	{
		write(1, var->var, ft_strlen(var->var));
		write(1, "\n", 1);
		var = var->nxt;
	}
	return (OK);
}

int	exec_exit(t_data **d)
{
	if ((*d)->nb_args > 2)
		return (err_cmd("exit : too many arguments", -1, d));
	if ((*d)->nb_args == 2 && !ft_atoi((*d)->arg[1]))
		return (err_cmd("exit : numeric arg. required", -1, d));
	if ((*d)->nb_args == 2)
		free_all_and_exit("", ft_atoi((*d)->arg[1]), d);
	else
		free_all_and_exit("", (*d)->exit_c, d);
	return (OK);
}
