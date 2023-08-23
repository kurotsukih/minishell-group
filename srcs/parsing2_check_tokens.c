// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   ft_check_tokens.c                                  :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: akalimol <akalimol@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/05/15 14:42:20 by akalimol          #+#    #+#             */
// /*   Updated: 2023/06/01 19:08:22 by akalimol         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// static char	*ft_check_operator(t_lis t *n)
// {
// 	if (n->prev == NULL)
// 		return ((char *)n->content);
// 	if (n->next == NULL)
// 		return ("newline");
// 	if (n->next->type == PIPE || n->next->type == OR || n->next->type == AND)
// 		return ((char *)n->next->content);
// 	return (NULL);
// }

// static char	*ft_check_redirection(t_lis t *n)
// {
// 	if (!n->next)
// 		return ("newline");
// 	if (n->next->type != FILENAME)
// 		return ((char *)n->next->content);
// 	return (NULL);
// }

// int	check_tokens(t_lis t *n)
// {
// 	char	*error;

// 	error = NULL;
// 	while (n)
// 	{
// 		if (n->type == PIPE || n->type == OR || n->type == AND )
// 			error = ft_check_operator(n);
// 		else if (n->type == REDIR_IN || n->type == REDIR_OUT ||  n->type == HEREDOC || n->type == REDIR_OUT2)
// 			error = ft_check_redirection(n);
// 		if (error)
// 			return (exit_(-1, "bash: syntax err unexpected token `%s'\n", error, NULL, NULL, NULL), 0);
// 		n = n->next;
// 	}
// 	return (1);
// }
