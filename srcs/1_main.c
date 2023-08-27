/*
после "cat файл", "cat файл | cat -e" : free(): double free detected in tcache 2

readline rl_clear_history, rl_on_new_line rl_replace_line rl_redisplay, add_history,
malloc free
printf write access open read close
fork wait waitpid wait3 wait4 
signa l sigaction sigemptyset sigaddset kill
exit
getcwd chdir stat lstat fstat unlink execv e dup dup2 pipe
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

double quotes insdide simple ones ?

*/

#include "headers.h"

int g_signal = 0;

static void	put_env(char **env_array, t_data **d)
{
	int		i;
	t_env	*new;

	(*d)->env = NULL;
	(*d)->env = (t_env **)malloc(sizeof(t_env *));
	if ((*d)->env == NULL)
		exit_(d);
	*((*d)->env) = NULL;
	i = -1;
	while (1)
	{
		new = NULL;
		new = (t_env *)malloc(sizeof(t_env));
		if (new == NULL)
			exit_(d);
		new->var = env_array[i];
		if (new->var == NULL)
			break ; 
		new->nxt = *((*d)->env);
		*((*d)->env) = new;
	}
}

static void	init(t_data ***d, char **env_array)
{
	*d = (t_data **)malloc(sizeof(t_data *));
	if (*d == NULL)
		exit_(*d);
	**d = (t_data *)malloc(sizeof(t_data));
	if (**d == NULL)
		exit_(*d);
	(**d)->cmds = (t_cmds **)malloc(sizeof(t_cmds *));
	if ((**d)->cmds == NULL)
		exit_(*d);
	*((**d)->cmds) = NULL;
	put_env(env_array, *d);
	// signal(SIGQUIT, SIG_IGN);
	// signal(SIGINT, &sig_handler_main);
}

int	main(int argc, char **argv, char **env_array)
{
	char	*cmd_line;
	t_data	**d;

	(void)argc;
	(void)argv;
	init(&d, env_array);
	cmd_line = NULL;
	while (1)
	{
		cmd_line = readline("$");
		if (cmd_line == NULL) // EOF
			break ;
		// if (g_signal == 1)
		// {
		// 	g_signal = 0;
		// 	d->exit_code = 130;
		// 	continue;
		// }
		add_history(cmd_line);
		put_cmd_line_and_redirects(cmd_line, d);
		calc_args(d);
		if (there_are_unclosed_quotes(d) == 1)
			continue;
		calc_doll_conversions(d);
		print_cmds(d);
		exec_cmds(d);
		// free_cmd_line
	}
	return 0; //(exit_code);
}
