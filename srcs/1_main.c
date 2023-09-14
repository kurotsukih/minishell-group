/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:16 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/14 15:46:38 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
are there processus zombie ?


***** REDIRECTIONS (< << > >>)
- les redirs se font ds lordre
- peuvent etre située n'importe ou par rapport a la commande et ses arguments
- pour ce qui est des redir out ca cree un fichier
- à la premiere erreur (le droit decriture pour les redir out, etc) ca fait tout fail (!)
- mm logique pr les redirs in
- heredoc = redir in, mais au lieu de rediriger le contenu d'un fichier ds le stdin ca redirige un input 
le fichier qui répresent le heredo c peut etre implementé en tant que fichier temp, qui se delete a la fin de la cmd


**** EXIT CODES - TO DO
1	  les erreurs générales (une division par zéro, ...)
2	  mauvaise utilisation de commandes intégrées, d'après la documentation de Bash
126	  la commande ne peut s'exécuter (problème de droits, commande non exécutable)
127	  commande introuvable, дочерний возвращает 127 (problème $PATH, erreur de frappe)
128	  argument de commande invalide
128+n 128 + le numéro du sig
130	  terminé avec Ctrl-C (13 0 = 128 + 2)
255	  code de sortie en dehors de la limite (ex exit -1)


**** SIGNA LS
ctrl + \ = SIGQUI T = do nothing
ctrl + C = SIGIN T  = stop cmd, show new line
ctrl + D = SIGIN T  = quit shell
Ctrl + D does not send a sig, it sends EOF to the terminal

***** TESTS - TO DO !!!!!!!!!!!!!!!!!!!!!!
to test in bash (not zsh !) (to change with the command ">bash")   !!!!!!!!!!!!!!!!!!!!!!

double quote s insdide simple ones 
" $ "
export a="s -la" | l$a
export $a=" " |ls$a-la$a"Makefile"
wc -l < infile
wc -l < infile > outfile
extern cmd change the env ?
2 heredo cs

>out1 >out2 > out3 < in >> out2 < in+ <in++
out1 et 2 sont vide vu qu'ils ont été créés puis finalement la ligne de cmd indique un autre fichier d'output (???)

si on met pas les wai tpid juste apres l'execution de la commande et 
qu'on les met a la fin dans une boucle il y a beaucoup de commandes 
avec des pipes qui ne fonctionnent plus correctement, par exemple ls|wc fait une boucle infini

<<lol bash
echo ahah
exit 12
> lol
je demande a lancer bash en redirigeant un heredo c dans le stdin
le fichier qu'il represente contiendra tout ce que je rentre en input jusqu'a une ligne = "lol"
la cmd lance bash, puis dans ce bash execute echo ahah puis exit 12,
on reviens au shell d'origine
echo $? affiche 12

||

*/

#include "headers.h"

int g_signal = 0;

static int	init_minishell(int argc, char **argv, char **env, t_data **d) // **d ?
{
	(void)argc;
	(void)argv;
	*d = (t_data *)malloc_(sizeof(t_data), d);
	(*d)->env = arr_to_lst(env, d);
	(*d)->redir = ""; // no need ?
	(*d)->token = "";
	(*d)->saved_stdin = dup(STDIN_FILENO); // if fail s ?
	(*d)->saved_stdout = dup(STDOUT_FILENO);
	(*d)->exit_c = 0;
	if (signal(SIGINT, &sig_handler) == SIG_ERR) 
		free_all_and_exit("Could not set signal handler", -1, d); // is it necessary ?
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR) 
		free_all_and_exit("Could not set signal handler", -1, d);
	return (OK);
}

// no matter what this func returns
static int	parse_and_exec_cmd_line(char *cmd_line, t_data **d)
{
	if (init_new_line(cmd_line, d) == FAILURE)
		return (FAILURE);
	while (1) // loop on commands, command = what is between | and |
	{
		if (init_cmd(d) == FAILURE)
			return (FAILURE);
		while (1) // loop on tokens, token = cmd name OR un arg of the cmd
		{
			init_token(d);
			put_nxt_token_to_d(cmd_line, d);
			if (ft_strlen((*d)->token) == 0)
				break ;
		}
		put_tmpfile_as_fd_out_if_pipe(cmd_line, d);
		exec_cmd(d);
		if (cmd_line[(*d)->i] != '|')
			break ;
		((*d)->i)++;
		unlink(TMP_FILE_HEREDOC);
	}
	return (OK);
}

int	main(int argc, char **argv, char **env)
{
	char	*cmd_line;
	t_data	*d;

	init_minishell(argc, argv, env, &d);
	while (1) // loop on command lines
	{
		cmd_line = NULL;
		cmd_line = readline("$");
		if (cmd_line == NULL) // EOF = ctrl + D
			break ;
		if (g_signal == 1)
		{
			g_signal = 0;
			d->exit_c = 130;
			continue;
		}
		add_history(cmd_line);
		parse_and_exec_cmd_line(cmd_line, &d);
		free_(cmd_line);
		unlink(TMP_FILE_OUT); // deletes a name from the filesystem
	}
	free_all_and_exit("", 0, &d); // executed only if ctrl + D ?
	return (0);
}
