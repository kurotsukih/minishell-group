#include "headers.h"

static int	there_are_unclosed_quotes(t_cmds *cmd)
{
	int		mod;
	int		i;

	mod_(REINIT_QUOTES);
	i = -1;
	while (cmd->args[0][++i] != '\0')
		mod = mod_(cmd->args[0][i]);
	return (mod != QUOTES0);
}

int	args_are_correct(t_cmds *cmd, t_data **d)
{
	(void)d; ///
	if (!cmd->args)
	{
		//free_all_and_go_to_next_cmd("Too many arguments" , exit_code = 255)
		return (0);
	}
	if (cmd->nb_args > cmd->nb_max_args)
	{
		//free_all_and_go_to_next_cmd("Too many arguments" , exit_code )
		return (0);
	}
	if (ft_strcmp(cmd->args[0], "exit") == 0 && !ft_atoi(cmd->args[1])) ////
	{
		//free_all_and_go_to_next_cmd("exit: numeric argument required" , exit_code = 2)
		return (0);
	}
	if (there_are_unclosed_quotes(cmd))
	{
		//free_all_and_go_to_next_cmd("unclosed quotes" , exit_code )
		return (0);
	}
	return (1);
}

static char	*path2_(char *s1, int len_s1, t_cmds *cmd, t_data **d)
{
	int		i;
	int		j;
	char	*dest;

	dest = (char *) malloc_(len_s1 + ft_strlen(cmd->args[0]) + 1, d);
	i = 0;
	while (i < len_s1)
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i++] = '/';
	j = 0;
	while (cmd->args[0][j] != '\0')
	{
		dest[i] = cmd->args[0][j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

static char	*path_(t_cmds *cmd, t_data **d)
{
	char	*paths;
	char	*path;
	int		i;
	int		i_beg;

	paths = get_value_from_env("PATH", d);
	if (!paths)
	{
		//free_all_and_go_to_next_cmd("no var env PATH" , exit_code = 127)
		return (NULL);
	}
	i = -1;
	i_beg = 0;
	while (++i < (int)ft_strlen(paths))
		if (paths[i] == '\0' || paths[i] == ':')
		{
			path = path2_(&(paths[i_beg]), i - i_beg, cmd, d);
			if (access(path, X_OK) == 0)
				return (path);
			if (errno != 2)
				(*d)->exit_c = 126;
			i_beg = i + 1;
		}
	//free_all_and_go_to_next_cmd("command not found" , exit_code = 127)
	return (NULL);
}

// char *args[] = { "/bin/sh", "-c", "echo hello! > /path/to/file", NULL };
// execv("/bin/sh", args);

// execve creates a new process with the same open file descriptors as the parent
// (there are exceptions and naunces which can be read from the execve man page))
void exec_extern_cmd(t_cmds *cmd, t_data **d)
{
	char	*path;
	int		pid;
	int		status;
	char	**env_array;

	pid = fork();
	if (pid < -1)
		free_all_and_exit("fork error", d);
	if (pid == 0)
	{
		(*d)->exit_c = 0;
		path = path_(cmd, d);
		env_array = env_to_array(d);
		if (path != NULL && env_array != NULL)
			execve(path, cmd->args, env_array);
		free(path);
		free_charchar(env_array, len_env(d));
	}
	else
		wait(&status);
}

// >out1 < src1 cat >>out2 >out3 <src2 <<eof -e >out3 < src3

// les redir se font ds lordre, 
// donc pour ce qui est des redir out ca cree un fichier out1 car la premiere redir out c'est > out1
// puis finalement ca créer out2 car c'est la 2eme redir out parsée dans la cmd puis out3
// out1 et 2 sont vide vu qu'ils ont été créés puis finalement la ligne de cmd indique un autre fichier d'output
// mm logique pr les redirs in, 
// à la premiere erreur (le droit decriture pour les redir out, etc) ca fait tout fail

// tu peux voir le parsing d'une commande comme un tri entre ce qui es redir in, redir out
// et le reste c'est la commande en elle meme, 
// dans cet exemple une fois trié toute les redir out (les > et ce qui les suit genre > out1)
// et tte les redir in, il te reste cat -e
// les redir peuvent etre située n'importe ou par rapport a la commande et ses eventuels arguments

// heredoc = juste une redir in, 
// mais au lieu de rediriger le contenu d'un fichier ds le stdin ca redirige un input 
// (jusqua atteindre le delimiteur que tu defini dans ton heredoc)

// exemple d'un heredoc 
/*
<<lol bash
echo ahah
exit 12
> lol
*/
// ahah
// echo $?
// 12
// je demande a lancer bash en redirigeant un heredoc dans le stdin,
// le delimiteur est lol
// le fichier qu'il represente contiendra tout ce que je rentre en input jusqu'a une ligne = "lol"
// echo ahah suivit de exit 12 donc la cmd lance bash,
// puis dans ce bash execute echo ahah puis exit 12,
// on reviens au shell d'origine et si on echo le retour de la derniere commande (ici bash) c'est bien 12

// (le fichier qui répresent le heredoc peut etre implementé en tant que fichier 
// temporaire qui se delete a la fin de la cmd) 

void	exec_cmds(t_data **d)
{
	t_cmds	*cmd;
	t_cmds	*nxt;
	int		saved_stdout;

	cmd = *((*d)->cmds);
	while (cmd != NULL)
	{
		if (!args_are_correct(cmd, d))
			continue; //?
		// if (cmd->nxt != NULL && ft_strcmp(cmd->redirect, ">") == 0)
		// {
		// 	cmd->fd_out = open(cmd->nxt->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		// 	if(!(cmd->fd_out))
		// 		{}
		// 	saved_stdout = dup(STDOUT_FILENO); // создать дополнительный дескриптор для stdout
		// 	if (dup2(cmd->fd_out, STDOUT_FILENO) == -1) // stdout в файл, дублированием дескриптора 
		// 		{}
		// 	close(cmd->fd_out); // закрыть файл 
		// }
		if (ft_strcmp(cmd->args[0], "echo") == 0)
			exec_echo(cmd);
		else if (ft_strcmp(cmd->args[0], "env") == 0 && (*d)->env != NULL)
			exec_env(d);
		else if (ft_strcmp(cmd->args[0], "pwd") == 0)
			exec_pwd();
		else if (ft_strcmp(cmd->args[0], "cd") == 0)
			exec_cd(cmd, d);
		else if (ft_strcmp(cmd->args[0], "export") == 0)
			exec_export(cmd, d);
		else if (ft_strcmp(cmd->args[0], "unset") == 0 && (*d)->env != NULL)
			exec_unset(cmd, d);
		else if (ft_strcmp(cmd->args[0], "exit") == 0)
			exec_exit(d);
		else
			exec_extern_cmd(cmd, d);
		// if (cmd->nxt != NULL && ft_strcmp(cmd->redirect, ">") == 0)
		// {
		// 	dup2(saved_stdout, STDOUT_FILENO); // восстановить исходный stdout
		// 	close(saved_stdout);
		// }
		nxt = cmd->nxt;
		cmd = nxt;
		(void)saved_stdout;
	}
}
