/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 20:00:11 by mel-bouh          #+#    #+#             */
/*   Updated: 2023/11/06 18:03:48 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	length;
	char	*sub;

	i = start;
	j = 0;
	length = ft_strlen(s);
	if (s == NULL || start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len > length - start)
		len = length - start;
	sub = (char *)malloc(len + 1);
	if (sub == NULL)
		return (NULL);
	while (s[i] && j < len)
		sub[j++] = s[i++];
	sub[j] = '\0';
	return (sub);
}
