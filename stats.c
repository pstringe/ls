/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/24 17:33:24 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/28 20:30:20 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

struct stat	get_stats(const char *path)
{
	struct stat	stats;

	lstat(path, &stats);
	return (stats);
}

int			get_blocks(char *path)
{
	struct stat	f;

	stat(path, &f);
	return (f.st_blocks);
}

void		meat(char *path, struct stat stats)
{
	char *pw;
	char *gw;

	pw = getpwuid(stats.st_uid) ? getpwuid(stats.st_uid)->pw_name : NULL;
	gw = getgrgid(stats.st_gid) ? getgrgid(stats.st_gid)->gr_name : NULL;
	output_type(path, stats.st_mode);
	output_permissions(stats.st_mode);
	ft_printf(" %d", stats.st_nlink);
	ft_printf("%10s %10s", pw, gw);
}

void		output_stats(char *file, void **aux)
{
	struct stat		stats;
	t_ops			*ops;
	char			path[512];

	ops = (t_ops*)*aux;
	get_path(path, (char*)aux[1], file);
	if (!ops->l)
		ft_printf("%s\n", file);
	else
	{
		stats = get_stats(path);
		meat(path, stats);
		if (stats.st_rdev)
			ft_printf("%3d, %3d ", major(stats.st_rdev), minor(stats.st_rdev));
		else
			ft_printf("%10lld ", stats.st_size);
		output_time(stats.st_mtime);
		output_name(path, stats.st_mode);
	}
}
