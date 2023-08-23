// #include "minishell.h"

// t_list	*ft_preprocess_parameter(t_list *token, t_cmd *cmd, int i_cmd)
// {
// 	t_list	*temp;

// 	temp = token->next;
// 	token = ft_lstretrieve(&token, token);
// 	ft_lstadd_back(&(cmd[i_cmd].params), token);
// 	return (temp);
// }

// t_list	*ft_preprocess_redirection(t_list *token, t_cmd *cmd, int i_cmd)
// {
// 	t_list	*temp;

// 	free(token->content);
// 	token->content = token->next->content;
// 	ft_lstremove(&token, token->next, NULL);
// 	temp = token->next;
// 	token = ft_lstretrieve(&token, token);
// 	ft_lstadd_back(&(cmd[i_cmd].redir), token);
// 	return (temp);
// }

// void	ft_preprocess_cmd(t_cmd *cmds, t_list *t)
// {
// 	t_list	*temp;
// 	int		i_cmd;

// 	i_cmd = 0;
// 	while (t)
// 	{
// 		if (t->type == PARAM)
// 			t = ft_preprocess_parameter(t, cmds, i_cmd);
// 		else if (t->type == REDIR_IN || t->type == REDIR_IN || t->type == HEREDOC || t->type == REDIR_OUT2)
// 			t = ft_preprocess_redirection(t, cmds, i_cmd);
// 		else
// 		{
// 			i_cmd++;
// 			temp = t->next;
// 			ft_lstdelone(t);
// 			t = temp;
// 			t->prev = NULL;
// 		}
// 	}
// }
