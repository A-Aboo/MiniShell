/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anasimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 09:42:20 by anasimi           #+#    #+#             */
/*   Updated: 2025/11/22 09:42:21 by anasimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_words(char const *s, char c)
{
	int	i;
	int	w;
	int	in;

	i = 0;
	w = 0;
	in = 0;
	while (s[i])
	{
		if (s[i] == c)
			in = 0;
		else if (!in)
		{
			w++;
			in = 1;
		}
		i++;
	}
	return (w);
}

char	*add(char const *s, char c)
{
	int		len;
	int		i;
	char	*w;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	w = malloc((len + 1) * sizeof(char));
	if (!w)
		return (NULL);
	i = 0;
	while (i < len)
	{
		w[i] = s[i];
		i++;
	}
	w[len] = '\0';
	return (w);
}

void	free_words(char **r, int j)
{
	while (j > 0)
	{
		j--;
		free(r[j]);
	}
	free(r);
}

int	split_fill(char **r, char const *s, char c)
{
	int	i;
	int	j;
	int	in;

	i = 0;
	j = 0;
	in = 0;
	while (s[i])
	{
		if (s[i] != c && !in)
		{
			r[j] = add(&s[i], c);
			if (!r[j])
				return (free_words(r, j), 0);
			in = 1;
			j++;
		}
		if (s[i] == c)
			in = 0;
		i++;
	}
	r[j] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**r;
	int		w;

	if (!s)
		return (NULL);
	w = count_words(s, c);
	r = malloc((w + 1) * sizeof(char *));
	if (!r)
		return (NULL);
	if (!split_fill(r, s, c))
		return (NULL);
	return (r);
}
