#include "headers.h"

void	sig_handler_main(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = 1;
	}
}

void	sig_handler_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_redisplay();
		close(STDIN_FILENO);
		g_signal = 1;
	}
}

void	sig_handler_fork(int signal)
{
	if (signal == SIGINT)
		exit(130);
	if (signal == SIGQUIT)
		exit(131);
}

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
		((unsigned char *)b)[i++] = c;
	return (b);
}

void	exit_(int exit_code, char *msg, char *msg_param, t_list **lst_to_clear, char **str_to_free)
{
	if (msg == NULL)
		printf("%s\n", strerror(errno));
	else if (msg != NULL && msg_param == NULL)
		printf("bash: %s\n", msg);
	else if (msg != NULL && msg_param != NULL)
		printf(msg, msg_param);
	if (lst_to_clear != NULL)
	// ft_lstclear(lst_to_clear);
	if (str_to_free != NULL)
		free(*str_to_free);
	if (exit_code != -1)
		exit(exit_code);
}

char *alphanum_(char *s)
{
	int		i;
	char	*alphanum;

	if ((s[0] < 'a' || s[0] > 'z') && (s[0] < 'A' && s[0] > 'Z'))
		return (NULL);
	i = -1;
	while ((s[++i] >= '0' && s[i] < '9') || (s[i] >= 'a' && s[i] < 'z') || (s[i] >= 'A' && s[i] < 'Z') || s[i] == '_')
		;
	alphanum = NULL;
	alphanum = (char *)malloc(i + 1);
	if (alphanum == NULL)
		return (NULL);
	i = -1;
	while ((s[++i] >= '0' && s[i] < '9') || (s[i] >= 'a' && s[i] < 'z') || (s[i] >= 'A' && s[i] < 'Z') || s[i] == '_')
		alphanum[i] = s[i];
	alphanum[i] = '\0';
	return (alphanum);
}

int strdup_and_trim(char *src, char **dest0, int len)
{
	int		i;
	char	*dest;

	while (src[0] == ' ' && src[0] != '\0')
	{
		src++;
		len--;
	}
	i = len;
	while (src[--i] == ' ' && i >= 0)
	{
		src[i] = '\0';
		len--;
	}
	dest = NULL;
	dest = (char *)malloc(len + 1);
	if (dest == NULL)
		return (-1);
	i = -1;
	while (++i < len)
		dest[i] = src[i];
	dest[i] = '\0';
	*dest0 = dest;
	return (0);
}

/// double quotes insdide simple ones ?
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

char	*redirect_(char *s)
{
	if (s[0] == '>' && s[1] == '>')
		return (">>");
	else if (s[0] == '<' && s[1] == '<')
		return ("<<");
	else if (s[1] == '>' && s[1] != '\0' && s[2] != '>')
		return (">");
	else if (s[1] == '<' && s[1] != '\0' && s[2] != '<')
		return ("<");
	else if (s[1] == '|')
		return ("|");
	else
		return ("");
}

//if $a="s -la" then l$a -> ls -la ???
// static int	is_correct_token_(char *s, int len)
// {
// 	int	i;

// 	if (s[0] == '\'' && s[len - 1] == '\'')
// 	{
// 		i = 0;
// 		while (++i < len - 1)
// 			if (s[i] == '\'' || s[i] == '\"')
// 				return (0);
// 		return (1);
// 	}
// 	if (s[0] == '\"' && s[len - 1] == '\"')
// 	{
// 		i = 0;
// 		while (++i < len - 1)
// 			if (s[i] == '\'' || s[i] == '\"' || len_dollar_convers(&s[i], len - i + 1) == -1)
// 				return (0);
// 		return (1);
// 	}
// 	i = -1;
// 	while (++i < len)
// 		if (s[i] == '\'' || s[i] == '\"' || len_dollar_convers(&s[i], len - i + 1) == -1 || (i < len - 1 && s[i] != '$' && s[i + 1] == '?'))
// 			return (0);
// 	return (1);
// }
