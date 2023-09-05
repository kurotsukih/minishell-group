/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:31 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/04 02:15:55 by akostrik         ###   ########.fr       */
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
# define TMP_FILE "tmp"
# define OK 0
# define FAILURE -1
# define YES 1
# define NO 0

extern int g_signal;

typedef struct		s_env
{
	char			*var;
	struct s_env	*nxt;
}					t_env;

// *cmd = currectly traited cmd

// структура, связанная с открытым файлом :
// - ссылка на виртуальный Inode файла в памяти ядра
// - флаги доступа к файлу (чтение, запись)
// - текущая позиция чтения/записи 
// - нек доп данные
// Информация о блокировках используется совместно несколькими процессами,
// потому вынесена в виртуальный Inode.

// в момент запуска программы открыты ФД 0, 1 и 2
// Другие ФД так же могут быть открыты, это не регламентируется
// При выделении нового ФД при вызове open, pipe, dup, ... выбирается наименьший свободный ФД

// Процесс может создавать новые ФД, которые будут ссылками на ту же структуру данных в ядре, что и оригинальный ФД и соответственно те же флаги и позицию чтения/записи. Закрытие ФД уменьшает количество ссылок на открытый файл
// Фактическое закрытие файла произойдёт тогда, когда на него не будет ссылаться ни один ФД

// dup возвращает первый свободный номер ФД
// а dup2 указать номер нового ФД
typedef struct		s_data
{
	t_env			**env;
	int				nb_args;
	char			**arg;
	int				nb_ins;
	int				*in;
	int				nb_outs;
	int				*out;
	int				i_args;
	int				i_ins;
	int				i_outs;
	int				saved_stdin;
	int				saved_stdout;
	int				exit_c;
}					t_data;

// utils parse
void	calc_nb_args_ins_outs(char *s, int len, t_data **d);
void	calc_dollar_conversions(char *s, t_data **d);
int		heredoc_to_file(char *delim, t_data **d);

// exec and utils exec                       min args    max   accept <in
void	exec_1_cmd_to_all_outs(t_data **d);
int		exec_echo(t_data **d);   // 0           ...   no ?
int		exec_cd(t_data **d);     // 0           1     no ?
int		exec_pwd(t_data **d);    // 0           0     no
int		exec_export(t_data **d); // 0           ...   no ?
int		exec_unset(t_data **d);  // 1           ...   no ?
int		exec_env(t_data **d);    // 0           0     no
int		exec_exit(t_data **d);   // 0           1     no ?
char	*path_(t_data **d);

// utils env
void	init_env(char **env_array, t_data **d);
char	*key_(char *s, t_data **d);
char 	*val_(char *s, t_data **d);
char	**env_to_array(t_data **d);
int		len_env_(t_data **d);
char	*get_value_from_env(char *key, t_data **d);

// utils str
int		nb_spaces(char *s);
char	*redir_(char *s);
int		len_alphanum(char *s);
char	*alphanum_(char *s, t_data **d);
char	*strdup_(char *s, t_data **d);
char	*strdup_and_erase_redirs(char *s0, t_data **d);
char	*strdup_and_erase_args_except_redirs(char *s0, t_data **d);
char	*strndup_and_trim(char *srs, int len, t_data **d);
int		strcmp_(char *s1, char *s2);
int		mod_(char c);
int		unclosed_quotes(char *s);

// general utils
void	*malloc_(int size, t_data **d);
void	free_(void *mem);
void	free_array(char **arr, int len);
void	free_all_and_exit(char *msg, int exit_c, t_data **d); /// ***d ?
void	print_cmd(char *msg, t_data **d);
void	sig_handler_main(int signal);
void	sig_handler_fork(int signal);
void	sig_handler_heredoc(int signal);

#endif