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

# define REINIT_QUOTES -1
# define QUOTES0 0
# define QUOTES1 1
# define QUOTES2 2

extern int g_signal;

// args[0] progname, argd[n] mixed args and options 
typedef struct 		s_cmds
{
	char			*cmd;
	char			**args;
	int				nb_args;
	char			*redirect;
	int				is_filename;
	int				in_fd;
	int				out_fd;
	int				exit_code;
	struct s_cmds	*nxt;
	struct s_cmds	*prv;
}					t_cmds;

typedef struct		s_env
{
	char			*var;
	struct s_env	*nxt;
}					t_env;

// treat_cmd_line
int 	put_cmd_and_redirect(char *cmd_line, t_cmds **l);
void 	put_nb_args(t_cmds **l);
int		put_args(t_cmds **l);
int		verify_unclosed_quotes(t_cmds **l);
int		put_doll_conversions(t_cmds **l, t_env **env);
int		exec_cmds(t_cmds **l, t_env **env);
void	exec_env(t_env **env); 
int		exec_export(t_cmds *cmd, t_env ***env);
void	exec_unset(t_cmds *cmd, t_env ***env);

// utils
char	*alphanum_(char *s);
void	sig_handler_main(int signal);
void	sig_handler_fork(int signal);
void	sig_handler_heredoc(int signal);
int		ft_isnum(char *str);
// int		ft_abs(int num);
// int		ft_strcmp_alt(char *str);
// void	*free_charchar(char **s);
int		strdup_and_trim(char *srs, char **dest, int len);
int		mod_(char c);	
char	*redirect_(char *s);
int nb_args_(char *s, int len);

int	init_new_cmd(t_cmds **new, char *redirect);
void	print_list(t_cmds **cmd);
int		put_cmd_and_redirect_1(t_cmds **l, char *cmd, int len_cmd, char *redirect);

// utils env
char	*val_(char *s);
char	*key_(char *s);
int		env_to_list(char **env_main, t_env ***env_list);
char	**env_to_array(t_env **env_list);
int		len_list(t_env **env);
char	*get_value_from_env(char *key, t_env **env);


//void	exit_(int exit_code, char *msg, char *msg_param, t_cmds **lst_to_clear, char **str_to_sree);
// int		ft_lstsize(t_cmds *lst);
// int		ft_lstremove(t_cmds **lst, t_cmds *node);
// void	ft_lstadd_front(t_cmds **lst, t_cmds *new_node);
// void	ft_lstdelone(t_cmds *lst);
// void	ft_lstclear(t_cmds **lst);
// void	ft_lstiter(t_cmds *lst, void (*f)(void *));
// t_cmds	*ft_lstnew();
// t_cmds	*ft_lstlast(t_cmds *lst);
// t_cmds	*ft_lstmap(t_cmds *lst, void *(*f)(void *), void (*del)(void *));
// t_cmds	*ft_lstretrieve(t_cmds **lst, t_cmds *node);
// t_cmds	*ft_lstfirst(t_cmds *lst);

#endif