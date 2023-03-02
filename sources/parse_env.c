/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 15:06:30 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/02 17:26:43 by bgresse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_list_size(t_env *head);

void	ft_set_env(t_env **head, char *key, char *value, bool equal)
{
	t_env	*temp;

	temp = *head;
	while (temp)
	{
		if (!ft_strcmp(temp->key, key))
		{
			free(temp->value);
			temp->value = ft_strdup(value);
			return ;
		}
		temp = temp->next;
	}
	add_node(*head, key, value, equal);
}

char	**ft_join_envp(t_env **head)
{
	t_env	*current;
	char	**envp;
	size_t	i;

	i = 0;
	current = *head;
	envp = malloc(sizeof(char *) * ft_list_size(*head));
	while (current)
	{
		envp[i++] = ft_strjoin(current->key, ft_strjoin("=", current->value));
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	ft_parse_env(t_env **head, char **envp)
{
	size_t	i;
	char	*equal;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			key = ft_strndup(envp[i], equal - envp[i]);
			value = ft_strdup(equal + 1);
			*head = add_node(*head, key, value, true);
			free(key);
		}
		i++;
	}
}

char	*ft_get_env(t_env **head_env, char *key)
{
	t_env	*temp;

	temp = *head_env;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

char	**ft_pathfinder(t_env **head)
{
	char	*paths_line;
	char	**paths;

	paths_line = ft_get_env(head, "PATH");
	paths = ft_split_slash(paths_line, ':');
	return (paths);
}
