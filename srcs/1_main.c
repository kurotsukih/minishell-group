/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:16 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/02 18:09:55 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
после "cat файл", "cat файл | cat -e" : double fre e detected in tcache 2

readline rl_clear_history, rl_on_new_line rl_replace_line rl_redisplay, add_history,
mallo c fre e
print f writ e access ope n read close
fork wai t  wai tpid  wai t3  wai t4 
signa l sigaction sigemptyset sigaddset kill
exit
getcwd chdir stat lstat fstat unlink execv e dup dup2 pipe
ope ndir readdir closedir
strerro r perro r
isatty ttyname ttyslot ioctl
geten v 
tcsetattr tcgetattr tgetent tgetflag tgetnum tgetstr tgoto tputs

not authorised:  putenv(3), setenv(3), unsetenv(3), environ(5)

SIGIN T = the user types C-c
SIGQUI T = SIGIN T, except that it’s controlled by C-\ + produces a core dump when it terminates the process, 
CTRL-\ causes a program to terminate and dump core

cd : Quand on change de répertoire, le bash voit sa variable $PWD changer automatiquement

si on met pas les wai tpid juste apres l'execution de la commande et qu'on les met a la fin dans une boucle il y a beaucoup de commandes avec des pipes qui ne fonctionnent plus correctement, par exemple ls|wc fait une boucle infini 

double quotes insdide simple ones ?

" $ " is ok ?

should we treat this : (?)
a="s -la"   > l$a -> ls -la
$a=" "      > ls$a-la$a"Makefile" -> ls -la Makefile 

wc -l < infile
tu dupliques l'entrée standard dans l'infile puis tu lances la cmd avec execve 

wc -l < infile > outfile
*/

#include "headers.h"

int g_signal = 0;

static void	put_env(char **env_array, t_data **d)
{
	int		i;
	t_env	*new;

	(*d)->env = (t_env **)malloc_(sizeof(t_env *), d);
	*((*d)->env) = NULL;
	i = -1;
	while (1)
	{
		new = (t_env *)malloc_(sizeof(t_env), d);
		new->var = env_array[++i];
		if (new->var == NULL)
			break ; 
		new->nxt = *((*d)->env);
		*((*d)->env) = new;
	}
}

static void	init_d(t_data ***d, char **env_array) // **d ?
{
	*d = (t_data **)malloc_(sizeof(t_data *), *d);
	**d = (t_data *)malloc_(sizeof(t_data), *d);
	(**d)->cmds = (t_cmd **)malloc_(sizeof(t_cmd *), *d);
	*((**d)->cmds) = NULL;
	(**d)->curr_cmd = NULL;
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
	init_d(&d, env_array);
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
		parse(cmd_line, d);
		verif_args(d);
		// print_cmds("", d);
		exec_cmds(d);
		del_cmds(d);
		free(cmd_line);
	}
	close((*d)->saved_stdout);
	return 0; //(d->exit_code);
}
