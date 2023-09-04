/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_builtins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:44 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/04 02:13:44 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

int	exec_env(t_data **d)
{
	t_env	*var;
	int		i;

	if ((*d)->env == NULL)
		return (0);
	if ((*d)->nb_args > 1)
		return (printf("env : too many arguments\n"), 0);
	var = *((*d)->env);
	while (var != NULL)
	{
		i = -1;
		while(++i < (*d)->nb_outs)
		{
			write((*d)->out[i], var->var, ft_strlen(var->var));
			write((*d)->out[i], "\n", 1);
		}
		var = var->nxt;
	}
	return (0);
}

int	exec_exit(t_data **d)
{
	if ((*d)->nb_args > 2)
		return (printf("exit : too many arguments\n"), 0);
	if ((*d)->nb_args == 2 && !ft_atoi((*d)->arg[1]))
		return (printf("exit : numeric arg. required\n"), 0);
	if ((*d)->nb_args == 2)
		free_all_and_exit("", ft_atoi((*d)->arg[1]), d);
	else
		free_all_and_exit("", 0, d); // code = 0 ?
	return (0);
}
