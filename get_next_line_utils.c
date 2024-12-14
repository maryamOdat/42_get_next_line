/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 17:24:58 by modat             #+#    #+#             */
/*   Updated: 2024/11/08 19:30:02 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>

int	ft_strlen(char *buffer)
{
	size_t	i;

	i = 0;
	while (buffer[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*ft_strdup(char *s)
{
	size_t	i;
	size_t	n;
	char	*ptr;

	n = ft_strlen(s) + 1;
	ptr = (char *)malloc(n * sizeof(char));
	if (!ptr)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		ptr[i] = s[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	slen;
	size_t	available_len;
	size_t	i;

	i = 0;
	if (!s)
		return (ft_strdup(""));
	slen = ft_strlen(s);
	if (start >= slen)
		return (ft_strdup(""));
	available_len = slen - start;
	if (available_len < len)
		len = available_len;
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	i;
	char	*str;
	size_t	slen;
	char	*temp;

	temp = s1;
	i = 0;
	if (!s1 || !s2)
		return (free(temp), NULL);
	slen = ft_strlen(s1) + ft_strlen(s2);
	str = (char *)malloc(slen + 1);
	if (!str)
		return (free(temp), NULL);
	while (*s1)
	{
		str[i++] = *s1++;
	}
	while (*s2)
	{
		str[i++] = *s2++;
	}
	str[i] = '\0';
	return (free(temp), str);
}

char	*ft_strchr(char *s, int c)
{
	while ((*s != '\0') && (*s != (char)c))
	{
		s++;
	}
	if (*s == (char)c)
	{
		return ((char *)s);
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}
