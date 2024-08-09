/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 14:31:10 by nkannan           #+#    #+#             */
/*   Updated: 2024/03/23 04:32:54 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

// Returns an array of strings obtained by splitting ’s’ using the
// character ’c’ as a delimiter. The array must be ended by a NULL.
// If split("AAbbBBbbCC", "b") -> {"AA", "BB", "CC", NULL}

// If malloc returns NULL when allocating memory for the split string area,
// free all previously allocated memory.

static size_t	ft_count_words(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (s[i] != c)
			count++;
		while (s[i] != c && s[i + 1])
			i++;
		i++;
	}
	return (count);
}

static size_t	ft_word_len(char const *s, char c, size_t j)
{
	size_t	len;

	len = 0;
	while (s[j] != c && s[j])
	{
		len++;
		j++;
	}
	return (len);
}

static char	**ft_free(char **split, size_t i)
{
	while (i--)
		free(split[i]);
	free(split);
	return (NULL);
}

static char	**ft_split_helper(char const *str, char ch, char **split,
		size_t count)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	while (i < count)
	{
		while (str[j] == ch)
			j++;
		split[i] = (char *)ft_calloc(ft_word_len(str, ch, j) + 1, sizeof(char));
		if (!split[i])
			return (ft_free(split, i));
		k = 0;
		while (str[j] != ch && str[j])
			split[i][k++] = str[j++];
		i++;
	}
	split[i] = NULL;
	return (split);
}

char	**ft_split(char const *str, char ch)
{
	char	**split;
	size_t	count;

	if (!str)
		return (NULL);
	count = ft_count_words(str, ch);
	split = (char **)ft_calloc(count + 1, sizeof(char *));
	if (!split)
		return (NULL);
	return (ft_split_helper(str, ch, split, count));
}
