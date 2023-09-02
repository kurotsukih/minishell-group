/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_builtins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:44 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/02 17:45:44 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

void	*exec_env(t_data **d)
{
	t_env	*var;

	if ((*d)->env == NULL)
		return (NULL);
	var = *((*d)->env);
	while (var != NULL)
	{
		printf("%s\n", var->var);
		var = var->nxt;
	}
	return (NULL);
}

void	*exec_exit(t_cmd *cmd, t_data **d)
{
	//(*d)->exit_c = 
	if (cmd->nb_args == 2)
		free_all_and_exit("", ft_atoi(cmd->arg[1]), d);
	else
		free_all_and_exit("", 0, d); // code = 0 ?
	return (NULL);
}
