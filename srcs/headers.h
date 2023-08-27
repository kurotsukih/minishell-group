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

typedef struct 		s_cmds
{
	char			**args; // args[0] = prog name, argd[n] mixed args and options 
	int				nb_args;
	int				nb_args_max;
	char			*redirect; // ">>" ">" "<" "<<" "|"
	int				is_filename;
	int				fd_in;
	int				fd_out;
	struct s_cmds	*nxt;
}					t_cmds;

typedef struct		s_env
{
	char			*var;
	struct s_env	*nxt;
}					t_env;

typedef struct 		s_data
{
	t_cmds			**cmds;
	t_env			**env;
	int				exit_c;
	char			*err;
	char			*to_free;
}					t_data;

void	put_cmd_line_and_redirects(char *cmd_line, t_data **d);
void	calc_args(t_data **d);
int		args_are_correct(t_cmds *cmd, t_data **d);
int		there_are_unclosed_quotes(t_cmds *cmd);
void	calc_doll_conversions(t_data **d);
void	exec_env(t_data **d); 
void	exec_export(t_cmds *cmd, t_data **d);
void	exec_unset(t_cmds *cmd, t_data **d);
void	exec_extern_cmd(t_cmds *cmd, t_data **d);
int		exec_cmds(t_data **d);

// utils
void	sig_handler_main(int signal);
void	sig_handler_fork(int signal);
void	*malloc_(size_t size, t_data **d);
void	exit_(t_data **d); /// ***d

void	init_cmd(t_cmds **new, char *redirect, t_data **d);
char	*redirect_(char *s);
int		mod_(char c);	
int		nb_args_(char *s, int len);
void	print_cmds(t_data **d);

char	*val_(char *s);
char	*key_(char *s, t_data **d);
char	**env_to_array(t_data **d);
int		len_env(t_data **d);
char	*get_value_from_env(char *key, t_data **d);

char	*alphanum_(char *s, t_data **d);
void	strdup_and_trim(char *srs, char **dest, int len, t_data **d);


#endif