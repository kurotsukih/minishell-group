#include "headers.h"

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
		if (strcmp(cmd->arg[i], "-n") == 0)
			option_n = 1;
		else if (i < cmd->nb_args - 1)
			printf("%s ", cmd->arg[i]);
		else
			printf("%s", cmd->arg[i]);
	}
	if (option_n == 0)
		printf("\n");
}