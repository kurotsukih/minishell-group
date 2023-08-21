#include "minishell.h"

static int	ft_check_operator(int exit_code, char *str)
{
	if (ft_strcmp(str, "&&") == 0)
	{
		if (exit_code == 0)
			return (0);
		return (1);
	}
	if (exit_code == 0)
		return (1);
	return (0);
}

int	ft_exec_recursion(t_node *n, t_data *data, t_node *parent)
{
	if (n->type == PIPE)
	{
		if (!parent)
			n->exit_code = ft_exec_recursion(n->left, data, NULL);
		else
		{
			n->exit_code = 0;
			if (!ft_check_operator(parent->exit_code, parent->elems->content))
				n->exit_code = ft_exec_recursion(n->left, data, NULL);
		}
		if (n->exit_code != 0)
			n->left = NULL;
		if (n->right->is_micro == n->is_micro && n->right->type == PIPE)
			n->exit_code = ft_exec_recursion(n->right, data, n);
		else if (!ft_check_operator(n->exit_code, n->elems->content))
			n->exit_code = ft_exec_recursion(n->right, data, n);
		if (n->exit_code != 0)
			n->right = NULL;
	}
	else
		return (ft_exec_command(n, data));
	return (n->exit_code);
}

void	ft_execution(t_data *data)
{
	if (ft_preprocess(data->n) == -1)
	{
		ft_clean_tree(data->n);
		return ;
	}
	data->exit_code = ft_exec_recursion(data->n, data, NULL);
}

