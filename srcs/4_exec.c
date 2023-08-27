#include "headers.h"

static int	there_are_unclosed_quotes(t_cmds *cmd)
{
	int		mod;
	int		i;

	mod_(REINIT_QUOTES);
	i = -1;
	while (cmd->args[0][++i] != '\0')
		mod = mod_(cmd->args[0][i]);
	if (mod != QUOTES0)
		return (printf("unclodes quotes\n"), 1);
	return (0);
}

int	args_are_correct(t_cmds *cmd, t_data **d)
{
	if (!cmd->args)
	{
		(*d)->exit_c = 255;
		return (0);
	}
	if (cmd->nb_args > cmd->nb_args_max)
	{
		printf("%s: Too many arguments", cmd->args[0]);
		// d->exit_code = 
		return (0);
	}
	if (ft_strcmp(cmd->args[0], "exit") == 0 && !ft_atoi(cmd->args[1])) ////
	{
		printf("exit: numeric argument required");
		(*d)->exit_c =2;
		return (0);
	}
	if (there_are_unclosed_quotes(cmd))
	{
		printf("%s: unclosed quotes", cmd->args[0]);
		// d->exit_code = 
		return (0);
	}
	return (1);
}

static char	*path2_(char *s1, int len_s1, t_cmds *cmd, t_data **d)
{
	int		i;
	int		j;
	char	*dest;

	dest = (char *) malloc_(len_s1 + ft_strlen(cmd->args[0]) + 1, d);
	i = 0;
	while (i < len_s1)
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i++] = '/';
	j = 0;
	while (cmd->args[0][j] != '\0')
	{
		dest[i] = cmd->args[0][j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

static char	*path_(t_cmds *cmd, t_data **d)
{
	char	*paths;
	char	*path;
	int		i;
	int		i_beg;

	paths = get_value_from_env("PATH", d);
	if (!paths)
		return ((printf("no var env PATh"), *d)->exit_c = 127, NULL);
	i = -1;
	i_beg = 0;
	while (++i < (int)ft_strlen(paths))
		if (paths[i] == '\0' || paths[i] == ':')
		{
			path = path2_(&(paths[i_beg]), i - i_beg, cmd, d);
			if (access(path, X_OK) == 0)
				return (path);
			if (errno != 2)
				(*d)->exit_c = 126;
			i_beg = i + 1;
		}
	return (printf("command not found"), (*d)->exit_c = 127, NULL);
}

void	exec_extern_cmd(t_cmds *cmd, t_data **d)
{
	char	*path;
	int		pid;
	int		status;
	char	**env_array;

	pid = fork();
	if (pid < -1)
		free_all_and_exit("fork error", d);
	if (pid == 0)
	{
		(*d)->exit_c = 0;
		path = path_(cmd, d);
		env_array = env_to_array(d);
		if (path != NULL)
			execve(path, cmd->args, env_array);
		free(path);
		free_charchar(env_array, len_env(d));
	}
	else
		wait(&status);
}

// wc -l < infile
// dupliquer STDIN dans l'infile

void	treat_redirect(t_cmds **cmd, t_data **d)
{
	if ((*cmd)->nxt != NULL && (*cmd)->redirect[0] == '<')
	{
		(*cmd)->fd_in = open((*cmd)->nxt->args[0], O_RDONLY); 
		// if (isatty((*cmd)->fd_in) == 1)
		// 	return (close(return_fd), fd);
		// if ((*cmd)->fd_in != 1)
		// 	close((*cmd)->fd_in);
			// d->exit_c = 
			// return 
		dup2((*cmd)->fd_in, STDIN_FILENO);
		// if !dup2
			// d->exit_c = 
			// return
		//delete_cmd(&(*cmd)->nxt, d);
		(void)d;
	}
}

void	exec_cmds(t_data **d)
{
	t_cmds *cmd;
	t_cmds *copy;

	cmd = *((*d)->cmds);
	while (cmd != NULL)
	{
		printf("exec_cmd %s\n", cmd->args[0]);
		if (!args_are_correct(cmd, d))
			continue; //?
		treat_redirect(&cmd, d);
		printf("treat_redirect %s done\n", cmd->args[0]);
		if (ft_strcmp(cmd->args[0], "echo") == 0)
		{
			printf("call exec_echo %s\n", cmd->args[0]);
			exec_echo(cmd);
			printf("exec_echo %s done\n", cmd->args[0]);
		}
		else if (ft_strcmp(cmd->args[0], "env") == 0 && (*d)->env != NULL)
			exec_env(d);
		else if (ft_strcmp(cmd->args[0], "pwd") == 0)
			exec_pwd();
		else if (ft_strcmp(cmd->args[0], "cd") == 0)
			exec_cd(cmd, d);
		else if (ft_strcmp(cmd->args[0], "export") == 0)
			exec_export(cmd, d);
		else if (ft_strcmp(cmd->args[0], "unset") == 0 && (*d)->env != NULL)
			exec_unset(cmd, d);
		else if (ft_strcmp(cmd->args[0], "exit") == 0)
			exec_exit(d);
		else
			exec_extern_cmd(cmd, d);
		//if (d->exit-c == ...)
			//exit
		copy = cmd;
		// printf("call delete_cmd %s\n", cmd->args[0]);
		printf("call delete_cmd\n");
		delete_cmd(cmd, d);
		printf("retr delete_cmd \n\n");
		cmd = copy->nxt;
	}
}
