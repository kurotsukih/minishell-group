/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:31 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/05 21:43:23 by akostrik         ###   ########.fr       */
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
# define OK 0
# define FAILURE -1
# define YES 1
# define NO 0

extern int g_signal;

// *cmd = currectly traited cmd

// структура, связанная с открытым файлом :
// - ссылка на виртуальный Inode файла в памяти ядра
// - флаги доступа к файлу (чтение, запись)
// - текущая позиция чтения/записи 
// - нек доп данные
// Информация о блокировках используется совместно несколькими процессами,
// потому вынесена в виртуальный Inode.

// в момент запуска программы открыты ФД 0, 1 и 2
// Другие ФД могут быть открыты, это не регламентируется
// При выделении нового ФД при вызове open, pipe, du p, ... выбирается наименьший свободный ФД

// Процесс может создавать новые ФД, которые будут ссылками на ту же структуру данных в ядре, что и оригинальный ФД, те же флаги, позицию чтения/записи. Закрытие ФД уменьшает количество ссылок на открытый файл
// Фактическое закрытие файла произойдёт тогда, когда на него не будет ссылаться ни один ФД

// du p возвращает первый свободный номер ФД
// а dup2 указать номер нового ФД

typedef struct		s_lst
{
	void			*val;
	struct s_lst	*nxt;
}					t_lst;

typedef struct		s_data
{
	// minishell params
	t_lst			**env;
	int				saved_stdin;
	int				saved_stdout;

	// cmd_line params
	int				i;

	// cmd params
	t_lst			**args;
	t_lst			**outs;
	int				in;
	int				exit_c;

	// token params
	char			*token;
	char			*redir;
}					t_data;

// utils parse
int		all_quotes_are_closed(char *s);
void	skip_spaces(char *s, t_data **d);
void	calc_redir(char *s, t_data **d);
char	*alphanum_(char *s, t_data **d);
char	*calc_token_str(char *stop, char *s, t_data **d);
int		heredoc_to_file(char *delim, t_data **d);
char	*dedollarized_(char *s, t_data **d);
int		mod_(char c);
char	*strdup_(char *s, t_data **d);
char	*strndup_and_trim(char *srs, int len, t_data **d);
int		strcmp_(char *s1, char *s2);
int		is_in(char c, char *s);

// exec                             min args    max   accept <in
int		exec_cmd(t_data **d);
int		exec_echo(t_data **d);   // 0           ...   no ?
int		exec_cd(t_data **d);     // 0           1     no ?
int		exec_pwd(t_data **d);    // 0           0     no
int		exec_export(t_data **d); // 0           ...   no ?
int		exec_unset(t_data **d);  // 1           ...   no ?
int		exec_env(t_data **d);    // 0           0     no
int		exec_exit(t_data **d);   // 0           1     no ?
char	*path_(t_data **d);

// utils env
char	*key_(char *s, t_data **d);
char 	*val_(char *s, t_data **d);
char	*get_val_from_env(char *key, t_data **d);

// utils lst
void	put_to_lst(void *val, t_lst ***lst, t_data **d);
t_lst	**arr_to_lst(char **arr, t_data **d);
char	**lst_to_arr(t_lst **lst, t_data **d);
int		len_lst(t_lst **lst);
void	del_from_lst(t_lst *to_del, t_lst **lst);
void	del_all_from_lst(t_lst **lst);
void	free_lst(t_lst ***lst);

// utils
void	init_d(t_data ***d, char **env);
void	reinit_cmd(t_data **d);
void	*malloc_(int size, t_data **d);
void	print_d(char *msg, t_data **d);
int		write_fd(int fd, char *s);
int		write_fd_with_n(int fd, char *s);
void	free_(void *mem);
void	free_2_array(char **arr);
void	free_all_and_exit(char *msg, int exit_c, t_data **d); /// ***d ?
int		err_cmd(char *msg, int exit_c, t_data **d);
void	sig_handler_main(int signal);
void	sig_handler_fork(int signal);
void	sig_handler_heredoc(int signal);

#endif