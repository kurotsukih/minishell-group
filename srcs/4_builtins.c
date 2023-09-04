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

int	exec_echo(t_data **d)
{
	int	option_n;
	int	i;
	int	j;
	int	fd;

	option_n = 0;
	j = -1;
	while (++j < (*d)->nb_args)
		if (strcmp((*d)->arg[j], "-n") == 0)
			option_n = 1;
	i = -1;
	while(++i < (*d)->nb_outs)
	{
		fd = (*d)->out[i];
		j = -1;
		while (++j < (*d)->nb_args)
			if (strcmp((*d)->arg[j], "-n") != 0)
			{
				write(fd, (*d)->arg[j], ft_strlen((*d)->arg[j]));
				if (i < (*d)->nb_args - 1)
					write(fd, " ", 1);
			}
		if (option_n == 0)
			write(fd, "\n", 1);
	}
	return (0);
}

int	exec_cd(t_data **d)
{
	char	*dir;
	int		res;

	if ((*d)->nb_args == 1)
	{
		dir = NULL;
		dir = get_value_from_env("HOME", d);
		if (dir == NULL)
			return (printf("cd : variable HOME not found\n"), 0); 	// exic code ?
		res = chdir(dir);
		free(dir);
		if (res == -1)
			return (printf("cd : chdir failure\n"), 0); 	// exic code ?
	}
	else if ((*d)->nb_args == 2)
	{
		if (chdir((*d)->arg[1]) == -1)
			return (printf("cd : chdir failure\n"), 0); 	// exic code ?
	}
	else
		return (printf("cd : too many arguments\n"), 0);
	return (0);
}

int	exec_pwd(t_data **d)
{
	char	*s;
	int		i;

	if ((*d)->nb_args == 1)
	{
		s = getcwd(NULL, 0);
		if (s == NULL)
			return (printf("pwd : getcwd failed\n"), 0); 	// exic code ?
		i = -1;
		while(++i < (*d)->nb_outs)
			write((*d)->out[i], s, ft_strlen(s));
		free(s);
	}
	else
		return (printf("pwd : too many arguments\n"), 0);
	return (0);
}

int	exec_export(t_data **d)
{
	t_env	*new_var;
	int		i;

	if ((*d)->nb_args == 0)
		return (exec_env(d), 0);
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
	return (0);
}

int	exec_unset(t_data **d)
{
	t_env	*var;
	t_env	*prv;
	char	*key;
	int		i;

	if ((*d)->env == NULL)
		return (0);
	if ((*d)->nb_args == 1)
		return (printf("unset : too few arguments\n"), 0);
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
				free(var->var);
				if (prv != NULL)
					prv->nxt = var->nxt;
				else if (*((*d)->env) == var)
					*((*d)->env) = var->nxt;
				free(var);
				return (0);
			}
			prv = var;
			var = var->nxt;
		}
	}
	return (0);
}

