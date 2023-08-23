// #include "minishell.h"


// void	ft_execute_unset(t_list **env, t_list *head)
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


/////////////////////////////////////////////////////////////

// static void	ft_print_export_token(char *str)
// {
// 	char	*equal_sign;
// 	char	*value;

// 	equal_sign = ft_strchr(str, '=');
// 	if (equal_sign != NULL)
// 	{
// 		*equal_sign = '\0';
// 		value = equal_sign + 1;
// 		ft_printf("export %s=\"%s\"\n", str, value);
// 		*equal_sign = '=';
// 	}
// 	else
// 		ft_printf("export %s\n", str);
// }

// static void	ft_remove_if_exist(t_list **env, char *new_value)
// {
// 	t_list	*token;
// 	t_list	*temp;
// 	char	*str;
// 	int		i;

// 	token = *env;
// 	while (token)
// 	{
// 		i = 0;
// 		str = (char *)token->content;
// 		while (str[i] && new_value[i] && str[i] != '=')
// 			i++;
// 		temp = token->next;
// 		if (str[i] == '=' || str[i] == '\0')
// 			if (new_value[i] == '=' || new_value[i] == '\0')
// 				if (ft_strncmp(str, new_value, i) == 0)
// 					ft_lstremove(env, token);
// 		token = temp;
// 	}
// }

// int	ft_execute_export(t_list *params, t_list **env)
// {
// 	t_list	*token;
// 	char	*new_value;

// 	if (!params)
// 	{
// 		ft_bubble_sort_list(*env);
// 		while (*env)
// 		{
// 			ft_print_export_token((char *)(*env)->content);
// 			*env = (*env)->next;
// 		}
// 		return (0);
// 	}
// 	new_value = ft_strdup((char *)params->content);
// 	if (!new_value)
// 		return (-1);
// 	token = ft_lstnew(new_value, 1);
// 	if (!token)
// 		return (-1);
// 	ft_remove_if_exist(env, new_value);
// 	ft_lstadd_front(env, token);
// 	return (0);
// }

