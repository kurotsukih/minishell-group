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

*/

#include "headers.h"

int g_signal = 0;

static int	treat_cmd_line(char *cmd_line, t_env **env)
{
	t_cmds	**cmds;

	cmds = (t_cmds **)malloc(sizeof(t_cmds *));
	if (cmds == NULL)
		return (-1);
	*cmds = NULL;
	if (put_cmd_and_redirect(cmd_line, cmds) == -1)
		return (-1);
	if (put_args(cmds) == -1)
		return (-1);
	if (verify_unclosed_quotes(cmds) == -1)
		return (-1);
	put_doll_conversions(cmds, env);
	print_list(cmds);
	exec_cmds(cmds, env);
	// if (exit_code == 0)
	return (0);
}

int	main(int argc, char **argv, char **env_array)
{
	char	*cmd_line;
	t_env	**env;

	(void)argc;
	(void)argv;
	env = NULL;
	if(env_to_list(env_array, &env) == -1)
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
