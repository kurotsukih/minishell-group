#include "headers.h"

// char *args[] = { "/bin/sh", "-c", "echo hello! > /path/to/file", NULL };
// execv("/bin/sh", args);

// execve creates a new process with the same ope n file descriptors as the parent
// (there are exceptions and naunces which can be read from the execve man page))

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

void	exec_exit(t_data **d)
{
	//(*d)->exit_c = 
	free_all_and_exit("", -1, d);
}


void	exec_echo(t_cmd *cmd)
{
	int	option_n;
	int	i;

	option_n = 0;
	i = 0;
	while (++i < cmd->nb_args)
	{
		if (ft_strcmp(cmd->arg[i], "-n") == 0)
			option_n = 1;
		else if (i == cmd->nb_args - 1)
			printf("%s", cmd->arg[i]);
		else
			printf("%s ", cmd->arg[i]);
	}
	if (option_n == 0)
		printf("\n");
}

void exec_extern_cmd(t_cmd *cmd, t_data **d)
{
	char	*path;
	int		pid;
	int		status;
	char	**env_array;

	pid = fork();
	if (pid < -1)
		free_all_and_exit("fork error", -1, d); //
	if (pid == 0)
	{
		(*d)->exit_c = 0;
		path = path_(cmd, d);
		env_array = env_to_array(d);
		printf("execve %s %s\n", path, cmd->arg[0]);
		if (path != NULL && env_array != NULL)
			execve(path, cmd->arg, env_array);
		free(path);
		free_array_env(env_array, len_env(d));
	}
	else
		wait(&status);
}

void	exec_cmds(t_data **d)
{
	t_cmd	*cmd;

	cmd = *((*d)->cmds);
	while (cmd != NULL)
	{
		(*d)->curr_cmd = cmd; // not used ?
		// if (cmd->fd_in != STDIN_FILENO)
		// {
		// 	if (dup2(cmd->fd_out, STDOUT_FILENO) == -1) // дубл. дескриптора => stdout в файл
		// 		cmd->err = "dup2 failed";
		// 	close(cmd->fd_out);
		// }
		if (ft_strlen(cmd->err) > 0)
			continue ;
		if (ft_strcmp(cmd->arg[0], "echo") == 0)
			exec_echo(cmd);
		else if (ft_strcmp(cmd->arg[0], "env") == 0 && (*d)->env != NULL)
			exec_env(d);
		else if (ft_strcmp(cmd->arg[0], "pwd") == 0)
			exec_pwd(d);
		else if (ft_strcmp(cmd->arg[0], "cd") == 0)
			exec_cd(cmd, d);
		else if (ft_strcmp(cmd->arg[0], "export") == 0)
			exec_export(cmd, d);
		else if (ft_strcmp(cmd->arg[0], "unset") == 0 && (*d)->env != NULL)
			exec_unset(cmd, d);
		else if (ft_strcmp(cmd->arg[0], "exit") == 0)
			exec_exit(d);
		else
			exec_extern_cmd(cmd, d);
		// if (cmd->fd_in == STDIN_FILENO)
		// 	if (dup2((*d)->saved_stdout, STDOUT_FILENO) == -1) // восстановить исходный stdout
		// 		cmd->err = "dup2 failed"; // exit code ?
		if (ft_strlen(cmd->err) > 0)
			printf("eror %s", cmd->err); // exit code ?
		cmd = cmd->nxt;
	}
}
