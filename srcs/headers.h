/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:31 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/12 16:28:57 by akostrik         ###   ########.fr       */
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
# define GET_MOD -2
# define QUOTES0 0
# define QUOTES1 1
# define QUOTES2 2
# define TMP_FILE "tmp"
# define TMP_FILE_H "tmp_h"
# define OK 0
# define FAILURE -1
# define YES 1
# define NO 0
# define STDIN STDIN_FILENO
# define STDOUT STDOUT_FILENO

extern int g_signal;

typedef struct		s_lst
{
	char			*val;
	struct s_lst	*nxt;
}					t_lst;

typedef struct		s_data
{
	t_lst			**env; // EXEC
	int				saved_stdin; // (EXEC but not important)
	int				saved_stdout; // (EXEC but not important)

	int				i;      // cmd_line params

	t_lst			**args;  // cmd params // arg[0] = cmd name  // EXEC
	int				fd_in;   // EXEC
	int				fd_out;  // EXEC
	int				exit_c;  // EXEC // exit code TO DO !!!!!!!!!!!!!!!!!!!!!
	int				there_are_redirs_out;

	char			*token; // token params
	char			*redir;
}					t_data;

// func. free_all_and_exit should have **d for free d in case of an error
// so all the functions have **d

// utils parse (12 functions)
int		all_quotes_are_closed(char *s);
int		skip_spaces(char *s, t_data **d);
void	calc_redir(char *s, t_data **d);
char	*calc_token(char *stop, char *s, t_data **d);
char	*dedollarize_str(char *s, t_data **d);
int		heredoc_to_file(char *delim, t_data **d);
int		init_new_cmd_line(char *s, t_data **d);
int		init_new_cmd(t_data **d);
void	init_new_token(t_data **d);
int		put_fd_if_the_out_is_pipe(char *cmd_line, t_data **d);
int		mod_(char c);
int		char_is_in_str(char c, char *s);

// exec (10)                      min args    max args  accept <in
int		exec_cmd(t_data **d);
int		exec_echo(t_data **d);   // 0           ...       no ?
int		exec_cd(t_data **d);     // 0           1         no ?
int		exec_pwd(t_data **d);    // 0           0         no
int		exec_export(t_data **d); // 0           ...       no ?
int		exec_unset(t_data **d);  // 1           ...       no ?
int		exec_env(t_data **d);    // 0           0         no
int		exec_exit(t_data **d);   // 0           1         no ?
char	*path_(t_data **d);
char	*get_val_from_env(char *key, t_data **d);

// utils lst (7)
void	put_to_lst(char *val, t_lst ***lst, t_data **d);
t_lst	**arr_to_lst(char **arr, t_data **d);
char	**lst_to_arr(t_lst **lst, t_data **d);
int		len_lst(t_lst **lst);
void	del_from_lst(t_lst *to_del, t_lst **lst);
void	del_all_from_lst(t_lst **lst);
void	del_unnecessary_empty_args(t_data **d);
void	free_lst(t_lst ***lst);

// utils (12)
int		init_d(t_data **d, char **env);
void	*malloc_(int size, t_data **d);
void	print_d(char *msg, t_data **d);
int		write_fd(int fd, char *s);
int		write_fd_with_n(int fd, char *s);
void	free_(void *mem);
void	free_2_array(char **arr);
void	free_all_and_exit(char *msg, int exit_c, t_data **d); 
int		err_cmd(char *msg, int exit_c, t_data **d);
void	sig_handler_main(int signal);
void	sig_handler_fork(int signal);
void	sig_handler_heredoc(int signal);

#endif