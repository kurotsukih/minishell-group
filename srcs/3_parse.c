#include "headers.h"

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