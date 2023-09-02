/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:29 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/02 22:08:13 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

// char *args[] = { "/bin/sh", "-c", "echo hello! > /path/to/file", NULL };
// execv("/bin/sh", args);

// execve creates a new process with the same ope n file descriptors as the parent
// (there are exceptions and naunces which can be read from the execve man page))

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

// heredoc = juste une redir in, 
// mais au lieu de rediriger le contenu d'un fichier ds le stdin ca redirige un input 
// (jusqua atteindre le delimiteur que tu defini dans ton heredoc)

// exemple d'un heredoc 
/*
<<lol bash
echo ahah
exit 12
> lol
*/
// ahah
// echo $?
// 12
// je demande a lancer bash en redirigeant un heredoc dans le stdin,
// le delimiteur est lol
// le fichier qu'il represente contiendra tout ce que je rentre en input jusqu'a une ligne = "lol"
// echo ahah suivit de exit 12 donc la cmd lance bash,
// puis dans ce bash execute echo ahah puis exit 12,
// on reviens au shell d'origine et si on echo le retour de la derniere commande (ici bash) c'est bien 12

// le fichier qui répresent le heredoc peut etre implementé en tant que fichier 
// temp, se delete a la fin de la cmd

// 127 команда не найдена, дочерний процесс, созданный для ее выполнения, возвращает 127
// 126 команда найдена, но не может быть выполнена

// static void	pipe_(int i_cmd, t_data **d)
// {
// 	int		fd[2];

// 	if (i_cmd != 0 && cmds[i_cmd - 1].out_fd != 1)
// 		close(n->cmds[i_cmd - 1].out_fd);
// 	if (i_cmd < n->count_cmd - 1)
// 	{
// 		if (pipe(fd) == -1)
// 			return (exit_(-1), 1);
// 		n->cmds[i_cmd].out_fd = fd[1];
// 		n->cmds[i_cmd].out_pipe_fd = fd[0];
// 		n->cmds[i_cmd + 1].in_fd = fd[0];
// 	}
// 	if (ft_open_all_files(n->cmds[i_cmd].redir, n->cmds + i_cmd))
// 	{
// 		exit_();
// 		if (n->cmds[i_cmd].in_fd != -1 && n->cmds[i_cmd].in_fd != 0)
// 			close(n->cmds[i_cmd].in_fd);
// 		if (n->cmds[i_cmd].out_fd != -1 && n->cmds[i_cmd].out_fd != 1)
// 			close(n->cmds[i_cmd].out_fd);
// 		return (1);
// 	}
// 	return (0);
// }

// int	exec_command(t_node *n)
// {
// 	int	num;
// 	int	pid;
// 	int	result;
// 	t_list cmd;
// 	char	*path;
// 	int	exit_c;
// 	char	**args;

// 	num = 0;
// 	result = -1;
// 	pid = 0;
// 	cmd = *l;
// 	while (cmd != NULL)
// 	{
// 		result = ft_prepare_pipe(n, cmd);
// 		if ((check(cmds), n->count_cmd, result) && num++ >= 0)
// 		{
// 			pid = fork();
// 			exit_c = 0;
// 			if (pid == 0)
// 			{
// 				signal(SIGINT, &sig_handler_fork);
// 				signal(SIGQUIT, &sig_handler_fork);
// 				if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
// 					exit_();
// 				if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
// 					exit_();
// 				ft_clean_fds(cmd);
// 				if (cmd->params && ft_is_builtin(cmd->params) == 1)
// 					exit_c = exec_builtin(cmd, d, n);
// 				else if (cmd->params)
// 				{
// 					exit_c = exec_program(cmd, d->env, n);
// 					if (!strcmp(cmd->ars, "."))
// 						return (exit_(-1, "bash: .: filename arg required\n"), -1);
// 					exit_c = ft_find_path(cmd->args, env, &path);
// 					args = ft_construct_command(cmd->args);
// 					execve(path, args, ft_construct_command(env));
// 					path = cmd->args;
// 					if (ft_strchr(path, '/'))
// 						exit_(-1, "bash: %s: no such file or directory\n");
// 					else
// 						exit_(-1, "bash: %s: command not found\n");
// 				}
// 				pid = (exit(exit_c), 0);
// 			}
// 			else if (cmd->in_fd != 0)
// 				close(cmd->in_fd);
// 			pid = signal(SIGINT, SIG_IGN), pid;
// 		}
// 		else if (result == 0 && n->count_cmd == 1)
// 			result = execute_builtin(&n->cmds[i_cmd], d, n);
// 		cmd = cmd -> nxt;
// 	}
// 	ft_wait_child_processes(&result, num, pid);
// 	signal(SIGINT, &sig_handler_fork); // mb sig_handler_mai n
// 	return (result);
// }

