/*
после "cat файл", "cat файл | cat -e" : free(): double free detected in tcache 2

readline rl_clear_history, rl_on_new_line rl_replace_line rl_redisplay, add_history,
malloc free
printf write access open read close
fork wait waitpid wait3 wait4 
signa l sigaction sigemptyset sigaddset kill
exit
getcwd chdir stat lstat fstat unlink execve dup dup2 pipe
opendir readdir closedir
strerror perror
isatty ttyname ttyslot ioctl
geten v 
tcsetattr tcgetattr tgetent tgetflag tgetnum tgetstr tgoto tputs

not authorised:  putenv(3), setenv(3), unsetenv(3), environ(5)

SIGIN T = the user types C-c
SIGQUI T = SIGIN T, except that it’s controlled by C-\ + produces a core dump when it terminates the process, 
CTRL-\ causes a program to terminate and dump core

cd : Quand on change de répertoire, le bash voit sa variable $PWD changer automatiquement

si on met pas les waitpid juste apres l'execution de la commande et qu'on les met a la fin dans une boucle il y a beaucoup de commandes avec des pipes qui ne fonctionnent plus correctement, par exemple ls|wc fait une boucle infini 

ft_execution
ft_exec_recursion
ft_exec_command
ft_execute
ft_execute_builtin ft_execute_program
*/

#include "headers.h"

int g_signal = 0;

static int init_env(char **env_main, t_env ***env_list)
{
	int		i;
	t_env	*new;

	*env_list = NULL;
	*env_list = (t_env **)malloc(sizeof(t_env *)); ///
	if (*env_list == NULL)
		return (-1);
	**env_list = NULL;
	i = -1;
	while (env_main[++i])
	{
		new = NULL;
		new = (t_env *)malloc(sizeof(t_env));
		if (new == NULL)
			return (-1);
		new->val = part_after_sign_equal(env_main[i]);
		if (new->val == NULL)
			return (-1);
		new->key = part_before_sign_equal(env_main[i]);
		if (new->key == NULL)
			return (-1);
		new->nxt = **env_list;
		**env_list = new;
	}
	return (0);
};

static int	treat_cmd_line(char *cmd_line, t_env **env)
{
	t_list	**l;

	l = (t_list **)malloc(sizeof(t_list *));
	if (l == NULL)
		return (-1);
	*l = NULL;
	if (put_cmd_and_redirect(cmd_line, l) == -1)
		return (-1);
	put_nb_args(l);
	if (put_args(l) == -1)
		return (-1);
	if (verify_unclosed_quotes(l) == -1)
		return (-1);
	put_doll_conversions(l, env);
	print_list(l);
	exec_cmds(l, env);
	// if (d.exit_code == 0)
	// 	ft_execution(&d);
	return (0);
}

int	main(int argc, char **argv, char **env_main)
{
	char	*cmd_line;
	t_env	**env;

	(void)argc;
	(void)argv;
	env = NULL;
	if(init_env(env_main, &env) == -1)
		return (-1);
	// signal(SIGQUIT, SIG_IGN);
	// signal(SIGINT, &sig_handler_main);
	cmd_line = NULL;
	while (1)
	{
		cmd_line = readline("$");
		if (cmd_line == NULL) // EOF
			break ;
		// if (g_signal == 1)
		// {
		// 	g_signal = 0;
		// 	d.exit_code = 130;
		// 	continue;
		// }
		add_history(cmd_line);
		treat_cmd_line(cmd_line, env);
	}
	return 0; //(exit_code);
}
