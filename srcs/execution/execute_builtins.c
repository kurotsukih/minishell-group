#include "minishell.h"

int	ft_execute_cd(t_list *params, t_list *env)
{
	int		count;
	char	*home_dir;

	count = ft_lstsize(params);
	if (count == 0)
	{
		home_dir = NULL;
		while (env)
		{
			if (ft_strncmp((char *)env->content, "HOME=", 5) == 0)
				home_dir = ((char *)env->content + 5);
			env = env->next;
		}
		if (home_dir == NULL)
			return (exit_(-1, "bash: cd: HOME not set\n", NULL, NULL, NULL, NULL), -1);
		if (chdir(home_dir) == -1)
			return (exit_(-1, "bash: cd: HOME not set properly%s\n", home_dir, NULL, NULL, NULL), -1);
	}
	else if (count > 1)
		return (exit_(-1, "bash: cd: Too many arguments\n", NULL, NULL, NULL, NULL), -1); // str to free?
	else if (chdir((char *)params->content) == -1)
		return (exit_(-1, (char *)params->content, NULL, NULL, NULL, NULL), -1); // str to free?
	return (0);
}

void	ft_execute_echo(t_list *token)
{
	int	is_n;

	is_n = 0;
	while (token && ft_strcmp_alt((char *)token->content) == 0)
	{
		is_n = 1;
		token = token->next;
	}
	while (token)
	{
		ft_printf("%s", (char *)token->content);
		if (token->next)
			ft_printf(" ");
		token = token->next;
	}
	if (is_n == 0)
		ft_printf("\n");
}

void	ft_execute_env(t_list *env)
{
	while (env)
	{
		if (ft_strchr((char *)env->content, '='))
			ft_printf("%s\n", (char *)env->content);
		env = env->next;
	}
}

int	ft_execute_exit(t_data *d, t_node *n, t_list *token)
{
	char	*str;
	int		code;

	if (!token)
	{
		code = d->exit_code;
		return (ft_clean_tree(n), free_redirections(*(&(d->env))), exit(code), 0);
	}
	str = (char *)token->content;
	if (ft_isnum(str) != 1)
	{
		exit_(-1, "bash: exit: %s: numeric argument required", str, NULL, NULL, NULL);
		return (ft_clean_tree(n), free_redirections(*(&(d->env))), exit(2), 0);
	}
	if (ft_lstsize(token) > 1)
		return (exit_(-1, "bash: exit: too many arguments\n", NULL, NULL, NULL, NULL), 1);
	code = ft_abs(ft_atoi(str) % 256);
	return (ft_clean_tree(n), free_redirections(*(&(d->env))), exit(code), 0);
}

int	ft_execute_builtin(t_cmd *cmd, t_data *d, t_node *n)
{
	int	result;

	result = 0;
	if (ft_strcmp((char *)cmd->params->content, "cd") == 0)
		result = ft_execute_cd(cmd->params->next, d->env);
	else if (ft_strcmp((char *)cmd->params->content, "pwd") == 0)
		ft_execute_pwd();
	else if (ft_strcmp((char *)cmd->params->content, "echo") == 0)
		ft_execute_echo(cmd->params->next);
	else if (ft_strcmp((char *)cmd->params->content, "export") == 0)
		result = ft_execute_export(cmd->params->next, &d->env);
	else if (ft_strcmp((char *)cmd->params->content, "unset") == 0)
		ft_execute_unset(&d->env, cmd->params->next);
	else if (ft_strcmp((char *)cmd->params->content, "env") == 0)
		ft_execute_env(d->env);
	else if (ft_strcmp((char *)cmd->params->content, "exit") == 0)
		result = ft_execute_exit(d, n, cmd->params->next);
	return (result);
}
