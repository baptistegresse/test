/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 16:02:59 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/06 19:42:22 by bgresse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int g_status;

void	data_init(int argc, char **argv, char **envp, t_minishell *data)
{
	data->argc = argc;
	data->argv = argv;
	data->envp = envp;
	ft_parse_env(&data->head_env, data->envp);
	data->paths = ft_pathfinder(&data->head_env);
	g_status = 0;
}

// -----------------------------------------------------------------------------------

char	**ft_refresh_envp(t_env *head)
{
	size_t	env_len;
	char	**env;
	size_t	i;

	if (!head)
		return (NULL);
	env_len = ft_list_size(head);
	env = malloc(sizeof(char *) * env_len);
	i = 0;
	while (head)
	{
		if (head->equal)
			env[i] = ft_strjoin(head->key, ft_strjoin("=", head->value));
		else
			env[i] = head->key;
		i++;
		head = head->next;
	}
	env[i] = NULL;
	return (env);
}

// -----------------------------------------------------------------------------------

static char	*get_prompt(t_env **head)
{
	char	*user;
	char	*shlv;
	int		len_shlvl;

	user = ft_get_env(head, "USER");
	shlv = ft_get_env(head, "SHLVL");

	len_shlvl = ft_atoi(shlv);
	shlv = malloc(sizeof(char) * (len_shlvl + 1));
	shlv[len_shlvl] = 0;
	while (len_shlvl--)
		shlv[len_shlvl] = '.';
	if (!user)
		user = "guest";
	
	return (ft_strjoin(ft_strjoin(""GREEN"??? ", shlv), ft_strjoin(
		""CYAN"", ft_strjoin(user, ""PURPLE"@minishell"YELLOW" /??? - ?? -??? ??? "RESET""))));
}

void	ft_special_builtins(t_minishell *data);

void	ft_prompt(t_minishell *data)
{
	char		*buffer;
	int			pid;
	char		*prompt;

	while (1)
	{
		data->envp = ft_refresh_envp(data->head_env);
		prompt = get_prompt(&data->head_env);
		buffer = readline(prompt);
		if (!buffer)
			break ;
		if (!*buffer)
			continue;
		add_history(buffer);
		data->cmds = ft_cmdlist(buffer, data);
		if (data->cmds)
		{
			ft_special_builtins(data);
			pid = fork();
			if (pid == 0)
				pipex(data);
			waitpid(pid, &g_status, 0);
			g_status = WEXITSTATUS(g_status);
		}
	}
	printf("exit\n");
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*data;

	data = malloc(sizeof(t_minishell));
	data_init(argc, argv, envp, data);
	ft_prompt(data);
	return (g_status);
}
