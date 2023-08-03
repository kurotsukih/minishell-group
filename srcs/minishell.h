/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akalimol <akalimol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 13:24:48 by akadilkalim       #+#    #+#             */
/*   Updated: 2023/06/01 20:15:13 by akalimol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include "ft_clean.h"
# include "struct_data.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>

# include "ft_clean.h"
# include "ft_error.h"
# include "struct_data.h"
# include "struct_list.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# include "ft_clean.h"
# include "libft.h"
# include "struct_data.h"
# include <stdio.h>

# include "ft_error.h"
# include "struct_data.h"
# include "struct_list.h"
# include <stdio.h>
# include <stdlib.h>

char	*ft_add_spaces(char *str);
t_list	*ft_tokenization(char *str, t_list *env, t_data *data);
void	ft_assign_types(t_list *node);
int		ft_check_tokens(t_list *node);
// int		ft_extend_wildcard(t_list **head);
int		ft_open_heredocs(t_list *head, t_list *env);
t_node	*ft_make_tree(t_list *token, t_node *parent);

int	ft_exec_recursion(t_node *node, t_data *data, t_node *parent);
int	ft_check_operator(int exit_code, char *str);
int	ft_preprocess(t_node *node);
int	ft_exec_command(t_node *node, t_data *data);

int		ft_prepare_pipe(t_node *node, int i_cmd);
int		ft_execute(t_cmd *cmd, t_data *data, t_node *node);
int		ft_is_builtin(t_list *token);
int		ft_execute_program(t_cmd *cmd, t_list *env, t_node *node);
int		ft_execute_builtin(t_cmd *cmd, t_data *data, t_node *node);
void	ft_wait_child_processes(int *is_success, int size, int pid);
int		check(t_cmd *cmd, int count, int result);

void	ft_init_data(int argc, char **argv, char **env, t_data *data);
int		ft_parsing(char *command, t_list *env, t_data *data);
void	ft_execution(t_data *data);

#endif