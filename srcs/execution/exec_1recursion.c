// #include "minishell.h"

// static int	ft_check_operator(int exit_code, char *str)
// {
// 	if (ft_strcmp(str, "&&") == 0)
// 	{
// 		if (exit_code == 0)
// 			return (0);
// 		return (1);
// 	}
// 	if (exit_code == 0)
// 		return (1);
// 	return (0);
// }

// int	ft_exec_recursion(t_node *n, t_data *d, t_node *parent)
// {
// 	if (n->type == PIPE)
// 	{
// 		if (!parent)
// 			n->exit_code = ft_exec_recursion(n->left, d, NULL);
// 		else
// 		{
// 			n->exit_code = 0;
// 			if (!ft_check_operator(parent->exit_code, parent->elems->content))
// 				n->exit_code = ft_exec_recursion(n->left, d, NULL);
// 		}
// 		if (n->exit_code != 0)
// 			n->left = NULL;
// 		if (n->right->is_micro == n->is_micro && n->right->type == PIPE)
// 			n->exit_code = ft_exec_recursion(n->right, d, n);
// 		else if (!ft_check_operator(n->exit_code, n->elems->content))
// 			n->exit_ft_exec_commandcode = ft_exec_recursion(n->right, d, n);
// 		if (n->exit_code != 0)
// 			n->right = NULL;
// 	}
// 	else
// 		return ((n, d));
// 	return (n->exit_code);
// }

// void	ft_execution(t_data *d)
// {
// 	if (ft_preprocess(d->n) == -1)
// 	{
// 		ft_clean_tree(d->n);
// 		return ;
// 	}
// 	d->exit_code = ft_exec_recursion(d->n, d, NULL);
// }

