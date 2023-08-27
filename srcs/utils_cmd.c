#include "headers.h"

void	init_cmd(t_cmds **new, char *redirect, t_data **d)
{
	*new = (t_cmds *)malloc_(sizeof(t_cmds), d);
	(*new)->nb_args_max = INT_MAX;
	(*new)->args = NULL;
	(*new)->redirect = redirect;
	(*new)->to_free = NULL;
	(*new)->fd_in = STDIN_FILENO;
	(*new)->fd_out = STDOUT_FILENO;
	(*new)->nxt = NULL;
	(*new)->prv = NULL;
}

int	mod_(char c)
{
	static char	mod = QUOTES0;

	if (c == REINIT_QUOTES)
		mod = QUOTES0;
	else if (mod == QUOTES0 && c == '\'')
		mod = QUOTES1;
	else if (mod == QUOTES0 && c == '\"')
		mod = QUOTES2;
	else if (mod == QUOTES1 && c == '\'')
		mod = QUOTES0;
	else if (mod == QUOTES2 && c == '\"')
		mod = QUOTES0;
	return (mod);
}

char	*redirect_(char *s)
{
	if (s[0] == '>' && s[1] == '>')
		return (">>");
	else if (s[0] == '<' && s[1] == '<')
		return ("<<");
	else if (s[0] == '>' && s[1] != '\0' && s[2] != '>')
		return (">");
	else if (s[0] == '<' && s[1] != '\0' && s[2] != '<')
		return ("<");
	else if (s[0] == '|')
		return ("|");
	else
		return ("");
}

int	nb_args_(char *s, int len)
{
	int	nb_args;
	int	i;

	mod_(REINIT_QUOTES);
	nb_args = 0;
	i = -1;
	while (++i < len)
		if (mod_(s[i]) == QUOTES0 && s[i] != ' ' && (s[i + 1] == ' ' || s[i + 1] == '\0' || s[i + 1] == '\'' || s[i + 1] == '\"' || i == len - 1))
			nb_args++;
	return (nb_args);
}

void print_cmds(char *msg, t_data **d)
{
	int		j;
	t_cmds	*cmd;

	printf("LIST %s %14p:\n", msg, (*d)->cmds);
	if ((*d)->cmds == NULL || *((*d)->cmds) == NULL)
	{
		printf("  empty\n");
		return ;
	}
	cmd = *((*d)->cmds);
	// int i = -1;
	// while (cmd != NULL && ++i < cmd->nb_args)
	while (cmd != NULL)
	{
		printf("  %p [%s] %d agrs : ", cmd, cmd->args[0], cmd->nb_args);
		if (cmd->args != NULL)
		{
			j = -1;
			while (++j < cmd->nb_args)
				printf("[%s] ", cmd->args[j]);
		}
		printf(": [%s]\n", cmd->redirect);
		if (cmd == NULL)
			break ; ////
		cmd = cmd->nxt;
	}
}

void delete_cmd_from_list(t_cmds *cmd, t_data **d)
{
	// int		i;

	// printf("delete cmd %s\n", cmd->args[0]);
	if (cmd == NULL)
		return ;
	// i = -1;
	// while(++i < cmd->nb_args)
	// 	free(cmd->args[i]);
	// free(cmd->args);
	if (cmd->prv == NULL)
		*((*d)->cmds) = cmd->nxt;
	else
		cmd->prv->nxt = cmd->nxt;
}

void	delete_cmds(t_data **d)
{
	t_cmds	*cmd_to_del;

	while(*((*d)->cmds) != NULL)
	{
		cmd_to_del = *((*d)->cmds);
		*((*d)->cmds) = (*((*d)->cmds))->nxt;
		delete_cmd_from_list(cmd_to_del, d);
	}
}