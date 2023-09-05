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

int	exec_echo(t_data **d)
{
	int	option_n;
	int	i;

	option_n = NO;
	i = 0;
	while (++i < (*d)->nb_args)
		if (strcmp_((*d)->arg[i], "-n") == 0)
			option_n = YES;
		else
			{
				printf("%s", (*d)->arg[i]);
				if (i < (*d)->nb_args - 1)
					printf(" ");
			}
	if (option_n == YES)
		printf("\n");
	return (OK);
}

int	exec_cd(t_data **d)
{
	char	*dir;
	int		res;

	if ((*d)->nb_args > 2)
		return (printf("cd : too many arguments\n"), OK);
	if ((*d)->nb_args == 1)
	{
		dir = NULL;
		dir = get_value_from_env("HOME", d);
		if (dir == NULL)
			return (printf("cd : variable HOME not found\n"), OK); 	// exic code ?
		res = chdir(dir);
		free_(dir);
		if (res == -1)
			return (printf("cd : chdir failure\n"), OK); 	// exic code ?
		return (OK);
	}
	if (chdir((*d)->arg[1]) == -1)
		return (printf("cd : chdir failure\n"), OK); 	// exic code ?
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
		return (printf("%s : fork pb\n", (*d)->arg[0]), -1); // fre e all and exit ?
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

static int	exec_1_cmd_to_1_out(int i, t_data **d)
{
	if (dup2((*d)->out[i], STDOUT_FILENO) == -1)
		return (printf("%s : dup2 pb start out\n", (*d)->arg[0]), OK); // exit_code = 127, if (errno != 2) exit_c = 126;
	close((*d)->out[i]);
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
	if (dup2((*d)->saved_stdout, STDOUT_FILENO) == -1)
		return (printf("%s : dup2 pb end out\n", (*d)->arg[0]), OK);
	return (OK);
}

int	exec_1_cmd_to_all_outs(t_data **d)
{
	int	i;

	if (dup2((*d)->in, STDIN_FILENO) == -1)
		return (printf("%s : dup2 pb start in\n", (*d)->arg[0]), OK); // exit_code = 127, if (errno != 2) exit_c = 126;
	close((*d)->in);
	i = -1;
	while (++i < (*d)->nb_outs)
		exec_1_cmd_to_1_out(i, d);
	if (dup2((*d)->saved_stdin, STDIN_FILENO) == -1)
		return (printf("%s : dup2 pb end in\n", (*d)->arg[0]), OK);
	return (OK);
}
