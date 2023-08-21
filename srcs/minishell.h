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

/* extern следующие за ним типы и имена переменных объявляются где-то в другом месте. 
Позволяет компилятору знать о типах ипеременных без действительного создания их. 
Когда два модуля объединяются, все ссылки на внешние переменные пересматриваются.
*/

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

extern int g_signal;

# define INDEF 0
# define PIPE 1
# define OR 2
# define AND 3
# define LEFT_P 4
# define RIGHT_P 5
# define REDIR_IN 6
# define REDIR_OUT 7
# define HEREDOC 8
# define REDIR_OUT2 9
# define FILENAME 10
# define PARAM 11

typedef struct s_cmd
{
	t_list			*params;
	t_list			*redir;
	int				in_fd;
	int				out_fd;
	int				out_pipe_fd;
} t_cmd;

typedef struct s_node
{
	t_list			*elems;
	t_cmd			*cmds;
	int				count_cmd;
	int				type;
	struct s_node	*parent;
	struct s_node	*left;
	struct s_node	*right;
	int				is_micro;
	int				exit_code;
} t_node;

typedef struct s_data
{
	t_list			*env;
	t_node			*n;
	int				exit_code;
} t_data;

char	*add_spaces(char *str);
void	assign_types(t_list *n);
int		check_tokens(t_list *n);
int		open_heredocs(t_list *head, t_list *env);
t_node	*make_tree(t_list *token, t_node *parent);

int		ft_preprocess(t_node *n);
int		ft_exec_command(t_node *n, t_data *d);

int		ft_prepare_pipe(t_node *n, int i_cmd);
int		ft_is_builtin(t_list *token);
int		ft_execute_program(t_cmd *cmd, t_list *env, t_node *n);
int		ft_execute_builtin(t_cmd *cmd, t_data *d, t_node *n);

void	ft_execution(t_data *d);

int		ft_find_path(char *cmd, t_list *env, char **place);
int		ft_open_in(t_list *token, int fd);
int		ft_open_out(t_list *token, int fd);
char	*ft_open_all_files(t_list *token, t_cmd *cmd);
void	ft_execute_pwd(void);
void	ft_execute_unset(t_list **env, t_list *token);
int		ft_execute_exit(t_data *d, t_node *n, t_list *token);
int		ft_execute_export(t_list *params, t_list **env);

char	*strchr_alt(const char *s, int c);
int		size_expanded(char *str, char *value, char *end);
char	*strjoin_big(char *str, char *value, char *end);

char	*expand_string(char *str, t_list *env, t_data *d);

void	ft_preprocess_cmd(t_cmd *cmds, t_list *token);

// parsing
int		parse(char *cmd, t_list *env, t_data *d);
t_list	*expand_token(char *str, t_list *env, t_data *d);

// utils
void	sig_handler_main(int signal);
void	sig_handler_fork(int signal);
void	sig_handler_heredoc(int signal);
void	free_redirections(t_list *head);
void	ft_remove_quotes_string(char *str);
void	ft_remove_quotes_list(t_list *head);
void	exit_(int exit_code, char *msg, char *msg_param, t_list **lst_to_clear, void (*func_to_clear_lst)(void*), char **str_to_sree);
void	ft_clean_fds(t_cmd *cmd);
void	ft_clean_tree(t_node *n);
int		ft_isnum(char *str);
int		ft_abs(int num);
void	ft_bubble_sort_list(t_list *head);
int		ft_strcmp_alt(char *str);
void	*free_charchar(char **s);

#endif