/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akalimol <akalimol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 04:12:20 by akalimol          #+#    #+#             */
/*   Updated: 2023/06/01 17:22:18 by akalimol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_remove_quotes(t_list *head)
{
	t_list	*token;
	char	*str;

	token = head;
	while (token)
	{
		str = (char *)token->content;
		ft_remove_quotes_string(str);
		token = token->next;
	}
}
