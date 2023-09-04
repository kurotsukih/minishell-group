/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:16 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/04 02:16:09 by akostrik         ###   ########.fr       */
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
getcwd chdir stat lstat fstat unlink execv e du p du p2 pip e
ope ndir readdir closedir
strerro r perro r
isatty ttyname ttyslot ioctl
geten v 
tcsetattr tcgetattr tgetent tgetflag tgetnum tgetstr tgoto tputs

wait3 wait4 : resource usage information about the ft_wait_child_processes
stat lstat fstat : file or file system status, information about a file
unlink : call the unlink function to remove the specified file
pip e : creates a pip e, a unidirectional data channel that can be used for interprocess communication.
isatty : 1 if fd is an ope n file descriptor referring to a terminal; otherwise 0
ttyname : pathname of the terminal device that is ope n on the file descriptor fd

SIGIN T = the user types C-c
SIGQUI T = SIGIN T, except that it’s controlled by C-\ + produces a core dump when it terminates the process, 
CTRL-\ causes a program to terminate and dump core

cd : $PWD change

si on met pas les wai tpid juste apres l'execution de la commande et 
qu'on les met a la fin dans une boucle il y a beaucoup de commandes 
avec des pipes qui ne fonctionnent plus correctement, par exemple ls|wc fait une boucle infini

double quotes insdide simple ones ?

" $ " is ok ?

a="s -la"   > l$a -> ls -la
$a=" "      > ls$a-la$a"Makefile" -> ls -la Makefile 

wc -l < infile
wc -l < infile > outfile

1	  les erreurs générales, comme une division par zéro
2	  mauvaise utilisation de commandes intégrées, d'après la documentation de Bash
126	la commande appelée ne peut s'exécuter, problème de droits ou commande non exécutable
127	commande introuvable, problème possible avec $PATH ou erreur de frappe
128	argument de commande invalide
128+n	128 + le numéro du signal
130	terminé avec Ctrl-C (130 = 128 + 2)
255	code de sortie en dehors de la limite par ex exit -1

le fils s'est terminé normalement, exit(3), _exit(2), retour de main() :
WIFEXITED(status)   = true
WEXITSTATUS(status) = le code de sortie du fils

le fils s'est terminé à cause d'un signal:
WIFSIGNALED(status) = vrai 
WTERMSIG(status) = le numéro du signal

pour les process zombie j'ai utilisé la macro sigaction avec SIGCHLD et SA_NOCLDWAIT
*/

// >out1 >out1+ >> out2 < in >> out2+ < in+ <in++
// extern cmd change the env ? getenv() setenv() putenv() unsetenv()
// if env variable has many values (like PATH)
// à la premiere erreur (le droit decriture pour les redir out, etc) ca fait tout fail

#include "headers.h"

int g_signal = 0;

int parse_1_token(char *s, int *i, t_data **d)
{
	char	*redir;
	char	*alphanum;

	(*i) += nb_spaces(&s[*i]);
	redir = redir_(&s[*i]);
	(*i) += ft_strlen(redir);
	alphanum = alphanum_(&s[*i], d);
	(*i) += ft_strlen(alphanum);
	if (ft_strcmp(redir, "<") == 0)
		(*d)->in[++((*d)->i_ins)] = open(alphanum, O_RDONLY);
		//if (!(*d)->in[i_ins]) return (FAILURE)
	else if (ft_strcmp(redir, "<<") == 0)
	{
		heredoc_to_file(alphanum, d);
		(*d)->in[++((*d)->i_ins)] = open(TMP_FILE, O_RDONLY);
		//if (!(*d)->in[i_ins]) return (FAILURE)
	}
	else if (ft_strcmp(redir, ">") == 0)
		(*d)->out[++((*d)->i_outs)] = open(alphanum, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		//if (!(*d)->out[i_outs]) return (FAILURE)
	else if (ft_strcmp(redir, ">>") == 0)
		(*d)->out[++((*d)->i_outs)] = open(alphanum, O_WRONLY | O_CREAT | O_APPEND, 0666);
		//if (!(*d)->out[i_outs]) return (FAILURE)	
	else
		(*d)->arg[++((*d)->i_args)] = alphanum;
	// !remove_quotes(d) || // only for bultins? нужно? попробовать без
	return (OK);
}

static int	parse_1_cmd(char *s, int len, t_data **d)
{
	int		i;
	int		mod;

	calc_nb_args_ins_outs(s, len, d);
	mod_(REINIT_QUOTES);
	i = -1;
	while (++i < len)
	{
		mod = mod_(s[i]);
		if (mod == QUOTES0)
			if (parse_1_token(s, &i, d) == FAILURE)
				return (FAILURE);
	}
	if(mod != QUOTES0)
		return (printf("%s : unclosed quotes\n", s), FAILURE); // free d
	if ((*d)->nb_args == -1)
		return (printf("empty command\n"), FAILURE); // exit_code = 255
	if ((*d)->i_ins == -1)
		((*d)->in)[0] = dup(STDIN_FILENO); // prv pipe if ! dup  return return (FAILURE) ?
	if ((*d)->i_outs == -1)
		((*d)->out)[0] = dup(STDOUT_FILENO); // nxt pipe
	return (OK);
}

// arg[0] = prog name
static int	parse_and_exec_cmd_line(char *s, t_data **d)
{
	int	i_beg;
	int	i;
	int	len;

	// mod_(REINIT_QUOTES);
	i = 0;
	(*d)->i_args = -1;
	(*d)->i_ins = -1;
	(*d)->i_outs = -1;
	while (1)
	{
		i_beg = i;
		while (s[i] != '\0')
		{
			if (mod_(s[i]) == QUOTES0 && s[i] == '|')
				break;
			i++;
		}
		len = i - i_beg;
		if (parse_1_cmd(&s[i_beg], len, d) == OK)// ||
		// dollar converstions in ins? in heredo c? remove_quotes ?
			exec_1_cmd(d);
		if (s[i] == '\0')
			break;
		i++;
	}
	return (OK);
}

int	main(int argc, char **argv, char **env_array)
{
	char	*cmd_line;
	t_data	**d;

	(void)argc;
	(void)argv;
	d = (t_data **)malloc_(sizeof(t_data *), NULL); // func init_d
	*d = (t_data *)malloc_(sizeof(t_data), d);
	(*d)->saved_stdin = dup(STDIN_FILENO);
	(*d)->saved_stdout = dup(STDOUT_FILENO);
	init_env(env_array, d);
	// signal(SIGQUIT, SIG_IGN);
	// signal(SIGINT, &sig_handler_main);
	while (1)
	{
		cmd_line = NULL;
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
		parse_and_exec_cmd_line(cmd_line, d);
		free_(cmd_line);
	}
	return (/* free_all_and_exit("", d->exit_code, d)*/ OK);
}
