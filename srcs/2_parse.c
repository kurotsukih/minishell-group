#include "headers.h"

static void	put_full_cmd_to_arg0_1(char *full_cmd, int len, t_data **d)
{
	t_cmds	*new;
	t_cmds	*last;
	int		i;

	// printf("put_full_cmd_to_arg0_1 %s %d\n", full_cmd, len);
	init_cmd(&new, d);
	new->nb_args = nb_args_(full_cmd, len);
	new->args = (char **)malloc_((new->nb_args + 1)* sizeof(char *), d);
	new->args[0] = (char *)malloc_(len + 1, d);
	i = 0;
	while (++i < new->nb_args + 1)
		new->args[i] = NULL;
	i = -1;
	while (++i < len)
		new->args[0][i] = full_cmd[i];
	new->args[0][i] = '\0';
	if (*((*d)->cmds) == NULL)
		*((*d)->cmds) = new;
	else
	{
		last = *((*d)->cmds);
		while (last != NULL && last->nxt != NULL)
			last = last->nxt;
		last->nxt = new;
		new->prv = last;
	}
}

void	put_full_cmd_to_arg0(char *cmd_line, t_data **d)
{
	int		i_beg;
	int 	i;

	mod_(REINIT_QUOTES);
	i_beg = 0;
	i = -1;
	while (1)
		if ((mod_(cmd_line[++i]) == QUOTES0 && cmd_line[i + 1] == '|') || cmd_line[i + 1] == '\0')
		{
			put_full_cmd_to_arg0_1(&cmd_line[i_beg], i - i_beg + 1, d);
			if (cmd_line[i + 1] == '\0') // i + 3 ??
				break;
			i ++;
			i_beg = i + 1;
		}
}

// static void	put_cmd_and_redirect(char *cmd_txt, int len, char *redirect, t_data **d)
// {
// 	t_cmds	*new;
// 	t_cmds	*last;
// 	int		i;

// 	init_cmd(&new, redirect, d);
// 	new->nb_args = nb_args_(cmd_txt, len);
// 	new->args = (char **)malloc_((new->nb_args + 1)* sizeof(char *), d);
// 	new->args[0] = (char *)malloc_(len + 1, d);
// 	i = 0;
// 	while (++i < new->nb_args + 1)
// 		new->args[i] = NULL;
// 	i = -1;
// 	while (++i < len)
// 		new->args[0][i] = cmd_txt[i];
// 	new->args[0][i] = '\0';
// 	if (*((*d)->cmds) == NULL)
// 		*((*d)->cmds) = new;
// 	else
// 	{
// 		last = *((*d)->cmds);
// 		while (last != NULL && last->nxt != NULL)
// 			last = last->nxt;
// 		last->nxt = new;
// 		new->prv = last;
// 	}
// }

// void	put_cmds_and_redirects(char *cmd_line, t_data **d)
// {
// 	int		i_beg;
// 	int 	i;
// 	char	*redirect;

// 	i_beg = 0;
// 	i = 0;
// 	mod_(REINIT_QUOTES);
// 	while (1)
// 	{
// 		redirect = redirect_(&cmd_line[i + 1]);
// 		if ((mod_(cmd_line[i]) == QUOTES0 && ft_strlen(redirect) > 0) || cmd_line[i + 1] == '\0')
// 		{
// 			put_cmd_and_redirect(&cmd_line[i_beg], i - i_beg + 1, redirect, d);
// 			if (cmd_line[i + ft_strlen(redirect) + 1] == '\0')
// 				break;
// 			i += ft_strlen(redirect);
// 			i_beg = i + 1;
// 		}
// 		i++;
// 	}
// }

void	put_redirs(t_data **d)
{
	t_cmds	*cmd;
	char	*s;
	int		i;
	int		i_beg;
	char* 	redir;
	int 	len;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		mod_(REINIT_QUOTES);
		s = cmd->args[0];
		i = -1;
		while (s[++i] != '\0')
		{
			redir = redir_(&s[i]);
			if (mod_(s[i]) == QUOTES0 && ft_strlen(redir) > 0)
			{
				i_beg = i;
				i += ft_strlen(redir);
				while(s[i] == ' ')
					i++;
				while(s[i] != ' ' && s[i] != '>' && s[i] != '<' && s[i] != '\0')
					i++;
				len = i - i_beg - ft_strlen(redir);
				if (i == 0)
					continue;
				if (ft_strcmp(redir, ">") == 0)
					strdup_and_trim(&s[i_beg + ft_strlen(redir)], &(cmd->redir_out), len, d);
				else if (ft_strcmp(redir, ">>") == 0)
					strdup_and_trim(&s[i_beg + ft_strlen(redir)], &(cmd->redir_out2), len, d);
				else if (ft_strcmp(redir, "<") == 0)
					strdup_and_trim(&s[i_beg + ft_strlen(redir)], &(cmd->redir_in), len, d);
				i = i_beg;
				while(i < i_beg + len)
					s[i++] = ' ';
			}
		}
		cmd = cmd->nxt;
	}
}

static void	put_args_1(t_cmds *cmd, t_data **d) 
{
	int		i;
	int		i_beg;
	int		k;
	int		len;
	int		to_put_EOL;

	if (cmd->nb_args <= 1)
		return ;
	mod_(REINIT_QUOTES);
	i_beg = 0;
	k = 0;
	len = (int)ft_strlen(cmd->args[0]);
	i = -1;
	while (++i < len)
		if (mod_(cmd->args[0][i]) == QUOTES0 && cmd->args[0][i] != ' ' && (cmd->args[0][i + 1] == ' ' || cmd->args[0][i + 1] == '\0' || cmd->args[0][i + 1] == '\'' || cmd->args[0][i + 1] == '\"'))
		{
			if (k == 0)
				to_put_EOL = i + 1;
			else
				strdup_and_trim(&(cmd->args[0][i_beg]), &(cmd->args[k]), i - i_beg + 1, d);
			i_beg = i + 1;
			k++;
		}
	cmd->args[0][to_put_EOL] = '\0';
}

void	put_args(t_data **d)
{
	t_cmds	*cmd;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		put_args_1(cmd, d);
		cmd->args[cmd->nb_args] = NULL;
		if (ft_strcmp(cmd->args[0], "env") == 0)
			cmd->nb_max_args = 0;
		else if (ft_strcmp(cmd->args[0], "cd") == 0 || ft_strcmp(cmd->args[0], "exit") == 0)
			cmd->nb_max_args = 1;
		cmd = cmd->nxt;
	}
}

