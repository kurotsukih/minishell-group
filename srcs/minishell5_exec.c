#include "minishell.h"
#include "minishell.h"

int	exec_export(t_list *cmd, t_env **env)
{
	// t_env	*cur;
	t_env	*new;
	int		i;

	if (cmd->nb_args == 0)
		return (exec_env(env), 0);
	i = -1;
	while (++i < cmd->nb_args)
	{
		// cur = *env;
		// while (cur != NULL)
		// {
		// 	if (ft_strcmp(cur->key, part_before_sign_equal(cmd->args[i])))
		// 	{
		// 		free(cur->val);
		// 		cur->val = part_after_sign_equal(cmd->args[i]);
		// 		if (cur->val == NULL)
		// 			{} // ?
		// 		break ;
		// 	}
		// 	cur = cur->nxt;
		// }
		new = NULL;
		new = (t_env *)malloc(sizeof(t_env));
		if (new == NULL)
			return (-1);
		new->key = part_before_sign_equal(cmd->args[i]);
		new->val = part_after_sign_equal(cmd->args[i]);
		new->nxt = *env;
		*env = new;
	}
	return (0);
}

// void	exec_unset(t_list **env, t_list *head)
// {
// 	t_list	*token;
// 	char	*str;
// 	int		len;
// 	char	*cmd;

// 	if (!head)
// 		return ;
// 	cmd = (char *)head->content;
// 	len = ft_strlen(cmd);
// 	token = *env;
// 	while (token)
// 	{
// 		str = (char *)token->content;
// 		if (ft_strncmp(str, cmd, len) == 0 && str[len] == '=')
// 			break ;
// 		token = token->next;
// 	}
// 	if (!token)
// 		return ;
// 	ft_lstremove(env, token);
// }


// int	exec_exit(t_data *d, t_node *n, t_list *token)
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
