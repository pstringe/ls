/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 18:55:49 by pstringe          #+#    #+#             */
/*   Updated: 2018/04/30 19:23:42 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_ls(t_queue *sd)
{
	t_dir	*wd;

	if (!sd->tail)
		return (0);
	wd = ft_dequeue(sd);
	search_diectory(wd);
	display_directory(wd);
	while ((wd = ft_dequeu(sd)))
		ft_ls(sd);
}

int		main(int argc, char **argv)
{
	t_queue		*sub_dirs;

	sub_dirs = ft_queuenw(get_wd());
	return (0);
}
