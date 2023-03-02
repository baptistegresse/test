/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 14:52:56 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/02 16:23:20 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_input(t_cmdlist *cmds)
{
	size_t	length;
	char	*line;

	if (close(cmds->here_doc_pipe[0]) == -1)
		return (perror("close pipe"));
	while (1)
	{
		line = readline("> ");
		if (!line)
			return ;
		if (ft_strlen(line) - 1 > ft_strlen(cmds->limiter))
			length = ft_strlen(line) - 1;
		else
			length = ft_strlen(cmds->limiter);
		if (!ft_strncmp(line, cmds->limiter, length))
		{
			if (close(cmds->here_doc_pipe[1]) == -1)
				return (perror("close pipe"));
			exit(EXIT_SUCCESS);
		}
		ft_putstr_fd(line, cmds->here_doc_pipe[1]);
		ft_putstr_fd("\n", cmds->here_doc_pipe[1]);
		free(line);
	}
}

void	ft_heredoc(t_cmdlist *cmds)
{
	pid_t	here_doc_pid;

	if (pipe(cmds->here_doc_pipe) == -1)
		return (perror("heredoc pipe"));
	here_doc_pid = fork();
	if (here_doc_pid == -1)
		return (perror("heredoc fork"));
	if (here_doc_pid != 0 && close(cmds->here_doc_pipe[1]) == -1)
		return ((void)perror("close pipe"));
	if (here_doc_pid == 0)
		ft_input(cmds);
	waitpid(here_doc_pid, NULL, 0);
}

void	ft_redirect_heredoc(char **parsed_line, t_cmdlist *cmds, int i)
{
	if (!ft_error(parsed_line, i))
	{
		cmds->here_doc = 1;
		cmds->limiter = parsed_line[i + 1];
		ft_heredoc(cmds);
	}
}

void	ft_redirect_pipe(t_cmdlist **cmds);

void	ft_check_heredoc(char **parsed_line, t_cmdlist *cmds)
{
	int	i;

	i = -1;
	while (parsed_line[++i])
	{
		if (!ft_strcmp(parsed_line[i], "|"))
			ft_redirect_pipe(&cmds);
		else if (!ft_strcmp(parsed_line[i], "<<"))
			ft_redirect_heredoc(parsed_line, cmds, i);
	}
}
