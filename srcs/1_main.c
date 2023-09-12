/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:16 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/12 13:43:27 by akostrik         ###   ########.fr       */
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
double quote s insdide simple ones 
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

// printf("%d ", *((int *)(cur->val)));

// Par ailleurs, tu fais tes tests dans zsh pas bash

#include "headers.h"

int g_signal = 0;

int	put_token_to_d(t_data **d)
{

	if (ft_strcmp((*d)->redir, "<<") == 0)
	{
		heredoc_to_file((*d)->token, d);
		(*d)->fd_in = open(TMP_FILE_H, O_RDONLY);
	}
	else if (ft_strcmp((*d)->redir, "<") == 0)
		(*d)->fd_in = open((*d)->token, O_RDONLY);
	else if (ft_strcmp((*d)->redir, ">>") == 0)
		(*d)->fd_out = open((*d)->token, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else if (ft_strcmp((*d)->redir, ">") == 0)
		(*d)->fd_out = open((*d)->token, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else
		put_to_lst((*d)->token, &((*d)->args), d);
	// if (((*d)->redir)[0] == '<' && (*d)->in == -1)
	//	return (NULL);
	// if (((*d)->redir)[0] == '>' && out == -1)
	// 	return (NULL);
	return (OK);
}

// no matter what this func returns
static int	put_nxt_token_to_d(char *s, t_data **d)
{
	skip_spaces(s, d);
	(*d)->redir = "";
	(*d)->token = ""; // free((*d)->token); ?
	if (s[(*d)->i] == '\'')
	{
		(*d)->token = calc_token("\'\0", &s[(*d)->i + 1], d);
		(*d)->i += ft_strlen((*d)->token) + 2;
	}
	else if (s[(*d)->i] == '\"')
	{
		(*d)->token = calc_token("\"\0", &s[(*d)->i + 1], d);
		(*d)->i += ft_strlen((*d)->token) + 2;
		(*d)->token = dedollarized_((*d)->token, d);
	}
	else
	{
		calc_redir(s, d);
		skip_spaces(s, d);
		(*d)->token = calc_token(" \"\'<>|", &s[(*d)->i], d);
		((*d)->i) += ft_strlen((*d)->token);
		(*d)->token = dedollarized_((*d)->token, d);
	}
	if (ft_strlen((*d)->token) > 0 && put_token_to_d(d) == FAILURE)
		return (err_cmd("get token pb", -1, d));
	if (skip_spaces(s, d) == YES)
		{} // to add a space to the token (for echo) // add a space in any case ?
	return (OK);
}

// arg[0] = prog name
// no matter what this func returns
static int	exec_cmd_line(char *s, t_data **d)
{
	int	fd_tmp;

	if (all_quotes_are_closed(s) != OK)
		return (err_cmd("uncloses quotes", -1, d));
	(*d)->i = 0;
	while (1) // cmds
	{
		del_all_from_lst((*d)->args);
		(*d)->fd_in = -1;
		fd_tmp = open(TMP_FILE, O_RDONLY);
		if (fd_tmp == -1)
			(*d)->fd_in = dup(STDIN); // close ?
		else
			(*d)->fd_in = fd_tmp;
		(*d)->fd_out = dup(STDOUT);
		if ((*d)->fd_in == -1 || (*d)->fd_out == -1)
			return (err_cmd("dup pb", -1, d));
		fd_tmp = (*d)->fd_out;
		while (1) // tokens, token = arg or fd, while token != NULL // do while ?
		{
			put_nxt_token_to_d(s, d);
			if (ft_strlen((*d)->token) == 0)
				break ;
		}
		if ((*d)->fd_out == fd_tmp && s[(*d)->i] == '|')
			(*d)->fd_out = open(TMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if ((*d)->fd_out == -1)
			return (err_cmd("dup pb", -1, d));
		print_d("parsed", d);
		exec_cmd(d);
		if (s[(*d)->i] != '|') // == \0 ?
		{
			// cat (the last out)
			break ;
		}
		// unlink(TMP_FILE);
		// unlink(TMP_FILE_H);
		((*d)->i)++;
	}
	return (OK);
}

int	main(int argc, char **argv, char **env)
{
	char	*cmd_line;
	t_data	*d;

	((void)argc, (void)argv);
	init_d(&d, env);
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
		// 	(d*)->exit_code = 130;
		// 	continue;
		// }
		add_history(cmd_line);
		exec_cmd_line(cmd_line, &d);
		free_(cmd_line);
	}
	free_all_and_exit("", 0, &d); // never executed ?
	return (0);
}
