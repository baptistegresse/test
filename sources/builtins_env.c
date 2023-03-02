/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 12:37:40 by zrebhi            #+#    #+#             */
/*   Updated: 2023/03/02 17:38:03 by bgresse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_built_in_env(t_env **head)
{
	t_env	*temp;

	temp = *head;
	while (temp)
	{
		if (ft_strcmp(temp->value, ""))
			printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
}

void ft_print_env_sorted(t_env *env)
{
	// Copie de la liste chaînée
	t_env *copy = env;
	int count = 0;
	while (copy != NULL)
	{
		count++;
		copy = copy->next;
	}

	t_env **array = malloc(count * sizeof(t_env *));
	if (array == NULL)
	{
		printf("Erreur d'allocation de mémoire\n");
		return;
	}

	copy = env;
	for (int i = 0; i < count; i++)
	{
		array[i] = copy;
		copy = copy->next;
	}

	// Tri à bulles
	for (int i = 0; i < count - 1; i++)
	{
		for (int j = 0; j < count - i - 1; j++)
		{
			if (ft_strcmp(array[j]->key, array[j+1]->key) > 0)
			{
				t_env *temp = array[j];
				array[j] = array[j+1];
				array[j+1] = temp;
			}
		}
	}

	// Affichage des éléments triés
	for (int i = 0; i < count; i++)
	{
		printf("declare -x %s=\"%s\"\n", array[i]->key, array[i]->value);
	}

	// Libération de la mémoire allouée
	free(array);
}

void	ft_set_env(t_env **head, char *key, char *value, bool equal);

// Identifier: pas de num en str[0]
// - le reste alnum
// Les variables vides -> ne pas afficher =''

void    ft_built_in_export(t_env **head, char **full_cmd)
{
	size_t  i;
	char	*key;
	char	*value;
	char	*equal;

	i = 1;
	if (!full_cmd[i])
		ft_print_env_sorted(*head);
	while (full_cmd[i])
	{
		equal = ft_strchr(full_cmd[i], '=');
		if (equal)
		{
			key = ft_strndup(full_cmd[i], equal - full_cmd[i]);
			if (ft_get_env(head, key))
				ft_set_env(head, key, ft_strdup(equal + 1), true);
			else
			{
				value = ft_strdup(equal + 1);
				*head = add_node(*head, key, value, true);
				free(key);
			}
		}
		else
		{
			if (ft_get_env(head, full_cmd[i]))
				break;
			else
			*head = add_node(*head, full_cmd[i], "", false);
		}
		i++;
	}
}

static void	remove_env_node(t_env **head, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *head;
	prev = NULL;
	if (current != NULL && ft_strcmp(current->key, key) == 0)
	{
		*head = current->next;
		free(current->key);
		free(current->value);
		free(current);
		return ;
	}
	while (current != NULL && ft_strcmp(current->key, key) != 0)
	{
		prev = current;
		current = current->next;
	}
	if (current == NULL)
		return ;
	prev->next = current->next;
	return (free(current->key), free(current->value), free(current));
}

void	ft_built_in_unset(t_env **head, char *key)
{
	size_t	i;
	t_env	*temp;
	char	**key_list;

	i = 1;
	key_list = ft_split(key, ' ');
	while (key_list[i])
	{
		temp = *head;
		while (temp)
		{
			if (ft_strcmp(temp->key, key_list[i]) == 0)
				remove_env_node(head, key_list[i]);
			temp = temp->next;
		}
		i++;
	}
}
