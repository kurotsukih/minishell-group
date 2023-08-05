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

void	ft_error(char *str)
{
	if (str == NULL)
		printf("%s\n", strerror(errno)); /// проверить освобождается ли строка
}

void	ft_perror(char *str)
{
	char	*join;

	join = ft_strjoin("bash: ", str);
	perror(join);
	free(join);
}

void	ft_merror(char *str, char *param)
{
	ft_printf2(str, param);
}

void	ft_error_exit(int exit_code)
{
	ft_error(NULL);
	exit(exit_code);
}

