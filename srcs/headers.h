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
// # define SUCCESS 0
// # define FAILURE 1

extern int g_signal;

// arg[0] = prog name
typedef struct 		s_cmd
{
	char			**arg;
	int				nb_args;
	int				fd_in;
	int				fd_out;
	char			*path;
	struct s_cmd	*nxt;
	struct s_cmd	*prv; // not used ?
	char			*err;
}					t_cmd;

typedef struct		s_env
{
	char			*var;
	struct s_env	*nxt;
}					t_env;

// *cmd = currectly traited cmd
typedef struct		s_data
{
	t_cmd			**cmds;
	t_cmd			*curr_cmd;
	t_env			**env;
	char			**paths;
	int				nb_paths;
	int				exit_c;
	int				saved_stdout;
}					t_data;

void	put_full_cmd_to_arg0(char *s, t_data **d);
void	put_redirs_and_args(t_data **d);
void	put_paths_to_cmds(t_data **d);
void	del_cmd_from_lst(t_cmd *cmd, t_data **d);
void	del_cmds(t_data **d);

// utils cmd
void	init_cmd(t_cmd **new, t_data **d);
int		nb_args_(char *s, int len, t_data **d);
char	*redir_(char *s);
int		mod_(char c);
void	verif_args(t_data **d);
int		there_are_unclosed_quotes(t_cmd *cmd);
void	calc_dollar_convers(t_cmd *cmd, t_data **d);
int		is_builtin(t_cmd *cmd);
char	*path_(char *s1, char *s2, t_data **d);
void	open_file(char *redir, char *redir_file, t_data **d);
void	start_redirs(t_cmd *cmd);
void 	stop_redirs(t_cmd *cmd, t_data **d);
void	print_cmds(char *msg, t_data **d);

// utils env
char	*key_(char *s, t_data **d);
char 	*val_(char *s, t_data **d);
char	**env_to_array(t_data **d);
int		len_env_(t_data **d);
char	*get_value_from_env(char *key, t_data **d);
void	put_paths_to_d(t_data **d);
void	free_env_array(char **env, int len);

// utils exec
void	exec_pwd(t_data **d);
void	exec_echo(t_cmd *cmd);
void	exec_env(t_data **d); 
void	exec_export(t_cmd *cmd, t_data **d);
void	exec_unset(t_cmd *cmd, t_data **d);
void	exec_cmds(t_data **d);
void	exec_exit(t_data **d);
void	exec_cd(t_cmd *cmd, t_data **d);

// utils str
char	*alphanum_(char *s, t_data **d);
char	*strdup_(char *s, t_data **d);
char	*strdup_and_erase_redirs(char *s0, t_data **d);
char	*strdup_and_erase_args_except_redirs(char *s0, t_data **d);
char	*strndup_and_trim(char *srs, int len, t_data **d);

// utils
void	*malloc_(size_t size, t_data **d);
void	free_all_and_exit(char *msg, int exit_c, t_data **d); /// ***d ?
void	sig_handler_main(int signal);
void	sig_handler_fork(int signal);

#endif