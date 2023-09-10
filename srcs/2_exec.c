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
	t_lst	*cur;
	int		option_n;

	option_n = NO;
	cur = (*((*d)->args))->nxt;
	while (cur != NULL)
	{
		if (ft_strcmp((char *)(cur->val), "-n") == 0)
			option_n = YES;
		else
		{
			write_fd(1, cur->val);
			if (cur->nxt != NULL)
				write_fd(1, " ");
		}
		cur = cur->nxt;
	}
	if (option_n == YES)
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
		dir = NULL;
		dir = get_val_from_env("HOME", d);
		if (dir == NULL)
			return (err_cmd("cd : variable HOME not found", -1, d));
	}
	else if (len_lst((*d)->args) == 2)
		dir = (*((*d)->args))->nxt->val;
	res = chdir(dir);
	free_(dir);
	if (res == -1)
		return (err_cmd("cd : chdir failure", -1, d));
	return (OK);
}

static int	exec_extern_cmd(t_data **d)
{
	int		pid;
	char	*path;
	char	**args_arr;
	char	**env_arr;
	int		status;

	path = path_(d); // un chemin relatif ou absolu ?
	if (path == NULL)
		path = "."; // ?
	pid = fork();
	if (pid < -1)
		free_all_and_exit("fork pb", -1, d);
	if (pid == 0)
	{
		args_arr = lst_to_arr((*d)->args, d);
		env_arr = lst_to_arr((*d)->env, d);
		execve(path, args_arr, env_arr); //if env_array == NULL ? // every execve substitue le processus ???!!!
		free_2_array(args_arr); //not executed ?
		free_2_array(env_arr); //not executed ?
	}
	else
		wait(&status);
	return (OK);
}

// printf("%d ", *((int *)(cur->val)));
static int	exec_cmd_fd(int fd, t_data **d)
{
	char *cmd;

	printf("exec cdm fd %d\n", fd);
	// printf("dup 2 %d\n", *((int *)((*d)->outs)[i])
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (err_cmd("dup2 stdout pb", -1, d));
	close(fd);
	cmd = ((char *)((((*d)->args[0]))->val));
	printf("cmd = %s\n", cmd);
	if (ft_strcmp(cmd, "echo") == 0)
		exec_echo(d);
	else if (ft_strcmp(cmd, "cd") == 0)
		exec_cd(d);
	else if (ft_strcmp(cmd, "pwd") == 0) 
	{
		printf("call exec pwd\n");
		exec_pwd(d);
	}
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
	if (dup2((*d)->saved_stdout, STDOUT_FILENO) == -1)
		return (err_cmd("dup2 stdout pb", -1, d));
	unlink(TMP_FILE);
	printf("exec cdm fd return\n");
	return (OK);
}

int	exec_cmd(t_data **d)
{
	t_lst *out;

	// if (dup2((*d)->in, STDIN_FILENO) == -1)
	// 	return (err_cmd("dup2 start stdin pb", -1, d));
	// close((*d)->in); ??? creates problems (only for the 2nd cmd-line !???)
	printf("exec cdm\n");
	out = *((*d)->outs);
	while (out != NULL)
	{
		exec_cmd_fd(*((int *)(out->val)), d);
		out = out->nxt;
	}
	// if (dup2((*d)->saved_stdin, STDIN_FILENO) == -1)
	// 	return (err_cmd("dup2 end stdin pb", -1, d));
	printf("exec cdm return\n");
	return (OK);
}
