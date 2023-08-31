#include "headers.h"

void	init_cmd(t_cmds **new, t_data **d)
{
	*new = (t_cmds *)malloc_(sizeof(t_cmds), d);
	(*new)->nb_max_args = INT_MAX;
	(*new)->args = NULL;
	(*new)->redir_in = NULL;
	(*new)->redir_out = NULL;
	(*new)->redir_out2 = NULL;
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

char	*redir_(char *s)
{
	if (s[0] == '>' && s[1] == '>')
		return (">>");
	else if (s[0] == '<' && s[1] == '<')
		return ("<<");
	else if (s[0] == '>')
		return (">");
	else if (s[0] == '<')
		return ("<");
	else
		return ("");
}

int	nb_args_(char *s0, int len, t_data **d)
{
	int		nb_args;
	int		i;
	char	*s;

	s = strdup_and_erase_redirs(s0, d);
	mod_(REINIT_QUOTES);
	nb_args = 0;
	i = -1;
	while (++i < len)
		if (mod_(s[i]) == QUOTES0 && s[i] != ' ' && (s[i + 1] == ' ' || s[i + 1] == '\0' || s[i + 1] == '\'' || s[i + 1] == '\"' || i == len - 1))
				nb_args++;
	free(s);
	return (nb_args);
}

void print_cmds(char *msg, t_data **d)
{
	int		i;
	t_cmds	*cmd;

	printf("LIST %s %14p->%14p:\n  ", msg, (*d)->cmds, (*d)->cmds == NULL ? 0 : *((*d)->cmds));
	if ((*d)->cmds == NULL || *((*d)->cmds) == NULL)
	{
		printf("empty\n");
		return ;
	}
	cmd = *((*d)->cmds);
	while (cmd != NULL)
	{
		if (cmd->args != NULL)
		{
			i = -1;
			while (++i < cmd->nb_args)
				printf("%s : ", cmd->args[i]);
		}
		else
			printf("args = NULL");
		printf("\n  redirs [%s %s %s]\n  ", cmd->redir_out, cmd->redir_out2, cmd->redir_in);
		cmd = cmd->nxt;
	}
	printf("\n");
}

void del_cmd_from_list(t_cmds *cmd, t_data **d)
{
	int		i;
	t_cmds	*to_free;

	if (cmd == NULL)
		return ;
	i = -1;
	while(++i < cmd->nb_args)
	{
		free(cmd->args[i]);
		cmd->args[i] = NULL;
	}
	free(cmd->args);
	free(cmd->redir_in);
	free(cmd->redir_out);
	free(cmd->redir_out2);
	cmd->args = NULL;
	cmd->redir_in = NULL;
	cmd->redir_out = NULL;
	cmd->redir_out2 = NULL;
	to_free = cmd;
	if (cmd->nxt != NULL)
		cmd->nxt->prv = cmd->prv;
	if (cmd->prv == NULL)
	{
		*((*d)->cmds) = cmd->nxt;
		// printf("3a) del cmd\n");
	}
	else
	{
		cmd->prv->nxt = cmd->nxt;
		// printf("3b) del cmd\n");
	}
	free(to_free); // & ?
	to_free = NULL;
	// print_cmds("end free cmd", d);
}

void	del_cmds(t_data **d)
{
	t_cmds	*cmd;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		// *((*d)->cmds) = (*((*d)->cmds))->nxt;
		del_cmd_from_list(cmd, d);
		cmd = cmd->nxt;
	}
	print_cmds("end free cmds", d);
}