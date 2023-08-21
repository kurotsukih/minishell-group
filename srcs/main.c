/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akalimol <akalimol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 17:31:49 by akalimol          #+#    #+#             */
/*   Updated: 2023/07/08 19:32:59 by akalimol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
после "cat файл", "cat файл | cat -e" : free(): double free detected in tcache 2

readline rl_clear_history, rl_on_new_line rl_replace_line rl_redisplay, add_history,
malloc free
printf write access open read close
fork wait waitpid wait3 wait4 
signa l sigaction sigemptyset sigaddset kill
exit
getcwd chdir stat lstat fstat unlink execve dup dup2 pipe
opendir readdir closedir
strerror perror
isatty ttyname ttyslot ioctl
geten v
tcsetattr tcgetattr tgetent tgetflag tgetnum tgetstr tgoto tputs

SIGIN T = the user types C-c
SIGQUI T = SIGIN T, except that it’s controlled by C-\ + produces a core dump when it terminates the process, 
CTRL-\ causes a program to terminate and dump core

The line is allocated with malloc, the caller must free it.
Returns the line without \n, "" for "", NULL for "" + EOF
If an EOF is read with a non-empty line, it is treated as a newline.

linked list:
list->content	- full string (ex. "USER=akostrik")
list->typ e      - default or not */

#include "minishell.h"

int g_signal = 0;

void	init(char **env, t_data *d)
{
	t_list	*t;
	char	*str;
	int		i;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sig_handler_main);
	d->env = NULL;
	i = 0;
	while (env[i])
	{
		str = ft_strdup(env[i]);
		if (!str)
		{
			exit_(-1, NULL, NULL, &(d->env), &free, NULL);
			return ;
		}
		t = ft_lstnew(str, 0);
		if (!t)
		{
			exit_(-1, NULL, NULL, &t, &free, &str);
			return ;
		}
		ft_lstadd_back(&d->env, t);
		i++;
	}
	d->exit_code = 0;
}

int	main(int argc, char **argv, char **env)
{
	t_data	d;
	char	*cmd;

	(void)argc;
	(void)argv;
	cmd = NULL;
	init(env, &d);
	while (1)
	{
		cmd = readline("$");
		if (cmd == NULL) // EOF
			break ;
		if (g_signal == 1)
		{
			g_signal = 0;
			d.exit_code = 130;
			continue;
		}
		add_history(cmd);
		d.exit_code = parse(cmd, d.env, &d);
		if (d.exit_code == 0)
			ft_execution(&d);
		ft_clean_tree(d.n);
	}
	return (free_redirections(d.env), d.exit_code);
}
