/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:09:48 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/16 15:45:00 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	get_status(t_mini *mini, char **result)
{
	char	*status_str;
	char	*tmp;

	status_str = ft_itoa(mini->status);
	if (!status_str)
		system_error(mini);
	tmp = ft_strjoin(*result, status_str);
	free(status_str);
	free(*result);
	*result = tmp;
}

void	env_values(t_mini *mini, char *str, int *i, char **result)
{
	char	*env_name;
	char	*env_value;
	char	*tmp;

	env_name = get_env_name(mini, str + *i);
	env_value = get_env(mini, env_name);
	if (env_value)
	{
		tmp = ft_strjoin(*result, env_value);
		free(*result);
		*result = tmp;
	}
	if (ft_strlen(env_name))
		*i += ft_strlen(env_name) - 1;
	free(env_name);
	return ;
}

void	env_value(t_mini *mini, char **result, char *str, int *i)
{
	char	*tmp;

	(*i)++;
	if (str[*i] == '?')
		get_status(mini, result);
	else if (ft_isalnum(str[*i]) || str[*i] == '_')
		env_values(mini, str, i, result);
	else
	{
		tmp = malloc(ft_strlen(*result) + 2);
		if (!tmp)
			system_error(mini);
		ft_strlcpy(tmp, *result, ft_strlen(*result) + 1);
		tmp[ft_strlen(*result)] = '$';
		tmp[ft_strlen(*result) + 1] = '\0';
		free(*result);
		*result = tmp;
	}
}

void	re_malloc(t_mini *mini, char **result, int i, char *str)
{
	char	*tmp;

	tmp = malloc(ft_strlen(*result) + 2);
	if (!tmp)
		system_error(mini);
	ft_strlcpy(tmp, *result, ft_strlen(*result) + 1);
	tmp[ft_strlen(*result)] = str[i];
	tmp[ft_strlen(*result) + 1] = '\0';
	free(*result);
	*result = tmp;
	return ;
}

char	*expand_env_var(t_mini *mini, char *str)
{
	char	*result;
	int		i;
	bool	in_single_quote;
	bool	in_double_quote;

	result = ft_strdup("");
	if (!result)
		system_error(mini);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '$' && !in_single_quote)
			env_value(mini, &result, str, &i);
		else
			re_malloc(mini, &result, i, str);
		if (str[i] != '\0')
			i++;
	}
	free(str);
	return (result);
}
