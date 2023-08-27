#include "headers.h"

int	init_cmd(t_cmds **new, char *redirect)
{
	*new = (t_cmds *)malloc(sizeof(t_cmds));
	if (*new == NULL)
		return (-1);
	(*new)->nxt = NULL;
	(*new)->prv = NULL;
	(*new)->nb_args = 0;
	(*new)->args = NULL;
	(*new)->redirect = redirect;
	(*new)->is_filename = 0;
	return (0);
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
	else if (s[1] == '>' && s[1] != '\0' && s[2] != '>')
		return (">");
	else if (s[1] == '<' && s[1] != '\0' && s[2] != '<')
		return ("<");
	else if (s[1] == '|')
		return ("|");
	else
		return ("");
}

int	nb_args_(char *s, int len)
{
	int	nb_args;
	int	i;

	nb_args = 0;
	i = -1;
	while (++i < len)
		if (mod_(s[i]) == QUOTES0 && s[i] != ' ' && (s[i + 1] == ' ' || s[i + 1] == '\0' || s[i + 1] == '\'' || s[i + 1] == '\"'))
			nb_args++;
	return (nb_args);
}

void print_cmd(t_cmds *cmd)
{
	int	i;

	if (cmd == NULL)
	{
		printf("  cmd = NULL\n");
		return ;
	}
	printf("  %p [%s] %d agrs : ", cmd, cmd->args[0], cmd->nb_args);
	if (cmd->args != NULL)
	{
		i = -1;
		while (++i < cmd->nb_args)
			printf("[%s] ", cmd->args[i]);
	}
	printf(": [%s]\n", cmd->redirect);
	(void)i;
}

void print_cmds(t_data **d)
{
	t_cmds	*cmd;

	printf("LIST %14p:\n", (*d)->cmds);
	if ((*d)->cmds == NULL || *((*d)->cmds) == NULL)
	{
		printf("  empty\n");
		return ;
	}
	cmd = *((*d)->cmds);
	while (cmd != NULL)
	{
		print_cmd(cmd);
		cmd = cmd->nxt;
	}
}
