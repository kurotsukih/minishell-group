/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_builtins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:39 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/15 12:32:21 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

int	exec_echo(t_data **d)
{
	t_lst	*cur;
	int		option_n;

	option_n = NO;
	cur = (*((*d)->args))->nxt;
	while (cur != NULL)
	{
		if (ft_strcmp(cur->val, "-n") == 0)
			option_n = YES;
		else
			write_fd(1, cur->val);
		cur = cur->nxt;
	}
	if (option_n == NO)
		write_fd(1, "\n");
	return (OK);
}

int	exec_cd(t_data **d)
{
	char	*dir;
	int		res;

	if (len_lst((*d)->args) > 2)
		return (err_cmd("cd : too many arguments", 1, d)); // 1 +
	if (len_lst((*d)->args) == 1)
	{
		dir = get_val_from_env("HOME", d);
		if (dir == NULL)
			return (err_cmd("cd : HOME not defined", 1, d)); // 1 +
	}
	else if (len_lst((*d)->args) == 2)
		dir = (*((*d)->args))->nxt->val;
	res = chdir(dir);
	// if (len_lst((*d)->args) == 1)
	// 	free_(dir);
	if (res == -1)
		return (err_cmd("cd : chdir failure", 1, d)); // code 1 = general errors
	return (OK);
}

int	exec_pwd(t_data **d)
{
	char	*s;

	s = getcwd(NULL, 0);
	if (s == NULL)
		return (err_cmd("pwd : getcwd failed", -1, d));
	write_fd_with_n(1, s);
	free_(s);
	return (OK);
}

int	exec_export(t_data **d)
{
	t_lst	*cur;

	if (len_lst((*d)->args) == 1)
		return (exec_env(d), OK);
	exec_unset(d);
	cur = (*((*d)->args))->nxt;
	while (cur != NULL)
	{
		put_to_lst(cur->val, &((*d)->env), d);
		cur = cur->nxt;
	}
	return (OK);
}

int	exec_unset(t_data **d)
{
	t_lst	*arg;
	t_lst	*env;
	char	*key_arg;
	char	*key_env;

	if (len_lst((*d)->args) == 1)
		return (err_cmd("unset : too few arguments", 0, d)); // code 0 ok
	arg = (*((*d)->args))->nxt;
	while (arg != NULL)
	{
		key_arg = calc_token("=\0", arg->val, d);
		env = *((*d)->env);
		while (env != NULL)
		{
			key_env = calc_token("=\0", env->val, d);
			if (ft_strcmp(key_arg, key_env) == 0)
			{
				del_from_lst(env, (*d)->env);
				break ;
			}
			env = env->nxt;
		}
		arg = arg->nxt;
	}
	return (OK);
}

int	exec_env(t_data **d)
{
	t_lst	*env;

	if ((*d)->env == NULL)
		return( OK);
	if (len_lst((*d)->args) > 1)
		return (err_cmd("env : too many arguments", 127, d)); // code 127 ok
	env = *((*d)->env);
	while (env != NULL && env->val != NULL)
	{
		write_fd_with_n(1, env->val);
		env = env->nxt;
	}
	return (OK);
}

int	exec_exit(t_data **d)
{
	if (len_lst((*d)->args) > 2)
		return (err_cmd("exit : too many arguments", 1, d)); // code 1 ok
	if (len_lst((*d)->args) == 2 && !ft_atoi((*((*d)->args))->nxt->val))
		return (err_cmd("exit : numeric arg. required", 2, d)); // code 2 ok
	if (len_lst((*d)->args) == 2)
		free_all_and_exit("", ft_atoi((*((*d)->args))->nxt->val), d);
	else if (len_lst((*d)->args) == 1)
		free_all_and_exit("", (*d)->exit_c, d);
	return (OK);
}
