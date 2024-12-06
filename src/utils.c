/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 17:06:55 by nkannan           #+#    #+#             */
/*   Updated: 2024/12/06 16:37:34 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	ft_isnumber(const char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

char	*ft_strndup(t_mini *mini, const char *s, size_t n)
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
		system_error(mini);
	ft_strlcpy(str, s, i + 1);
	return (str);
}

char	*mn_substr(t_mini *mini, char const *s, unsigned int start, size_t len)
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
		system_error(mini);
	i = 0;
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

void	ft_strdel(char **as)
{
	if (as && *as)
	{
		free(*as);
		*as = NULL;
	}
}

int	ft_strarrlen(char **arr)
{
	int	len;

	len = 0;
	while (arr && arr[len])
		len++;
	return (len);
}

char	**ft_strarradd(t_mini *mini, char **arr, char *str)
{
	char	**new_arr;
	int		len;
	int		i;

	len = ft_strarrlen(arr);
	new_arr = (char **)malloc(sizeof(char *) * (len + 2));
	if (new_arr == NULL)
		system_error(mini);
	i = 0;
	while (i < len)
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[i] = str;
	new_arr[i + 1] = NULL;
	free(arr);
	return (new_arr);
}


char	*ft_charjoin_free(t_mini *mini, char *s1, char s2)
{
	char	*new_str;
	size_t	len;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	new_str = (char *)malloc(sizeof(char) * (len + 2));
	if (!new_str)
		system_error(mini);
	ft_strlcpy(new_str, s1, len + 1);
	new_str[len] = s2;
	new_str[len + 1] = '\0';
    free(s1);
	return (new_str);
}


char	*ft_strjoin_free(t_mini *mini, char *s1, char *s2)
{
	char	*new_str;

	new_str = ft_strjoin(s1, s2);
	if (!new_str)
		system_error(mini);
	free(s1);
	free(s2);
	return (new_str);
}

char	*ft_strjoin_space_free(t_mini *mini, char *s1, char *s2)
{
    char    *tmp;
    char    *result;

    tmp = ft_charjoin_free(mini, s1, ' ');
    if (!tmp)
        system_error(mini);
    result = ft_strjoin_free(mini, tmp, s2);
    if(!result)
        system_error(mini);
    return (result);
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


// int	ft_fnmatch(const char *pattern, const char *string, int flags)
// {
// 	(void)flags;
// 	if (*pattern == '*' && *(pattern + 1) == '\0')
// 		return (0);
// 	if (*pattern == '\0' && *string == '\0')
// 		return (0);
// 	if (*pattern == '*' && *(pattern + 1) != '\0' && *string == '\0')
// 		return (FNM_NOMATCH);
// 	if (*pattern == *string)
// 		return (ft_fnmatch(pattern + 1, string + 1, flags));
// 	if (*pattern == '*')
// 		return (ft_fnmatch(pattern + 1, string, flags)
// 			|| ft_fnmatch(pattern, string + 1, flags));
// 	return (FNM_NOMATCH);
// }
