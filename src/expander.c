/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:09:48 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/10 23:22:00 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*expand_variable(t_mini *mini, char *str, t_env *env_list)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	size_t	i;
	size_t	j;
	size_t	k;

	result = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (result == NULL)
		system_error(mini);
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
			var_name = ft_strndup(mini, str + i, k);
			if (var_name == NULL)
				system_error(mini);
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

static char	*expand_double_quote(t_mini *mini, char *str)
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

static void	expand_argv(t_mini *mini, char **argv, t_env *env_list)
{
	int		i;
	char	*expanded_str;

	i = 0;
	while (argv[i])
	{
		if (ft_strchr(argv[i], '$'))
		{
			expanded_str = expand_variable(mini, argv[i], env_list);
			free(argv[i]);
			argv[i] = expanded_str;
		}
		if (ft_strchr(argv[i], '\"'))
			argv[i] = expand_double_quote(mini, argv[i]);
		remove_quotes(&argv[i]);
		i++;
	}
}

static void	expand_redirects(t_mini *mini, t_redirect *redirects, t_env *env_list)
{
	while (redirects)
	{
		redirects->file_name = expand_variable(mini, redirects->file_name,
				env_list);
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

static char	*get_env_name(t_mini *mini, const char *str)
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

char	*expand_env_var(t_mini *mini, char *str)
{
	char	*result;
	char	*env_name;
	char	*env_value;
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	result = ft_strdup("");
	if (!result)
		system_error(mini);
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '$' && !in_single_quote)
		{
			i++;
			if (str[i] == '?')
			{
				char *status_str = ft_itoa(mini->status);
				if (!status_str)
					system_error(mini);
				char *tmp = ft_strjoin(result, status_str);
				free(status_str);
				free(result);
				result = tmp;
			}
			else if (ft_isalnum(str[i]) || str[i] == '_')
			{
				env_name = get_env_name(mini, str + i);
				env_value = getenv(env_name);
				if (env_value)
				{
					char *tmp = ft_strjoin(result, env_value);
					free(result);
					result = tmp;
				}
				if (ft_strlen(env_name))
					i += ft_strlen(env_name) - 1;
				free(env_name);
			}
			else
			{
				char *tmp = malloc(ft_strlen(result) + 2);
				if (!tmp)
					system_error(mini);
				ft_strlcpy(tmp, result, ft_strlen(result) + 1);
				tmp[ft_strlen(result)] = '$';
				tmp[ft_strlen(result) + 1] = '\0';
				free(result);
				result = tmp;
			}
		}
		else
		{
			char *tmp = malloc(ft_strlen(result) + 2);
			if (!tmp)
				system_error(mini);
			ft_strlcpy(tmp, result, ft_strlen(result) + 1);
			tmp[ft_strlen(result)] = str[i];
			tmp[ft_strlen(result) + 1] = '\0';
			free(result);
			result = tmp;
		}
		i++;
	}
	free(str);
	return (result);
}
