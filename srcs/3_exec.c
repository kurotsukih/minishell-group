/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_builtins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:39 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/04 02:13:30 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

int	exec_pwd(t_data **d)
{
	char	*s;

	if ((*d)->nb_args == 1)
	{
		s = getcwd(NULL, 0);
		if (s == NULL)
			return (printf("pwd : getcwd failed\n"), OK); 	// exic code ?
		printf("%s\n", s);
		free_(s);
	}
	else
		return (printf("pwd : too many arguments\n"), OK);
	return (OK);
}

int	exec_export(t_data **d)
{
	t_env	*new_var;
	int		i;

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
		return (printf("unset : too few arguments\n"), OK);
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
		return (printf("env : too many arguments\n"), OK);
	var = *((*d)->env);
	while (var != NULL)
	{
		printf("%s\n", var->var);
		var = var->nxt;
	}
	return (OK);
}

int	exec_exit(t_data **d)
{
	if ((*d)->nb_args > 2)
		return (printf("exit : too many arguments\n"), OK);
	if ((*d)->nb_args == 2 && !ft_atoi((*d)->arg[1]))
		return (printf("exit : numeric arg. required\n"), OK);
	if ((*d)->nb_args == 2)
		free_all_and_exit("", ft_atoi((*d)->arg[1]), d);
	else
		free_all_and_exit("", 0, d); // code = 0 ?
	return (OK);
}
