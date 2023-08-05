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

# define PIPE 1
# define OR 2
# define AND 3
# define LEFT_P 4
# define RIGHT_P 5
# define REDIRECT_IN 6
# define REDIRECT_OUT 7
# define HEREDOC 8
# define REDIRECT_OUT2 9
# define FILENAME 10
# define PARAMETER 11

// typedef struct s_list //// it is in libft
// {
// 	void			*content;
// 	int				type;
// 	struct s_list	*next;
// 	struct s_list	*prev;
// } t_list;

typedef struct s_cmd
{
	t_list			*params;
	t_list			*redir;
	int				in_fd;
	int				out_fd;
	int				out_pipe_fd;
}					t_cmd;

typedef struct s_node
{
	t_list			*elems;
	t_cmd			*cmds;
	int				count_cmd;
	int				type;
	int				exit_code;
	int				level;
	struct s_node	*parent;
	struct s_node	*left;
	struct s_node	*right;
	int				is_micro;

}					t_node;

typedef struct s_data
{
	t_list			*env;
	t_node			*node;
	int				exit_code;
}					t_data;

char	*ft_add_spaces(char *str);
t_list	*ft_tokenization(char *str, t_list *env, t_data *data);
void	ft_assign_types(t_list *node);
int		ft_check_tokens(t_list *node);
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

void	ft_execution(t_data *data);

void	free_(t_list *env);
int		ft_find_path(char *cmd, t_list *env, char **place);
char	**ft_construct_command(t_list *params);
int		ft_open_in(t_list *token, int fd);
int		ft_open_out(t_list *token, int fd);
char	*ft_open_all_files(t_list *token, t_cmd *cmd);
void	ft_execute_pwd(void);
void	ft_execute_unset(t_list **env, t_list *token);
int		ft_execute_exit(t_data *data, t_node *node, t_list *token);
void	ft_execute_env(t_list *env);
void	ft_execute_echo(t_list *token);
int		ft_execute_export(t_list *params, t_list **env);
int		ft_execute_cd(t_list *params, t_list *env);

char	*ft_strchr_alt(const char *s, int c);
int		ft_size_expanded(char *str, char *value, char *end);
char	*ft_strjoin_big(char *str, char *value, char *end);
int		ft_find_key(char *str);
char	*ft_find_value(char *key, int i_pos, t_list *env, t_data *data);

char	*ft_expand_string(char *str, t_list *env, t_data *data);
t_list	*ft_free_expand_token(char **words, t_list **head);
char	**ft_split_alt(char const *s, char c);

t_list	*ft_add_token(char *str, int i_beg, int i_end, t_data *data);
int		ft_is_token(char c, int checker);
t_list	*ft_token_merror(char *str, t_list *token);
t_list	*ft_token_error(t_list *token);
void	ft_remove_quotes(t_list *head);

void	ft_clean_fds(t_cmd *cmd);
void	ft_clean_tree(t_node *node);
void	ft_clean_cmds(t_cmd *cmds_p, int size);
void	ft_clean_tokens(t_list **token);
void	ft_clean_darray(char **trash);

void	ft_error(void);
void	ft_perror(char *str);
void	ft_merror(char *str, char *param);
void	ft_error_clean_exit(t_node *node, t_list *env, int exit_code);
void	ft_perror_clean_exit(t_data *my_data, char *str);
void	ft_merror_clean_exit(t_data *my_data, char *str, char *param);
void	ft_clean_exit(t_data *my_data);
void	ft_error_exit(int exit_code);
void	ft_perror_exit(char *str, int exit_code);
void	ft_merror_exit(char *str, char *param, int exit_code);

t_list	*ft_preprocess_parameter(t_list *token, t_cmd *cmd, int i_cmd);
t_list	*ft_preprocess_redirection(t_list *token, t_cmd *cmd, int i_cmd);

int		ft_preprocess_node(t_node *node);
int		ft_count_cmds(t_list *token);
void	ft_init_cmds(t_cmd *cmds, int num_cmds);
void	ft_preprocess_cmd(t_cmd *cmds, t_list *token);

//// utils
void	sig_handler_main(int signal);
void	sig_handler_fork(int signal);
void	sig_handler_heredoc(int signal);

#endif