#include "header.h"


int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}


char	*ft_strdup(char *s1)
{
	char	*copy;
	int		i;

	if (!s1)
		return (NULL);

	copy = malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!copy)
		return (NULL);

	i = 0;
	while (s1[i])
	{
		copy[i] = s1[i];
		i++;
	}

	copy[i] = '\0';

	return (copy);
}


char	*ft_substr(char *s, int start, int len)
{
	char	*sub;
	int		i;

	if (!s)
		return (NULL);

	if (start >= ft_strlen(s))
		return (ft_strdup(""));

	sub = malloc(sizeof(char) * (len + 1));
	if (!sub)
		return (NULL);

	i = 0;
	while (i < len && s[start + i])
	{
		sub[i] = s[start + i];
		i++;
	}

	sub[i] = '\0';

	return (sub);
}


int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;

	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;

	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}


int	ft_strncmp(char *s1, char *s2, int n)
{
	int	i;

	i = 0;

	while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
		i++;

	if (i == n)
		return (0);

	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}


char	*ft_strjoin(char *s1, char *s2)
{
	char	*result;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);

	result = malloc(sizeof(char) * (ft_strlen(s1)
				+ ft_strlen(s2) + 1));
	if (!result)
		return (NULL);

	i = 0;
	j = 0;

	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}

	while (s2[j])
	{
		result[i] = s2[j];
		i++;
		j++;
	}

	result[i] = '\0';

	return (result);
}


int	ft_isalnum(int c)
{
	if ((c >= '0' && c <= '9')
		|| (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z'))
		return (1);

	if (c == '_')
		return (1);

	return (0);
}