// can extern cmd change the env ?
static void *exec_extern_cmd(t_cmd *cmd, t_data **d)
{
	int		pid;
	int		status;
	char	**env_array;
	int		len_env;
	char	*path;

	pid = fork();
	if (pid < -1)
		return (printf("%s : fork failed\n", cmd->arg[0]), rmv_cmd(cmd, d), NULL); // exic code ?
	if (pid == 0)
	{
		(*d)->exit_c = 0; // code ?
		env_array = env_to_array(d);
		len_env = len_env_(d);
		path = path_(cmd, d);
		if (path == NULL)
			path = ".";
		execve(path, cmd->arg, env_array); //if env_array != NULL ?
		free_env_array(env_array, len_env);
	}
	else
		wait(&status);
	return (NULL);
}

void	*start_redirs(t_cmd *cmd, t_data **d)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1) // дубл. дескриптора => stdout в файл
			return (printf("%s : dup2 failed\n", cmd->arg[0]), rmv_cmd(cmd, d), NULL); // exit_code = 127, if (errno != 2) exit_c = 126;
		close(cmd->fd_in);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
			return (printf("%s : dup2 failed\n", cmd->arg[0]), rmv_cmd(cmd, d), NULL); // exit_code = 127, if (errno != 2) exit_c = 126;
		close(cmd->fd_out);
	}
	return (NULL);
}

void	*stop_redirs(t_cmd *cmd, t_data **d)
{
	//if (cmd->fd_in != STDIN_FILENO)
		if (dup2((*d)->saved_stdin, STDIN_FILENO) == -1) // восстановить исходный stdout
			return (printf("%s : dup2 failedd\n", cmd->arg[0]), rmv_cmd(cmd, d), NULL); // exit_code = 127, if (errno != 2) exit_c = 126;
	//if (cmd->fd_out != STDOUT_FILENO)
		if (dup2((*d)->saved_stdout, STDOUT_FILENO) == -1)
			return (printf("%s : dup2 failed\n", cmd->arg[0]), rmv_cmd(cmd, d), NULL); // exit_code = 127, if (errno != 2) exit_c = 126;
	(void)cmd;
	(void)d;
	return (NULL);
}

void	exec_cmds(t_data **d)
{
	t_cmd	*cmd;

	cmd = *((*d)->cmds);
	while (cmd != NULL)
	{

		(*d)->curr_cmd = cmd; // not used ?
		start_redirs(cmd, d);
		// à la premiere erreur (le droit decriture pour les redir out, etc) ca fait tout fail
		calc_dollar_conversions(cmd, d);
		remove_quotes(cmd);
		if (strcmp_(cmd->arg[0], "echo") == 0)
			exec_echo(cmd);
		else if (strcmp_(cmd->arg[0], "env") == 0)
			exec_env(d);
		else if (strcmp_(cmd->arg[0], "pwd") == 0)
			exec_pwd(cmd, d);
		else if (strcmp_(cmd->arg[0], "cd") == 0)
			exec_cd(cmd, d);
		else if (strcmp_(cmd->arg[0], "export") == 0)
			exec_export(cmd, d);
		else if (strcmp_(cmd->arg[0], "unset") == 0)
			exec_unset(cmd, d);
		else if (strcmp_(cmd->arg[0], "exit") == 0)
			exec_exit(cmd, d);
		else
			exec_extern_cmd(cmd, d);
		stop_redirs(cmd, d);
		cmd = cmd->nxt;
	}
}
