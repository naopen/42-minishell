/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:09:48 by nkannan           #+#    #+#             */
/*   Updated: 2024/08/08 17:09:57 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_tokens(t_token *tokens, int last_status)
{
	while (tokens)
	{
		if (tokens->kind == TK_WORD)
		{
			char	*expanded_str;

			expanded_str = expand_variable(tokens->str, last_status);
			free(tokens->str);
			tokens->str = expanded_str;
		}
		tokens = tokens->next;
	}
}

char	*expand_variable(char *str, int last_status)
{
	char	*new_str;
	char	*value;
	size_t	i;
	size_t	j;

	new_str = ft_strdup("");
	if (new_str == NULL)
		fatal_error("malloc");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			if (str[i] == '?')
			{
				new_str = ft_strjoin_free(new_str,
						expand_special_parameter(str + i, last_status));
				i++;
			}
			else
			{
				j = i;
				while (ft_isalnum(str[j]) || str[j] == '_')
					j++;
				value = getenv(ft_substr(str, i, j - i));
				if (value)
					new_str = ft_strjoin_free(new_str, value);
				i = j;
			}
		}
		else
		{
			if (ft_strchr(str + i, '*') != NULL)
				new_str = ft_strjoin_free(new_str, expand_wildcard(str + i));
			else
			{
				new_str = ft_strjoin_char_free(new_str, str[i]);
				i++;
			}
		}
	}
	return (new_str);
}

char	*expand_special_parameter(char *str, int last_status)
{
	if (ft_strncmp(str, "?", 1) == 0)
	{
		char	*status_str;

		status_str = ft_itoa(last_status);
		if (status_str == NULL)
			fatal_error("malloc");
		return (status_str);
	}
	return (NULL);
}

void	remove_quotes(char *str)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			i++;
		else
			str[j++] = str[i++];
	}
	str[j] = '\0';
}

char	*expand_wildcard(char *str)
{
	char			*new_str;
	DIR				*dir;
	struct dirent	*ent;

	new_str = NULL;
	dir = opendir(".");
	if (dir == NULL)
		return (ft_strdup(str));
	while ((ent = readdir(dir)) != NULL)
	{
		if (ft_fnmatch(str, ent->d_name, 0) == 0)
		{
			if (new_str == NULL)
				new_str = ft_strdup(ent->d_name);
			else
				new_str = ft_strjoin_space_free(new_str, ent->d_name);
		}
	}
	closedir(dir);
	if (new_str == NULL)
		return (ft_strdup(str));
	return (new_str);
}
