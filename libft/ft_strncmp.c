/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 17:47:26 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/09/26 21:44:11 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
	{
		while (*s1 && *s2 && *s1 == *s2)
		{
			s1++;
			s2++;
		}
	}
	else
	{
		while (*s1 == *s2 && *s1 && *s2 && n > 1)
		{
			s1++;
			s2++;
			n--;
		}
	}
	return (((unsigned char)(*s1) - (unsigned char)(*s2)));
}
