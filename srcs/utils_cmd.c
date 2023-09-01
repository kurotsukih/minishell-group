#include "headers.h"

void	init_cmd(t_cmd **new, t_data **d)
{
	*new = (t_cmd *)malloc_(sizeof(t_cmd), d);
	(*new)->arg = NULL;
	(*new)->fd_in = STDIN_FILENO;
	(*new)->fd_out = STDOUT_FILENO;
	(*new)->path = NULL;
	(*new)->nxt = NULL;
	(*new)->prv = NULL;
	(*new)->err = NULL;
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

int	there_are_unclosed_quotes(t_cmd *cmd)
{
	int		mod;
	int		i;

	mod_(REINIT_QUOTES);
	i = -1;
	while (cmd->arg[0][++i] != '\0')
		mod = mod_(cmd->arg[0][i]);
	return (mod != QUOTES0);
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
	t_cmd	*cmd;

	printf("LIST %s %14p->%14p:\n  ", msg, (*d)->cmds, (*d)->cmds == NULL ? 0 : *((*d)->cmds));
	if ((*d)->cmds == NULL || *((*d)->cmds) == NULL)
	{
		printf("empty\n");
		return ;
	}
	cmd = *((*d)->cmds);
	while (cmd != NULL)
	{
		if (cmd->arg != NULL)
		{
			i = -1;
			while (++i < cmd->nb_args)
				printf("%s : ", cmd->arg[i]);
		}
		else
			printf("args = NULL");
		printf(" fd_in = %d, fd_out = %d", cmd->fd_in, cmd->fd_out);
		if (cmd->err != NULL)
			printf(", err = %s\n", cmd->err);
		printf("\n");
		cmd = cmd->nxt;
	}
	printf("\n");
}

void del_cmd_from_lst(t_cmd *cmd, t_data **d)
{
	int		i;
	t_cmd	*to_free;

	if (cmd == NULL)
		return ;
	i = -1;
	while(++i < cmd->nb_args)
	{
		free(cmd->arg[i]);
		cmd->arg[i] = NULL;
	}
	free(cmd->arg);
	cmd->arg = NULL;
	to_free = cmd;
	if (cmd->nxt != NULL)
		cmd->nxt->prv = cmd->prv;
	if (cmd->prv == NULL)
		*((*d)->cmds) = cmd->nxt;
	else
		cmd->prv->nxt = cmd->nxt;
	free(cmd->path);
	free(cmd->err);
	free(to_free); // &to_free ?
	(*d)->curr_cmd = NULL;
	to_free = NULL;
}

void	del_cmds(t_data **d)
{
	t_cmd	*cmd;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		del_cmd_from_lst(cmd, d);
		cmd = cmd->nxt;
	}
}

