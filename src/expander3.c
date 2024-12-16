/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:42:31 by mkaihori          #+#    #+#             */
/*   Updated: 2024/12/16 15:45:42 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_argv(t_mini *mini, char **argv, t_env *env_list)
{
	int		i;
	char	*expanded_str;

	i = 0;
	while (argv[i])
	{
		if (ft_strchr(argv[i], '$'))
		{
			expanded_str = expand_variable(mini, argv[i], env_list, 0);
			free(argv[i]);
			argv[i] = expanded_str;
		}
		if (ft_strchr(argv[i], '\"'))
			argv[i] = expand_double_quote(mini, argv[i]);
		remove_quotes(&argv[i]);
		i++;
	}
}

void	expand_redirects(t_mini *mini,
	t_redirect *redirects, t_env *env_list)
{
	while (redirects)
	{
		redirects->file_name = expand_variable(mini, redirects->file_name,
				env_list, 0);
		redirects = redirects->next;
	}
}

void	expand(t_mini *mini, t_node *node, t_env *env_list)
{
	if (node == NULL)
		return ;
	expand_argv(mini, node->argv, env_list);
	expand_redirects(mini, node->redirects, env_list);
	expand(mini, node->next, env_list);
}

char	*get_env_name(t_mini *mini, const char *str)
{
	int		i;
	char	*name;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	name = ft_strndup(mini, str, i);
	if (!name)
		system_error(mini);
	return (name);
}

char	*expand_double_quote(t_mini *mini, char *str)
{
	char	*result;
	size_t	i;
	size_t	j;

	result = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (result == NULL)
		system_error(mini);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0' && str[i + 1] != '\"')
		{
			result[j++] = str[i++];
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				result[j++] = str[i++];
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}
