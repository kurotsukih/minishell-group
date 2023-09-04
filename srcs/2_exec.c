/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:29 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/04 02:12:45 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

// execve creates a new process with the same ope n file descriptors as the parent

// >out1 < src1 cat >>out2 >out3 <src2 <<eof -e >out3 < src3

// les redir se font ds lordre, 
// donc pour ce qui est des redir out ca cree un fichier out1 car la premiere redir out c'est > out1
// puis finalement ca créer out2 car c'est la 2eme redir out parsée dans la cmd puis out3
// out1 et 2 sont vide vu qu'ils ont été créés puis finalement la ligne de cmd indique un autre fichier d'output
// mm logique pr les redirs in, 
// à la premiere erreur (le droit decriture pour les redir out, etc) ca fait tout fail

// tu peux voir le parsing d'une commande comme un tri entre ce qui es redir in, redir out
// et le reste c'est la commande en elle meme, 
// dans cet exemple une fois trié toute les redir out (les > et ce qui les suit genre > out1)
// et tte les redir in, il te reste cat -e
// les redir peuvent etre située n'importe ou par rapport a la commande et ses eventuels arguments

// heredo c = juste une redir in, 
// mais au lieu de rediriger le contenu d'un fichier ds le stdin ca redirige un input 
// (jusqua atteindre le delimiteur)

// <<lol bash
// echo ahah
// exit 12
// > lol

// ahah
// echo $?
// 12
// je demande a lancer bash en redirigeant un heredo c dans le stdin,
// le delimiteur est lol
// le fichier qu'il represente contiendra tout ce que je rentre en input jusqu'a une ligne = "lol"
// echo ahah suivit de exit 12 donc la cmd lance bash,
// puis dans ce bash execute echo ahah puis exit 12,
// on reviens au shell d'origine et si on echo le retour de la derniere commande (ici bash) c'est bien 12

// le fichier qui répresent le heredo c peut etre implementé en tant que fichier 
// temp, qui se delete a la fin de la cmd

// 127 команда не найдена, дочерний процесс, созданный для ее выполнения, возвращает 127
// 126 команда найдена, но не может быть выполнена

// int	exec_command(t_node *n)
// {
// 	num = 0;
// 	while (cmd != NULL)
// 	{
// 		pid = fork();
// 		exit_c = 0;
// 		if (pid == 0)
// 		{
//			exit_c = exec_program(cmd, d->env, n);
//			exit_c = ft_find_path(cmd->args, env, &path);
//			execve(...);
//			pid = (exit(exit_c), 0);
//		}
// 		cmd = cmd -> nxt;
// 	}
// 	ft_wait_child_processes(num, pid);
// }

int	exec_echo(t_data **d)
{
	int	option_n;
	int	i;
	int	j;
	int	fd;

	option_n = 0;
	j = -1;
	while (++j < (*d)->nb_args)
		if (strcmp((*d)->arg[j], "-n") == 0)
			option_n = 1;
	i = -1;
	while(++i < (*d)->nb_outs)
	{
		fd = (*d)->out[i];
		j = -1;
		while (++j < (*d)->nb_args)
			if (strcmp((*d)->arg[j], "-n") != 0)
			{
				write(fd, (*d)->arg[j], ft_strlen((*d)->arg[j]));
				if (i < (*d)->nb_args - 1)
					write(fd, " ", 1);
			}
		if (option_n == 0)
			write(fd, "\n", 1);
	}
	return (0);
}

int	exec_cd(t_data **d)
{
	char	*dir;
	int		res;

	if ((*d)->nb_args == 1)
	{
		dir = NULL;
		dir = get_value_from_env("HOME", d);
		if (dir == NULL)
			return (printf("cd : variable HOME not found\n"), 0); 	// exic code ?
		res = chdir(dir);
		free_(dir);
		if (res == -1)
			return (printf("cd : chdir failure\n"), 0); 	// exic code ?
	}
	else if ((*d)->nb_args == 2)
	{
		if (chdir((*d)->arg[1]) == -1)
			return (printf("cd : chdir failure\n"), 0); 	// exic code ?
	}
	else
		return (printf("cd : too many arguments\n"), 0);
	return (0);
}

int	exec_pwd(t_data **d)
{
	char	*s;
	int		i;

	if ((*d)->nb_args == 1)
	{
		s = getcwd(NULL, 0);
		printf("s = %s\n", s);
		if (s == NULL)
			return (printf("pwd : getcwd failed\n"), OK); 	// exic code ?
		i = -1;
		while(++i < (*d)->nb_outs)
			write((*d)->out[i], s, ft_strlen(s));
		free_(s);
	}
	else
		return (printf("pwd : too many arguments\n"), OK);
	return (OK);
}

static int	exec_extern_cmd(t_data **d)
{
	int		pid;
	char	*path;
	char	**env_array;
	// int		len_env;
	int		status;
	int		j;

	path = path_(d); // un chemin relatif ou absolu ?
	if (path == NULL)
		path = ".";

	j = -1;
	while (++j < (*d)->nb_outs)
	{
		if (dup2((*d)->out[j], STDOUT_FILENO) == -1)
			return (printf("%s : dup2 pb\n", (*d)->arg[0]), 0); // exit_code = 127, if (errno != 2) exit_c = 126;
		close((*d)->out[j]);
		pid = fork();
		if (pid < -1)
			return (printf("%s : fork pb\n", (*d)->arg[0]), -1); // fre e all and exit ?
		if (pid == 0)
		{
			env_array = env_to_array(d);
			// len_env = len_env_(d);
			execve(path, (*d)->arg, env_array); //if env_array == NULL ? // every execve substitue le processus ???!!!
			// free_env_array(env_array, len_env); no executed ?
		}
		else
			wait(&status);
		if (dup2((*d)->saved_stdout, STDOUT_FILENO) == -1)
			return (printf("%s : dup2 pb\n", (*d)->arg[0]), 0);
	}
	return (0);
}

void	exec(t_data **d)
{
	if (strcmp_((*d)->arg[0], "echo") == 0)
		exec_echo(d);
	else if (strcmp_((*d)->arg[0], "cd") == 0)
		exec_cd(d);
	else if (strcmp_((*d)->arg[0], "pwd") == 0)
		exec_pwd(d);
	else if (strcmp_((*d)->arg[0], "export") == 0)
		exec_export(d);
	else if (strcmp_((*d)->arg[0], "unset") == 0)
		exec_unset(d);
	else if (strcmp_((*d)->arg[0], "env") == 0)
		exec_env(d);
	else if (strcmp_((*d)->arg[0], "exit") == 0)
		exec_exit(d);
	else
		exec_extern_cmd(d);
	unlink(TMP_FILE);
}
