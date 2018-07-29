/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/24 17:24:10 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/28 19:09:27 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		die(int c, char *path)
{
	if (c == -1)
		ft_printf("ft_ls: %s: you do not have permission to this directory\n", path);
	else if (c == -2)
		ft_printf("ft_ls: %s: no such file or directory\n", path);
	else if (c == -3)
		ft_printf("ft_ls: illegal option -- %s\nusage: ls [-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1] [file ...]\n", path);
	return (c);
}

void 	get_path(char path[512], char *parent, char *name)
{
	if (!path || !parent)
		return ;
	ft_bzero(path, 512);
	ft_memcpy(path, parent, ft_strlen(parent));
	ft_strlcat(path, "/", 512);
	ft_strlcat(path, name, 512);
}

char	*get_month(int d)
{
	char *m;

	if (d == 0)
		m = "Jan";
	else if (d == 1)
		m = "Feb";
	else if (d == 2)
		m = "Mar";
	else if (d == 3)
		m = "Apr";
	else if (d == 4)
		m = "May";
	else if (d == 5)
		m = "Jun";
	else if (d == 6)
		m = "Jul";
	else if (d == 7)
		m = "Aug";
	else if (d == 8)
		m = "Sept";
	else if (d == 9)
		m = "Oct";
	else if (d == 10)
		m = "Nov";
	else if (d == 11)
		m = "Dec";
	else
		m = NULL;
	return (m);
}
