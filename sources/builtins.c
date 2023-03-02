/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <zrebhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 16:24:08 by bgresse           #+#    #+#             */
/*   Updated: 2023/03/02 16:31:25 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_set_env(t_env **head, char *key, char *value);

void	ft_built_in_cd(t_env **head, char **full_cmd)
{
	char	cwd[PATH_MAX];

	if (chdir(full_cmd[1]) == -1)
		perror("chdir()");
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if (ft_get_env(head, "PWD"))
			ft_set_env(head, "OLDPWD", ft_get_env(head, "PWD"));
		ft_set_env(head, "PWD", cwd);
	}
	else
		perror("getcwd()");
}

void	ft_built_in_exit(t_minishell *data)
{
	int	i;
	int	exit_code;

	i = -1;
	printf("exit\n");
	if (!data->cmds->full_cmd[1]|| !*data->cmds->full_cmd[1])
		exit(0);
	while (data->cmds->full_cmd[1][++i])
	{
		if (data->cmds->full_cmd[1][i] < '0' || data->cmds->full_cmd[1][i] > '9')
		{
			ft_putstr_fd("exit: ", 2);
			ft_putstr_fd(data->cmds->full_cmd[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit(2);
		}
	}
	if (data->cmds->full_cmd[2])
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(": too many arguments\n", 2);
		data->cmds = 0;
	}
	else
	{
		exit_code = ft_atoi(data->cmds->full_cmd[1]);
		exit(exit_code);
	}
}

void	ft_built_in_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("getcwd()");
}

int	ft_builtins(t_minishell *data)
{
	if (!ft_strcmp(data->cmds->full_cmd[0], "pwd"))
		return (ft_built_in_pwd(), 1);
	if (!ft_strcmp(data->cmds->full_cmd[0], "env") && \
	(!data->cmds->full_cmd[1] || !*data->cmds->full_cmd[1]))
		return (ft_built_in_env(&data->head_env), 1);
	if (!ft_strcmp(data->cmds->full_cmd[0], "export"))
		return (ft_built_in_export(&data->head_env, data->cmds->full_cmd), 1);
	if (!ft_strcmp(data->cmds->full_cmd[0], "cd"))
		return (1);
	if (!ft_strcmp(data->cmds->full_cmd[0], "unset"))
		return (1);
	return (0);
}

void	ft_special_builtins(t_minishell *data)
{
	if (!data->cmds->next && data->cmds->full_cmd[0])
	{
		if (!ft_strcmp(data->cmds->full_cmd[0], "cd"))
			return (ft_built_in_cd(&data->head_env, data->cmds->full_cmd));
		if (!ft_strcmp(data->cmds->full_cmd[0], "exit"))
			return (ft_built_in_exit(data));
		if (!ft_strcmp(data->cmds->full_cmd[0], "export") \
			&& data->cmds->full_cmd[1])
			return (ft_built_in_export(&data->head_env, \
			data->cmds->full_cmd));
		if (!ft_strcmp(data->cmds->full_cmd[0], "unset"))
			return (ft_built_in_unset(&data->head_env, \
			data->cmds->full_cmd[1]));
	}
}
