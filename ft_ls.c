/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 18:55:49 by pstringe          #+#    #+#             */
/*   Updated: 2018/05/01 06:58:08 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
/*
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
*/
int		display_directory(char *dir)
{
	DIR				*dp;
	struct dirent	*dptr;
	
	if (!(dp = opendir((const char*)dir)))
		return (-1);
	while ((dptr = readdir(dp)))
		if (dptr->d_name[0] != '.')
			ft_putendl(dptr->d_name);
	return (0);
}

int		main(int argc, char **argv)
{
	//t_queue		*sub_dirs;
	int			i;
	char		*flags;

	flags = NULL;
	if (argc >= 2)
	{
		flags = argv[1][0] == '-' ? (argv[1] + 1) : NULL;
		i = flags ? 1 : 0;
		while (++i < 2)
			display_directory(argv[i]);
	}
	if ((flags && argc == 2) || (!flags && argc == 1))
		display_directory(".");
	return (0);
}
