#include "../include/42api.h"

char	*ft_strjoin(char *s1, char *s2)
{
	char	*conc;
	size_t	i;
	size_t	size_s1;
	size_t	size_s2;

	i = -1;
	size_s1 = 0;
	size_s2 = 0;
	while (s1[size_s1])
		size_s1++;
	while (s2[size_s2])
		size_s2++;
	conc = malloc((size_s2 + size_s1 + 1) * sizeof(char));
	while (++i < size_s1)
		conc[i] = s1[i];
	i = -1;
	while (++i < size_s2)
		conc[i + size_s1] = s2[i];
	conc[size_s2 + size_s1] = '\0';
	free(s1);
	return (conc);
}

size_t	count_letters(char const *s, char c)
{
	int	amount_of_letters;

	amount_of_letters = 0;
	while (s[amount_of_letters] != c && s[amount_of_letters])
		amount_of_letters++;
	return (amount_of_letters);
}

static size_t	count_words(char const *s, char c)
{
	int		total;
	int		i;

	i = 0;
	total = 0;
	while (s[i] && s[i] == c)
		i++;
	while (s[i])
	{
		while (s[i] && s[i] != c)
			i++;
		total++;
		while (s[i] && s[i] == c)
			i++;
	}
	return (total);
}

char	*ft_substrsplit(char const *s, unsigned int start, size_t len)
{
	size_t	size;
	size_t	i;
	char	*sub;

	if (!s)
		return (NULL);
	i = 0;
	size = 0;
	while (s[size])
		size++;
	if ((size + 1) < len + 1)
		sub = (char *)malloc(size + 1);
	else if (len == size)
		sub = (char *)malloc((len - start) + 1);
	else
		sub = (char *)malloc(len + 1);
	if (!sub)
		return (NULL);
	while ((i < len) && (start + i) < size)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

char	**ft_split_long(char const *s, char c)
{
	char	**list;
	size_t	c_words;
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	c_words = count_words(s, c);
	list = malloc(sizeof(char *) * (c_words + 1));
	if (!list)
		return (0);
	while (i < c_words && *s)
	{
		if (*s != c)
		{
			list[i] = ft_substrsplit(s, 0, count_letters(s, c));
			if (!list[i++])
				return (0);
			s += count_letters(s, c);
		}
		else
			s++;
	}
	list[c_words] = NULL;
	return (list);
}

