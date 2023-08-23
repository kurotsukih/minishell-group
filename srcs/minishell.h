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

# define REINIT_QUOTES_MOD 0
# define OUTSIDE_QUOTES 0
# define INSIDE_SIMP_QUOTES 1
# define INSIDE_DOUB_QUOTES 2

extern int g_signal;

// args = mixed args and options 
typedef struct s_list
{
	char			*cmd;
	char			**args;
	int				nb_args;
	char			*redirect;
	int				is_filename;
	int				in_fd;
	int				out_fd;
	int				exit_code;
	struct s_list	*nxt;
	struct s_list	*prv;
} t_list;

// parsing
int		minishell(char *cmd, char **env);

// utils
char	*alphanum_(char *s);
void	sig_handler_main(int signal);
void	sig_handler_fork(int signal);
void	sig_handler_heredoc(int signal);
void	ft_remove_quotes_string(char *str);
void	ft_remove_quotes_list(t_list *head);
void	exit_(int exit_code, char *msg, char *msg_param, t_list **lst_to_clear, char **str_to_sree);
int		ft_isnum(char *str);
int		ft_abs(int num);
void	ft_bubble_sort_cmd(t_list *head);
int		ft_strcmp_alt(char *str);
void	*free_charchar(char **s);
char	*strdup_(char *s, size_t len);
int	mod_(char c);
char	*redirect_(char *s);

// utils list
int	init_list(t_list ***l);
int		init_new_elt(t_list	**new);
void	print_list(t_list **cmd);
int	put_cmd_and_redirect_to_l(t_list **l, char *cmd, int len_cmd, char *redirect);

int			ft_lstsize(t_list *lst);
int			ft_lstremove(t_list **lst, t_list *node);
void		ft_lstadd_front(t_list **lst, t_list *new_node);
void		ft_lstadd_back(t_list **lst, t_list *new_node);
void		ft_lstdelone(t_list *lst);
void		ft_lstclear(t_list **lst);
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstnew();
t_list		*ft_lstlast(t_list *lst);
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list		*ft_lstretrieve(t_list **lst, t_list *node);
t_list		*ft_lstfirst(t_list *lst);

#endif