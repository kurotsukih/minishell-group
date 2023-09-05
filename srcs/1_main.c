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
wait3 wait4 : resource usage information about the f t_wait_child_processes
stat lstat fstat : file or file system status, information about a file
unlink : call the unlink function to remove the specified file
pip e : creates a pip e, a unidirectional data channel that can be used for interprocess communication.
isatty : 1 if fd is an ope n file descriptor referring to a terminal; otherwise 0
ttyname : pathname of the terminal device that is ope n on the file descriptor fd

SIGIN T = the user types C-c
SIGQUI T = SIGIN T, except that it’s controlled by C-\ + produces a core dump when it terminates the process, 
CTRL-\ causes a program to terminate and dump core

si on met pas les wai tpid juste apres l'execution de la commande et 
qu'on les met a la fin dans une boucle il y a beaucoup de commandes 
avec des pipes qui ne fonctionnent plus correctement, par exemple ls|wc fait une boucle infini

pour les process zombie j'ai utilisé la macro sigaction avec SIGCHLD et SA_NOCLDWAIT

à la premiere erreur (le droit decriture pour les redir out, etc) ca fait tout fail

execve creates a new process with the same ope n file descriptors as the parent

//// REDIRS
les redir se font ds lordre
les redir peuvent etre située n'importe ou par rapport a la commande et ses eventuels arguments
pour ce qui est des redir out ca cree un fichier

>out1 >out2 > out3 < in >> out2 < in+ <in++
out1 et 2 sont vide vu qu'ils ont été créés puis finalement la ligne de cmd indique un autre fichier d'output (???)

mm logique pr les redirs in, à la premiere erreur (le droit decriture pour les redir out, etc) ca fait tout fail

heredo c = redir in, mais au lieu de rediriger le contenu d'un fichier ds le stdin ca redirige un input 
le fichier qui répresent le heredo c peut etre implementé en tant que fichier temp, qui se delete a la fin de la cmd

<<lol bash
echo ahah
exit 12
> lol
je demande a lancer bash en redirigeant un heredo c dans le stdin
le fichier qu'il represente contiendra tout ce que je rentre en input jusqu'a une ligne = "lol"
la cmd lance bash, puis dans ce bash execute echo ahah puis exit 12,
on reviens au shell d'origine
echo $? affiche 12

//// CODES
1	  les erreurs générales, comme une division par zéro
2	  mauvaise utilisation de commandes intégrées, d'après la documentation de Bash
126	la commande appelée ne peut s'exécuter, problème de droits ou commande non exécutable
127	commande introuvable, problème possible avec $PATH ou erreur de frappe
128	argument de commande invalide
128+n	128 + le numéro du signal
130	terminé avec Ctrl-C (130 = 128 + 2)
255	code de sortie en dehors de la limite par ex exit -1
126 команда найдена, но не может быть выполнена
127 команда не найдена, дочерний процесс, созданный для ее выполнения, возвращает 127

le fils s'est terminé normalement, exit(3), _exit(2), retour de main() :
WIFEXITED(status)   = true
WEXITSTATUS(status) = le code de sortie du fils

le fils s'est terminé à cause d'un signal:
WIFSIGNALED(status) = vrai 
WTERMSIG(status) = le numéro du signal

//// TESTS
double quotes insdide simple ones 
" $ "
export a="s -la" | l$a
export $a=" " |ls$a-la$a"Makefile"
wc -l < infile
wc -l < infile > outfile
extern cmd change the env ?
2 heredocs

// int	exec_command(t_node *n)
// {
// 	while (cmd != NULL)
// 	{
// 		pid = fork();
// 		exit_c = 0;
// 		if (pid == 0)
// 		{
//			exit_c = exec_program(cmd, d->env, n);
//			exit_c = f t_find_path(cmd->args, env, &path);
//			execve(...);
//			pid = (exit(exit_c), 0);
//		}
// 		cmd = cmd -> nxt;
// 	}
// 	f t_wait_child_processes(num, pid);
// }
*/

#include "headers.h"

int g_signal = 0;

static void	init(t_data ***d, char **env)
{
	*d = (t_data **)malloc_(sizeof(t_data *), NULL);
	**d = (t_data *)malloc_(sizeof(t_data), *d);
	(**d)->saved_stdin = dup(STDIN_FILENO);
	(**d)->saved_stdout = dup(STDOUT_FILENO);
	init_env(env, *d);
	// signal(SIGQUIT, SIG_IGN);
	// signal(SIGINT, &sig_handler_main);
}

int	save_alphanum_and_open_file(char *redir, char *alphanum, t_data **d)
{
	if (strcmp_(redir, "<") == 0)
		(*d)->in = open(alphanum, O_RDONLY);
		//if (!(*d)->in) return (FAILURE)
	else if (strcmp_(redir, "<<") == 0)
	{
		heredoc_to_file(alphanum, d);
		(*d)->in = open(TMP_FILE, O_RDONLY);
		//if (!(*d)->in) return (FAILURE)
	}
	else if (strcmp_(redir, ">") == 0)
	{
		(*d)->out[++((*d)->i_outs)] = open(alphanum, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		//if (!(*d)->out[i_outs]) return (FAILURE)
	}
	else if (strcmp_(redir, ">>") == 0)
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
	char	*redir;
	char	*alphanum;

	(*d)->i_args = -1;
	(*d)->i_outs = -1;
	(*d)->in = -1;
	calc_nb_args_and_outs(s, len, d);
	mod_(REINIT_QUOTES);
	i = 0;
	while (i < len)
	{
		mod = mod_(s[i]);
		if (mod == QUOTES0)
		{
			i += nb_spaces(&s[i]);
			redir = redir_(&s[i]);
			i += ft_strlen(redir);
			i += nb_spaces(&s[i]);
			alphanum = alphanum_(&s[i], d); // quotes ?
			i += ft_strlen(alphanum) - 1;
			if (save_alphanum_and_open_file(redir, alphanum, d) == FAILURE)
				return (FAILURE);
		}
		i++;
	}
	if(mod != QUOTES0)
		return (err_cmd("unclosed quotes", -1, d));
			if ((*d)->nb_args == -1)
		return (err_cmd("empty command", -1, d));
	if ((*d)->i_outs == -1)
		((*d)->out)[0] = dup(STDOUT_FILENO); // nxt pipe
	if ((*d)->in == -1)
		(*d)->in = dup(STDIN_FILENO); // prv pipe if ! dup  return return (FAILURE) ?
	return (OK);
}

// arg[0] = prog name
static int	parse_and_exec_cmd_line(char *s, t_data **d)
{
	int	i_beg;
	int	i;
	int	len;

	i = 0;
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
		// dollar converstions in in? in heredoc? remove_quotes ?
		{
			print_cmd("", d);
			exec_1_cmd_to_all_outs(d);
		}
		if (s[i] == '\0')
			break;
		i++;
	}
	return (OK);
}

int	main(int argc, char **argv, char **env)
{
	char	*cmd_line;
	t_data	**d;

	(void)argc;
	(void)argv;
	init(&d, env);
	while (1)
	{
		cmd_line = NULL;
		cmd_line = readline("$");
		if (cmd_line == NULL) // EOF
			break ;
		// if (g_signal == 1)
		// {
		// 	g_signal = 0;
		// 	(d*)->exit_code = 130;
		// 	continue;
		// }
		add_history(cmd_line);
		parse_and_exec_cmd_line(cmd_line, d);
		free_(cmd_line);
	}
	free_all_and_exit("", 0, d);
	return (OK); // ?
}
