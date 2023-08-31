#include "headers.h"

// pwd a b >out1 >out1+ >> out2 < in >> out2+ < in+ <in++
static void	put_full_cmd_to_arg0_1(char *full_cmd, int len, t_data **d)
{
	t_cmds	*new;
	t_cmds	*last;
	int		i;

	init_cmd(&new, d);
	new->nb_args = nb_args_(full_cmd, len, d);
	// printf("%s : %d args\n", full_cmd, new->nb_args);
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

static void	put_args_1(t_cmds *cmd, t_data **d) 
{
	int		i;
	int		i_beg;
	int		k;
	int		len;
	int		here_put_EOL;
	char	*s;

	if (cmd->nb_args <= 1)
		return ;
	mod_(REINIT_QUOTES);
	i_beg = 0;
	k = 0;
	len = (int)ft_strlen(cmd->args[0]); // delete len
	i = -1;
	s = strdup_and_erase_redirs(cmd->args[0], len, d);
	while (++i < len)
		if (mod_(s[i]) == QUOTES0 && s[i] != ' ' && (s[i + 1] == ' ' || s[i + 1] == '\0' || s[i + 1] == '\'' || s[i + 1] == '\"'))
		{
			if (k == 0)
				here_put_EOL = i + 1;
			else
				cmd->args[k] = strndup_and_trim(&s[i_beg], i - i_beg + 1, d);
			i_beg = i + 1;
			k++;
		}
	cmd->args[0][here_put_EOL] = '\0';
	free(s);
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


void	put_redirs_1(t_cmds *cmd, t_data **d)
{
	char	*s;
	int		i;
	int		i_beg;
	char	*redir;
	char	*redir_file;
	int		len;

	mod_(REINIT_QUOTES);
	s = strdup_and_erase_args_except_redirs(cmd->args[0], d);
	i = -1;
	while (s[++i] != '\0')
	{
		redir = redir_(&s[i]);
		if (mod_(s[i]) == QUOTES0 && ft_strlen(redir) > 0)
		{
			i += ft_strlen(redir);
			i_beg = i;
			while(s[i] == ' ')
				i++;
			while(s[i] != ' ' && s[i] != '>' && s[i] != '<' && s[i] != '\0')
				i++;
			len = i - i_beg;
			redir_file = strndup_and_trim(&s[i_beg], len, d);
			if (ft_strlen(redir_file) == 0)
				{
					// 'bash: erreur de syntaxe près du symbole inattendu « > »"
					continue; 
				}
			if (ft_strcmp(redir, ">") == 0)
				cmd->redir_out = redir_file;
			else if (ft_strcmp(redir, ">>") == 0)
				cmd->redir_out2 = redir_file;
			else if (ft_strcmp(redir, "<") == 0)
				cmd->redir_in = redir_file;
			// i = i_beg;
			// while(i < i_beg + len)
			// 	s[i++] = ' ';
		}
	}
}

void	put_redirs(t_data **d)
{
	t_cmds	*cmd;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		put_redirs_1(cmd, d);
		cmd = cmd->nxt;
	}
}

// void	put_redirs(t_data **d)
// {
// 	t_cmds	*cmd;
// 	char	*s;
// 	int		i;
// 	int		i_beg;
// 	char* 	redir;
// 	int 	len;

// 	cmd = *((*d)->cmds);
// 	while(cmd != NULL)
// 	{
// 		mod_(REINIT_QUOTES);
// 		s = cmd->args[0];
// 		i = -1;
// 		while (s[++i] != '\0')
// 		{
// 			redir = redir_(&s[i]);
// 			if (mod_(s[i]) == QUOTES0 && ft_strlen(redir) > 0)
// 			{
// 				i_beg = i;
// 				i += ft_strlen(redir);
// 				while(s[i] == ' ')
// 					i++;
// 				while(s[i] != ' ' && s[i] != '>' && s[i] != '<' && s[i] != '\0')
// 					i++;
// 				len = i - i_beg - ft_strlen(redir);
// 				if (i == 0)
// 					continue;
// 				if (ft_strcmp(redir, ">") == 0)
// 					strdup_and_trim(&s[i_beg + ft_strlen(redir)], &(cmd->redir_out), len, d);
// 				else if (ft_strcmp(redir, ">>") == 0)
// 					strdup_and_trim(&s[i_beg + ft_strlen(redir)], &(cmd->redir_out2), len, d);
// 				else if (ft_strcmp(redir, "<") == 0)
// 					strdup_and_trim(&s[i_beg + ft_strlen(redir)], &(cmd->redir_in), len, d);
// 				i = i_beg;
// 				while(i < i_beg + len)
// 					s[i++] = ' ';
// 			}
// 		}
// 		cmd = cmd->nxt;
// 	}
// }
