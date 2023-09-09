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

	printf("exec pwd\n");
	print_d("!", d);
	printf("len list = %d\n", len_lst((*d)->args));
	printf("exec pwd\n");
	if (len_lst((*d)->args) > 2) //null
		return (err_cmd("pwd : too many arguments", -1, d));
	printf("exec pwd\n");
	s = getcwd(NULL, 0);
	printf("exec pwd\n");
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
		put_to_lst(cur->val, &((*d)->env), d); /// ft_strdup ?
		cur = cur->nxt;
	}
	return (OK);
}

int	exec_unset(t_data **d)
{
	t_lst	*cur;
	t_lst	*env;

	if (len_lst((*d)->args) == 1)
		return (err_cmd("unset : too few arguments", -1, d));
	cur = (*((*d)->args))->nxt;
	while (cur != NULL)
	{
		env = *((*d)->env);
		while (env != NULL)
		{
			if (ft_strncmp(cur->val, env->val, ft_strlen(cur->val)) == 0)
			{
				del_from_lst(env, (*d)->env);
				break ;
			}
			env = env->nxt;
		}
		cur = cur->nxt;
	}
	return (OK);
}

int	exec_env(t_data **d)
{
	t_lst	*env;

	if ((*d)->env == NULL)
		return( OK);
	if (len_lst((*d)->args) > 1)
		return (err_cmd("env : too many arguments", -1, d));
	env = *((*d)->env);
	while (env != NULL)
	{
		write_fd_with_n(1, env->val);
		env = env->nxt;
	}
	return (OK);
}

int	exec_exit(t_data **d)
{
	if (len_lst((*d)->args) > 2)
		return (err_cmd("exit : too many arguments", -1, d));
	if (len_lst((*d)->args) == 2 && !ft_atoi((*((*d)->args))->nxt->val))
		return (err_cmd("exit : numeric arg. required", -1, d));
	if (len_lst((*d)->args) == 1)
		free_all_and_exit("", (*d)->exit_c, d);
	else if (len_lst((*d)->args) == 2)
		free_all_and_exit("", ft_atoi((*((*d)->args))->nxt->val), d);
	return (OK);
}
