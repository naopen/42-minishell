/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:06:55 by nkannan           #+#    #+#             */
/*   Updated: 2024/08/11 23:33:21 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	fatal_error(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	char	*str;

	if (s == NULL)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
		i++;
	str = (char *)malloc(sizeof(char) * (i + 1));
	if (str == NULL)
		fatal_error("malloc");
	ft_strlcpy(str, s, i + 1);
	return (str);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;

	if (s == NULL)
		return (NULL);
	if (start >= (size_t)ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s) - (size_t)start)
		len = ft_strlen(s) - (size_t)start;
	substr = (char *)malloc(sizeof(char) * (len + 1));
	if (substr == NULL)
		fatal_error("malloc");
	i = 0;
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

char	*ft_strdup(const char *s)
{
	return (ft_strndup(s, ft_strlen(s)));
}

char	*ft_strjoin_char_free(char *s1, char s2)
{
	char	*new_str;

	new_str = ft_strjoin(s1, &s2);
	if (new_str == NULL)
		fatal_error("malloc");
	free(s1);
	return (new_str);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*new_str;

	new_str = ft_strjoin(s1, s2);
	if (new_str == NULL)
		fatal_error("malloc");
	free(s1);
	free(s2);
	return (new_str);
}

char	*ft_strjoin_space_free(char *s1, char *s2)
{
	char	*new_str;

	new_str = ft_strjoin(s1, " ");
	if (new_str == NULL)
		fatal_error("malloc");
	new_str = ft_strjoin_free(new_str, s2);
	return (new_str);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	if (size == 0)
		return (ft_strlen(src));
	i = 0;
	while (src[i] && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*last;
	char		*start;

	if (str == NULL)
		str = last;
	if (str == NULL)
		return (NULL);
	while (*str && ft_strchr(delim, *str))
		str++;
	if (*str == '\0')
		return (NULL);
	start = str;
	while (*str && !ft_strchr(delim, *str))
		str++;
	if (*str != '\0')
	{
		*str = '\0';
		last = str + 1;
	}
	else
		last = NULL;
	return (start);
}


int	ft_fnmatch(const char *pattern, const char *string, int flags)
{
	(void)flags;
	if (*pattern == '*' && *(pattern + 1) == '\0')
		return (0);
	if (*pattern == '\0' && *string == '\0')
		return (0);
	if (*pattern == '*' && *(pattern + 1) != '\0' && *string == '\0')
		return (FNM_NOMATCH);
	if (*pattern == *string)
		return (ft_fnmatch(pattern + 1, string + 1, flags));
	if (*pattern == '*')
		return (ft_fnmatch(pattern + 1, string, flags)
			|| ft_fnmatch(pattern, string + 1, flags));
	return (FNM_NOMATCH);
}

// exportの識別子として有効かどうかを判定する関数
bool	is_valid_identifier(char *str)
{
	size_t	i;

	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (false);
	i++;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}
