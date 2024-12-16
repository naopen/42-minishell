/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaihori <nana7hachi89gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:06:20 by mkaihori          #+#    #+#             */
/*   Updated: 2024/12/16 15:45:46 by mkaihori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ex_var_name(t_mini *mini, char *str, size_t i, size_t k)
{
	char	*var_name;

	var_name = ft_strndup(mini, str + i, k);
	if (var_name == NULL)
		system_error(mini);
	return (var_name);
}

void	ex_var_value(t_env *env_list, char *var_name, char **result, size_t *j)
{
	char	*var_value;

	var_value = get_env_value(env_list, var_name);
	free(var_name);
	if (var_value)
	{
		strcpy((*result) + (*j), var_value);
		*j += ft_strlen(var_value);
	}
	return ;
}

char	*ready_result(char *str, t_mini *mini)
{
	char	*result;

	result = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (result == NULL)
		system_error(mini);
	return (result);
}

char	*expand_variable(t_mini *mini, char *str, t_env *env_list, size_t i)
{
	char	*result;
	char	*var_name;
	size_t	j;
	size_t	k;

	result = ready_result(str, mini);
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0')
		{
			i++;
			k = 0;
			while (str[i + k] && (ft_isalnum(str[i + k]) || str[i + k] == '_'))
				k++;
			var_name = ex_var_name(mini, str, i, k);
			ex_var_value(env_list, var_name, &result, &j);
			i += k;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

void	remove_quotes(char **str)
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