void	open_file(char *redir, char *redir_file, t_data **d)
{
	if (ft_strlen(redir_file) == 0)
	{
		//'bash: erreur de syntaxe près du symbole inattendu « > »"
		(*d)->curr_cmd->err = "erreur de syntaxe près du symbole inattendu « > »";
		return ;
	}
	if (ft_strcmp(redir, "<") == 0)
	{
		if ((*d)->curr_cmd->fd_in != STDIN_FILENO)
			close((*d)->curr_cmd->fd_in);
		(*d)->curr_cmd->fd_in = open(redir_file, O_RDONLY);
		if (!(*d)->curr_cmd->fd_in)
			(*d)->curr_cmd->err = "open file failed";
	}
	else if (ft_strcmp(redir, ">") == 0)
	{
		if ((*d)->curr_cmd->fd_out != STDIN_FILENO)
			close((*d)->curr_cmd->fd_out);
		(*d)->curr_cmd->fd_out = open(redir_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (!(*d)->curr_cmd->fd_out)
			(*d)->curr_cmd->err = "open file failed";
	}
	else if (ft_strcmp(redir, ">>") == 0)
	{
		if ((*d)->curr_cmd->fd_out != STDIN_FILENO)
			close((*d)->curr_cmd->fd_out);
		(*d)->curr_cmd->fd_out = open(redir_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (!(*d)->curr_cmd->fd_out)
			(*d)->curr_cmd->err = "open file failed";
	}
}

static char	*s_with_conversion_(char *old_s, int j, t_data **d)
{
	char	*new_s;
	int		len_new_s;
	char	*key;
	char	*val;
	int		k;

	key = alphanum_(&(old_s[j + 1]), d);
	val = get_value_from_env(key, d);
	len_new_s = ft_strlen(old_s) - ft_strlen(key) + ft_strlen(val);
	new_s = (char*)malloc_(len_new_s + 1, d);
	k = -1;
	while (++k < j)
		new_s[k] = old_s[k];
	k--; //
	while (++k < j + (int)ft_strlen(val))
		new_s[k] = val[k - j];
	k--;
	while(++k < len_new_s)
		new_s[k] = old_s[k + (int)ft_strlen(key) - (int)ft_strlen(val) + 1];
	new_s[k] = '\0';
	free(key);
	free(val);
	return (new_s);
}

void	calc_dollar_convers(t_cmd *cmd, t_data **d)
{
	char	*s_with_conversion;
	int		i;
	int		j;

	(*d)->curr_cmd = cmd;
	i = 0;
	while(++i < cmd->nb_args)
		if (cmd->arg[i][0] != '\'')
		{
			j = -1;
			while (cmd->arg[i][++j] != '\0' && cmd->arg[i][j + 1] != '\0')
			{
				// if (cmd->arg[i][j] == '$' && cmd->arg[i][j + 1] == '?')
				// 	(ft_itoa(exit_code));
				if (cmd->arg[i][j] == '$')
				{
					s_with_conversion = s_with_conversion_(cmd->arg[i], j, d);
					free(cmd->arg[i]);
					cmd->arg[i] = s_with_conversion;
				}
			}
		}
}

static void	verif_args_1(t_cmd *cmd)
{
	if (there_are_unclosed_quotes(cmd) && ft_strlen(cmd->err) == 0)
		cmd->err = "unclosed quotes"; // exit_code ?
	if (!cmd->arg && ft_strlen(cmd->err) == 0)
		cmd->err = "empty commande"; // exit_code = 255
	if (ft_strcmp(cmd->arg[0], "env") == 0 && cmd->nb_args > 1 && ft_strlen(cmd->err) == 0)
		cmd->err = "env : Too many arguments"; // exit_code ?
	if (ft_strcmp(cmd->arg[0], "cd") == 0 && cmd->nb_args > 2 && ft_strlen(cmd->err) == 0)
		cmd->err = "cd : Too many arguments"; // exit_code ?
	if (ft_strcmp(cmd->arg[0], "exit") == 0 && cmd->nb_args > 2 && ft_strlen(cmd->err) == 0)
		cmd->err = "exit : Too many arguments"; // exit_code ?
	if (ft_strcmp(cmd->arg[0], "exit") == 0 && !ft_atoi(cmd->arg[1]) && ft_strlen(cmd->err) == 0)
		cmd->err = "exit: numeric argument required"; // exit_code = 2
}

void	verif_args(t_data **d)
{
	t_cmd	*cmd;

	cmd = *((*d)->cmds);
	while(cmd != NULL)
	{
		// (*d)->curr_cmd = cmd; // not used 
		verif_args_1(cmd);
		cmd = cmd->nxt;
	}
}

int	is_builtin(t_cmd *cmd)
{
	return (\
	ft_strcmp(cmd->arg[0], "cd") == 0 || \
	ft_strcmp(cmd->arg[0], "exit") == 0 || \
	ft_strcmp(cmd->arg[0], "export") == 0 || \
	ft_strcmp(cmd->arg[0], "unset") == 0 || \
	ft_strcmp(cmd->arg[0], "echo") == 0 || \
	ft_strcmp(cmd->arg[0], "pwd") == 0 || \
	ft_strcmp(cmd->arg[0], "env") == 0);
}

char	*path_(char *s1, char *s2, t_data **d)
{
	int		i;
	int		s1_len;
	int		s2_len;
	char	*new_string;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new_string = (char *)malloc_(s1_len + s2_len + 2, d);
	if (new_string == NULL)
		return (NULL);
	i = -1;
	while (++i < s1_len)
		new_string[i] = s1[i];
	new_string[i] = '/';
	i = -1;
	while (++i < s2_len)
		new_string[s1_len + 1 + i] = s2[i];
	new_string[s1_len + 1 + i] = '\0';
	return (new_string);
}
