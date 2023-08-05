/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akalimol <akalimol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 15:30:15 by akalimol          #+#    #+#             */
/*   Updated: 2023/05/31 19:22:53 by akalimol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error_exit(int exit_code, char *str, char *param)
{
	if (str == NULL)
		printf("%s\n", strerror(errno));
	else if (str != NULL && param == NULL)
		printf("bash: %s\n", str);
	else if (str != NULL && param != NULL)
		printf(str, param);
	if (exit_code != -1)
		exit(exit_code);
	(void)param;
}

