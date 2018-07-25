/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/24 17:24:10 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/24 17:58:00 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char 	*get_path(char *parent, char *name)
{
	char *tmp;
	char *new;

	tmp = ft_strjoin(parent, "/");
	new = ft_strjoin(tmp, name);
	ft_memdel((void**)&tmp);
	return (new);
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
