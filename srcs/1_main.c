/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:16 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/12 16:28:40 by akostrik         ###   ########.fr       */
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
to test in bash (not zsh !) (chqnge with the command ">bash")   !!!!!!!!!!!!!!!!!!!!!!

double quote s insdide simple ones 
" $ "
export a="s -la" | l$a
export $a=" " |ls$a-la$a"Makefile"
wc -l < infile
wc -l < infile > outfile
extern cmd change the env ?
2 heredocs

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

*/

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
	// if (((*d)->redir)[0] == '<' && (*d)->in == -1) || ((*d)->redir)[0] == '>' && out == -1)
	// 	return (FAILURE);
	if (((*d)->redir)[0] == '>')
		(*d)->there_are_redirs_out = YES;
	return (OK);
}

// parsing next token
// token = name of the cmd  OR  un arg of the cmd  OR  un redir (<, <<, > or >>)
// no matter what this func returns
static int	parse_nxt_token(char *cmd_line, t_data **d)
{
	skip_spaces(cmd_line, d);
	if (cmd_line[(*d)->i] == '\'') // to verify !!!
	{
		(*d)->token = calc_token("\'\0", &cmd_line[(*d)->i + 1], d);
		(*d)->i += ft_strlen((*d)->token) + 2;
	}
	else if (cmd_line[(*d)->i] == '\"') // to verify !!!
	{
		(*d)->token = calc_token("\"\0", &cmd_line[(*d)->i + 1], d);
		(*d)->i += ft_strlen((*d)->token) + 2;
		(*d)->token = dedollarize_str((*d)->token, d);
	}
	else
	{
		calc_redir(cmd_line, d);
		skip_spaces(cmd_line, d);
		(*d)->token = calc_token(" \"\'<>|", &cmd_line[(*d)->i], d);
		((*d)->i) += ft_strlen((*d)->token);
		(*d)->token = dedollarize_str((*d)->token, d);
	}
	if (ft_strlen((*d)->token) > 0 && put_token_to_d(d) == FAILURE)
		return (err_cmd("get token pb", 1, d)); // 1 ?
	if (skip_spaces(cmd_line, d) == YES)
		{
			(*d)->token=" "; // can we free it in the end ?
			if (put_token_to_d(d) == FAILURE) // (only for spaces in echo outpub)
				return (err_cmd("get token pb", 1, d)); // 1 ?
		} 
	return (OK);
}

// arg[0] = cmd name
// arg[1], arg[2], ... = arguments
// no matter what this func returns

// unlink() deletes a name from the filesystem.
// If that name was the last link to a file and no processes have the file open, 
// the file is deleted
static int	parse_and_exec_cmd_line(char *cmd_line, t_data **d)
{
	if (init_new_line(cmd_line, d) == FAILURE)
		return (FAILURE);
	while (1)
	{
		if (init_cmd(d) == FAILURE)
			return (FAILURE);
		while (1) // tokens, token = arg or fd
		{
			init_token(d);
			parse_nxt_token(cmd_line, d);
			if (ft_strlen((*d)->token) == 0)
				break ;
		}
		put_fd_if_the_out_is_pipe(cmd_line, d);
		del_unnecessary_empty_args(d);
		exec_cmd(d);
		if (cmd_line[(*d)->i] != '|')
			break ;
		((*d)->i)++;
	}
	return (OK);
}

int	main(int argc, char **argv, char **env)
{
	char	*cmd_line;
	t_data	*d;

	init_minishell(argc, argv, env, &d);
	while (1)
	{
		cmd_line = NULL;
		cmd_line = readline("$");
		if (cmd_line == NULL) // EOF = ctrl + D ?
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
		unlink(TMP_FILE);
		unlink(TMP_FILE_H);
	}
	free_all_and_exit("", 0, &d); // only if ctrl + D ?
	return (0);
}
