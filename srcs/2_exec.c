/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:29 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/12 15:04:28 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

int	exec_echo(t_data **d)
{
	t_lst	*cur;
	int		option_n;

	option_n = NO;
	cur = (*((*d)->args))->nxt;
	while (cur != NULL)
	{
		if (ft_strcmp(cur->val, "-n") == 0)
			option_n = YES;
		else
		{
			write_fd(1, cur->val);
			if (cur->nxt != NULL)
				write_fd(1, " ");  // but for [ echo "$A"R ]: we should not print " "
		}
		cur = cur->nxt;
	}
	if (option_n == NO)
		write_fd(1, "\n");
	return (OK);
}

int	exec_cd(t_data **d)
{
	char	*dir;
	int		res;

	if (len_lst((*d)->args) > 2)
		return (err_cmd("cd : too many arguments", -1, d));
	if (len_lst((*d)->args) == 1)
	{
		dir = get_val_from_env("HOME", d);
		if (dir == NULL)
			return (err_cmd("cd : variable HOME not found", -1, d));
	}
	else if (len_lst((*d)->args) == 2)
		dir = (*((*d)->args))->nxt->val;
	res = chdir(dir);
	// if (len_lst((*d)->args) == 1)
	// 	free_(dir);
	if (res == -1)
		return (err_cmd("cd : chdir failure", -1, d));
	return (OK);
}

// execve creates a new process with the same ope n file descriptors as the parent
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
		path = path_(d); // un chemin relatif ou absolu ?
		if (path == NULL)
			path = "."; // ?
		args_arr = lst_to_arr((*d)->args, d);
		env_arr = lst_to_arr((*d)->env, d);
		if (execve(path, args_arr, env_arr) == -1)
			return(err_cmd("", -1, d));
		free_2_array(args_arr); // Not executed because execve has replaced the child process? 
		free_2_array(env_arr); // Does execve free args_arr and env_arr ?
	}
	else
		wait(&status);
	return (OK);
}

int	exec_cmd(t_data **d)
{
	char *cmd;

	if (dup2((*d)->fd_in, STDIN) == -1)
		return (err_cmd("dup2 start stdin pb", -1, d));
	close((*d)->fd_in);
	if (dup2((*d)->fd_out, STDOUT) == -1)
		return (err_cmd("dup2 stdout pb", -1, d));
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
		return (err_cmd("dup2 end stdin pb", -1, d));
	if (dup2((*d)->saved_stdout, STDOUT) == -1)
		return (err_cmd("dup2 stdout pb", -1, d));
	return (OK);
}
