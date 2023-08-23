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

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/signal.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <dirent.h>
# include "libft.h"

# define NO_USED 0
# define OUTSIDE_QUOTES 0
# define INSIDE_SIMP_QUOTES 1
# define INSIDE_DOUB_QUOTES 2

extern int g_signal;

// tokens[0] = cmd, tokens[i] = mixed args and options 
typedef struct s_cmd
{
	char 			**tokens;
	int				nb_tokens;
	struct s_cmd	*nxt;
	struct s_cmd	*prv;
	char			*redirect;
	int				is_filename;
	int				in_fd;
	int				out_fd;
	int				exit_code;
} t_cmd;

// parsing
int		parse(char *cmd, char **env);

// utils
char	*alphanum_(char *s);
void	sig_handler_main(int signal);
void	sig_handler_fork(int signal);
void	sig_handler_heredoc(int signal);
void	ft_remove_quotes_string(char *str);
void	ft_remove_quotes_list(t_cmd *head);
void	exit_(int exit_code, char *msg, char *msg_param, t_cmd **lst_to_clear, char **str_to_sree);
int		ft_isnum(char *str);
int		ft_abs(int num);
void	ft_bubble_sort_cmd(t_cmd *head);
int		ft_strcmp_alt(char *str);
void	*free_charchar(char **s);
char	*strdup_(char *s, size_t len);
void	print_cmds(t_cmd **cmd);

// utils list
int			ft_lstsize(t_cmd *lst);
int			ft_lstremove(t_cmd **lst, t_cmd *node);
void		ft_lstadd_front(t_cmd **lst, t_cmd *new_node);
void		ft_lstadd_back(t_cmd **lst, t_cmd *new_node);
void		ft_lstdelone(t_cmd *lst);
void		ft_lstclear(t_cmd **lst);
void		ft_lstiter(t_cmd *lst, void (*f)(void *));
t_cmd		*ft_lstnew();
t_cmd		*ft_lstlast(t_cmd *lst);
t_cmd		*ft_lstmap(t_cmd *lst, void *(*f)(void *), void (*del)(void *));
t_cmd		*ft_lstretrieve(t_cmd **lst, t_cmd *node);
t_cmd		*ft_lstfirst(t_cmd *lst);

#endif