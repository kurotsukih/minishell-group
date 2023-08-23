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

SIGIN T = the user types C-c
SIGQUI T = SIGIN T, except that it’s controlled by C-\ + produces a core dump when it terminates the process, 
CTRL-\ causes a program to terminate and dump core

*/

#include "minishell.h"

int g_signal = 0;

int	treat_cmd_line(char *cmd_line, char **env)
{
	t_list	**l;
	t_list	*cmd;

	init_list(&l);
	if (put_cmd_and_redirect_all(cmd_line, l) == -1)
		return (-1);
	calc_nb_args_all(l);
	if (put_args_all(l) == -1)
		return (-1);
	if (verify_unclosed_quotes(l) == -1)
		return (-1);
	print_list(l);

	cmd = *l;
	while(cmd != NULL)
	{
		if (cmd->nb_args > 0)
			treat_dollar_conversions(cmd->args[0], env);
		cmd = cmd->nxt;
	}
	print_list(l);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char	*cmd_line;

	(void)argc;
	(void)argv;
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
		// if (d.exit_code == 0)
		// 	ft_execution(&d);
	}
	return 0; //(exit_code);
}
