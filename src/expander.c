/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:09:48 by nkannan           #+#    #+#             */
/*   Updated: 2024/11/16 16:03:16 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*expand_variable(char *str, t_env *env_list)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	size_t	i;
	size_t	j;
	size_t	k;

	result = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (result == NULL)
		exit_with_error("minishell: malloc error");
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0')
		{
			i++;
			k = 0;
			while (str[i + k] && (ft_isalnum(str[i + k]) || str[i + k] == '_'))
				k++;
			var_name = ft_strndup(str + i, k);
			if (var_name == NULL)
				exit_with_error("minishell: malloc error");
			var_value = get_env_value(env_list, var_name);
			free(var_name);
			if (var_value)
			{
				strcpy(result + j, var_value);
				j += ft_strlen(var_value);
			}
			i += k;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

static char	*expand_double_quote(char *str)
{
	char	*result;
	size_t	i;
	size_t	j;

	result = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (result == NULL)
		exit_with_error("minishell: malloc error");
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

static void	remove_quotes(char **str)
{
	char	*new_str;
	size_t	i;
	size_t	j;

	new_str = (char *)malloc(sizeof(char) * (ft_strlen(*str) + 1));
	if (new_str == NULL)
		return ;
	i = 0;
	j = 0;
	while ((*str)[i])
	{
		if ((*str)[i] != '\'' && (*str)[i] != '\"')
			new_str[j++] = (*str)[i];
		i++;
	}
	new_str[j] = '\0';
	free(*str);
	*str = new_str;
}

static void	expand_argv(char **argv, t_env *env_list)
{
	int		i;
	char	*expanded_str;

	i = 0;
	while (argv[i])
	{
		if (ft_strchr(argv[i], '$'))
		{
			expanded_str = expand_variable(argv[i], env_list);
			free(argv[i]);
			argv[i] = expanded_str;
		}
		if (ft_strchr(argv[i], '\"'))
			argv[i] = expand_double_quote(argv[i]);
		remove_quotes(&argv[i]);
		i++;
	}
}

static void	expand_redirects(t_redirect *redirects, t_env *env_list)
{
	while (redirects)
	{
		redirects->file_name = expand_variable(redirects->file_name,
				env_list);
		redirects = redirects->next;
	}
}

void	expand(t_node *node, t_env *env_list)
{
	if (node == NULL)
		return ;
	expand_argv(node->argv, env_list);
	expand_redirects(node->redirects, env_list);
	expand(node->next, env_list);
}
