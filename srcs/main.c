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

t_list	*add_token(char *cdm_with_spaces, int i_beg, int i_end, t_data *d)
{
	char	*new_str;
	t_list	*n;
	int		i;

	if (i_beg == i_end)
		i_end++;
	new_str = (char *)malloc((i_end - i_beg + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < i_end - i_beg)
	{
		new_str[i] = cdm_with_spaces[i + i_beg];
		i++;
	}
	new_str[i] = '\0';
	if (ft_strchr(new_str, '$'))
		n = expand_token(new_str, d->env, d);
	else
		n = ft_lstnew(new_str, 0);
	if (!n)
		return (NULL);
	return (n);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	char	*cmd;

	(void)argc;
	(void)argv;
	cmd = NULL;
	init(env, &data);
	while (1)
	{
		cmd = readline("$");
		if (cmd == NULL) // EOF
			break ;
		if (g_signal == 1)
		{
			g_signal = 0;
			data.exit_code = 130;
			continue;
		}
		add_history(cmd);
		data.exit_code = parse(cmd, data.env, &data);
		if (data.exit_code == 0)
			ft_execution(&data);
		ft_clean_tree(data.n);
	}
	return (free_redirections(data.env), data.exit_code);
}
