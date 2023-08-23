#include "minishell.h"

void	exec_echo(t_list *cmd)
{
	int	option_n;
	int	i;

	option_n = 0;
	i = -1;
	while (++i < cmd->nb_args)
	{
		ft_printf("%s", cmd->args[i]);
		if (i != cmd->nb_args - 1)
			ft_printf(" ");
		if (cmd->args[i][0] == '-' && cmd->args[i][1] == 'n' && cmd->args[i][2] == '\0')
			option_n = 1;
	}
	if (option_n == 0)
		ft_printf("\n");
}

int	exec_cmds(t_list **l, char **env)
{
	t_list *cmd;
	int	result;

	cmd = *l;
	while (cmd != NULL)
	{
		result = 0;
		if (ft_strcmp(cmd->cmd, "echo") == 0)
			exec_echo(cmd);
		// else if (ft_strcmp((char *)cmd->params->content, "pwd") == 0)
		// 	ft_execute_pwd();
		// else if (ft_strcmp((char *)cmd->params->content, "unset") == 0)
		// 	ft_execute_unset(&d->env, cmd->params->next);
		// else if (ft_strcmp((char *)cmd->params->content, "env") == 0)
		// 	ft_execute_env(d->env);
		// else if (ft_strcmp((char *)cmd->params->content, "cd") == 0)
		// 	result = ft_execute_cd(cmd->params->next, d->env);
		// else if (ft_strcmp((char *)cmd->params->content, "export") == 0)
		// 	result = ft_execute_export(cmd->params->next, &d->env);
		// else if (ft_strcmp((char *)cmd->params->content, "exit") == 0)
		// 	result = ft_execute_exit(d, n, cmd->params->next);
		cmd = cmd->nxt;
	}
	(void)env;
	return (result);
}

// int	ft_execute_cd(t_list *params, t_list *env)
// {
// 	int		nb_params;
// 	char	*home_dir;

// 	nb_params = ft_lstsize(params);
// 	if (nb_params == 0)
// 	{
// 		home_dir = NULL;
// 		while (env)
// 		{
// 			if (ft_strncmp((char *)env->content, "HOME=", 5) == 0)
// 				home_dir = ((char *)env->content + 5);
// 			env = env->next;
// 		}
// 		if (home_dir == NULL)
// 			return (exit_(-1, "bash: cd: HOME not set\n", NULL, NULL, NULL, NULL), -1);
// 		if (chdir(home_dir) == -1)
// 			return (exit_(-1, "bash: cd: HOME not set properly%s\n", home_dir, NULL, NULL, NULL), -1);
// 	}
// 	else if (nb_params > 1)
// 		return (exit_(-1, "bash: cd: Too many arguments\n", NULL, NULL, NULL, NULL), -1); // str to free?
// 	else if (chdir((char *)params->content) == -1)
// 		return (exit_(-1, (char *)params->content, NULL, NULL, NULL, NULL), -1); // str to free?
// 	return (0);
// }

// void	ft_execute_env(t_list *env)
// {
// 	while (env)
// 	{
// 		if (ft_strchr((char *)env->content, '='))
// 			ft_printf("%s\n", (char *)env->content);
// 		env = env->next;
// 	}
// }

// int	ft_execute_exit(t_data *d, t_node *n, t_list *token)
// {
// 	char	*str;
// 	int		code;

// 	if (!token)
// 	{
// 		code = d->exit_code;
// 		return (ft_clean_tree(n), free_redirections(*(&(d->env))), exit(code), 0);
// 	}
// 	str = (char *)token->content;
// 	if (ft_isnum(str) != 1)
// 	{
// 		exit_(-1, "bash: exit: %s: numeric argument required", str, NULL, NULL, NULL);
// 		return (ft_clean_tree(n), free_redirections(*(&(d->env))), exit(2), 0);
// 	}
// 	if (ft_lstsize(token) > 1)
// 		return (exit_(-1, "bash: exit: too many arguments\n", NULL, NULL, NULL, NULL), 1);
// 	code = ft_abs(ft_atoi(str) % 256);
// 	return (ft_clean_tree(n), free_redirections(*(&(d->env))), exit(code), 0);
// }

