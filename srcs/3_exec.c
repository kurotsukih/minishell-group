/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:29 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/14 15:24:59 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

// execve creates a new process with the same ope n file descriptors
static int	exec_extern_cmd(t_data **d)
{
	int		pid;
	char	*path;
	char	**args_arr;
	char	**env_arr;
	int		status;

	pid = fork();
	if (pid < -1)
		free_all_and_exit("fork pb", -1, d);
	if (pid == 0)
	{
		signal(SIGINT, &sig_handler_fork);
		signal(SIGQUIT, &sig_handler_fork); // ???
		path = path_(d); // un chemin relatif ou absolu ?
		if (path == NULL)
			path = "."; // ?
		args_arr = lst_to_arr((*d)->args, d);
		env_arr = lst_to_arr((*d)->env, d);
		if (execve(path, args_arr, env_arr) == -1)
			return(err_cmd("", -1, d));
		free_2_array(args_arr); // not executed because execve has replaced the child process? 
		free_2_array(env_arr);  // does execve free args_arr and env_arr ?
	}
	else
	{
		wait(&status);
		verify_child_exit_code(status, d);
	}
	signal(SIGINT, &sig_handler);
	return (OK);
}

int	exec_cmd(t_data **d)
{
	char *cmd;

	if (dup2((*d)->fd_in, STDIN) == -1)
		return (err_cmd("dup2 start stdin pb", 1, d)); // 1 = general errors
	close((*d)->fd_in);
	if (dup2((*d)->fd_out, STDOUT) == -1)
		return (err_cmd("dup2 stdout pb", 1, d)); // 1 = general errors
	close((*d)->fd_out);
	cmd = (((*d)->args[0])->val);
	if (ft_strcmp(cmd, "echo") == 0)
		exec_echo(d);
	else if (ft_strcmp(cmd, "cd") == 0)
		exec_cd(d);
	else if (ft_strcmp(cmd, "pwd") == 0) 
		exec_pwd(d);
	else if (ft_strcmp(cmd, "export") == 0)
		exec_export(d);
	else if (ft_strcmp(cmd, "unset") == 0)
		exec_unset(d);
	else if (ft_strcmp(cmd, "env") == 0)
		exec_env(d);
	else if (ft_strcmp(cmd, "exit") == 0)
		exec_exit(d);
	else
		exec_extern_cmd(d);
	if (dup2((*d)->saved_stdin, STDIN) == -1)
		return (err_cmd("dup2 end stdin pb", 1, d)); // 1 = general errors
	if (dup2((*d)->saved_stdout, STDOUT) == -1)
		return (err_cmd("dup2 stdout pb", 1, d)); // 1 = general errors
	return (OK);
}

