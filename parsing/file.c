/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:59:01 by zelbassa          #+#    #+#             */
/*   Updated: 2025/01/20 16:04:44 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"

void	open_file(t_rt *rt, char *path)
{
	rt->file_fd = open(path, O_RDONLY);
	rt->file = path;
	if (check_file(rt) == 1)
	{
		free(rt);
		exit(1);
	}
}

int	check_file(t_rt *rt)
{
	if (rt->file == NULL)
	{
		printf("Error: file is NULL\n");
		return (1);
	}
	if (rt->file_fd < 0)
	{
		printf("Error: file descriptor is invalid\n");
		return (1);
	}
	if (is_rt_file(rt->file) == 0)
	{
		printf("Error: file is not a .rt file\n");
		return (1);
	}
	return (0);
}

int	is_rt_file(char *path)
{
	int	len;

	len = ft_strlen(path) - 3;
	if (len > 3)
		return (ft_strncmp(path + len, ".rt", 3) == 0);
	return (0);
}
