/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bouh <mel-bouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 19:42:06 by mel-bouh          #+#    #+#             */
/*   Updated: 2023/11/05 21:49:39 by mel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	void			*p;

	i = 0;
	p = (void *)s;
	while (i < n)
	{
		if (((unsigned char *)p)[i] == (unsigned char)c)
			return (p + i);
		i++;
	}
	return (NULL);
}
