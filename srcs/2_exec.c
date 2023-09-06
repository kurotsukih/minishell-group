/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:22:29 by akostrik          #+#    #+#             */
/*   Updated: 2023/09/05 21:04:08 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.h"

int	exec_echo(t_data **d)
{
	int	option_n;
	int	i;

	option_n = NO;
	i = 0;
	while (++i < (*d)->nb_args)
	{
		if (strcmp_((*d)->arg[i], "-n") == 0)
			option_n = YES;
		else
		{
			write(1, (*d)->arg[i], ft_strlen((*d)->arg[i]));
			if (i < (*d)->nb_args - 1)
				write(1, " ", 1);
		}
	}
	if (option_n == YES)
		write(1, "\n", 1);
	return (OK);
}

int	exec_cd(t_data **d)
{
	char	*dir;
	int		res;

	if ((*d)->nb_args > 2)
		return (err_cmd("cd : too many arguments", -1, d));
	if ((*d)->nb_args == 1)
	{
		dir = NULL;
		dir = get_value_from_env("HOME", d);
		if (dir == NULL)
			return (err_cmd("cd : variable HOME not found", -1, d));
		res = chdir(dir);
		free_(dir);
		if (res == -1)
			return (err_cmd("cd : chdir failure", -1, d));
		return (OK);
	}
	if (chdir((*d)->arg[1]) == -1)
		return (err_cmd("cd : chdir failure", -1, d));
	return (OK);
}

static int	exec_extern_cmd(t_data **d)
{
	int		pid;
	char	*path;
	char	**env_array;
	int		len_env;
	int		status;

	path = path_(d); // un chemin relatif ou absolu ?
	if (path == NULL)
		path = ".";
	pid = fork();
	if (pid < -1)
		return (err_cmd("fork pb", -1, d)); // fre e all and exit ?
	if (pid == 0)
	{
		env_array = env_to_array(d);
		len_env = len_env_(d);
		execve(path, (*d)->arg, env_array); //if env_array == NULL ? // every execve substitue le processus ???!!!
		free_2_array(env_array, len_env); //not executed ?
	}
	else
		wait(&status);
	return (OK);
}

static int	exec_1_cmd_1_out(int i, t_data **d)
{
	if (dup2((*d)->out[i], STDOUT_FILENO) == -1)
		return (err_cmd("dup2 stdout pb", -1, d));
	close((*d)->out[i]); // ?
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
	if (dup2((*d)->saved_stdout, STDOUT_FILENO) == -1)
		return (err_cmd("dup2 stdout pb", -1, d));
	unlink(TMP_FILE);
	return (OK);
}

int	exec_1_cmd(t_data **d)
{
	int	i;

	if (dup2((*d)->in, STDIN_FILENO) == -1)
		return (err_cmd("dup2 start stdin pb", -1, d));
	// close((*d)->in); ??? creates problems (only for the 2nd cmd-line !???)
	i = -1;
	while (++i < (*d)->nb_outs)
		exec_1_cmd_1_out(i, d);
	if (dup2((*d)->saved_stdin, STDIN_FILENO) == -1)
		return (err_cmd("dup2 end stdin pb", -1, d));
	return (OK);
}
