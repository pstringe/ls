/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recurse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/24 17:20:56 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/28 18:03:09 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int		can_output(char *file, t_ops *ops)
{
	int 	l;
	int 	hidden;
	int 	cur_dir;
	char 	*a;
	char 	*b;

	if (!file)
		return (0);
	l = ft_strlen(file);
	cur_dir = !ft_strncmp(file, ".", l);
	hidden = !cur_dir && ((a = strrchr(file, '.')) > (b = strrchr(file, '/')));
	a = NULL;
	b = NULL;
	return ((!ops->a && (!hidden || cur_dir)) || ops->a);
}

static int		can_recurse(char *dir, char *file, t_ops *ops)
{
	int		l;
	int		link;
	char 	link_name[512];
	char	tmp[512];

	link = 0;
	if (S_ISLNK(get_stats(file).st_mode) && readlink(file, link_name, 512) >= 0)
	{
		link = 1;
		ft_strncpy(tmp, dir, ft_strlen(dir));
		ft_strlcat(tmp, "/", 1);
		ft_strlcat(tmp, link_name, ft_strlen(link_name));
	}
	l = ft_strlen(file);
	return (can_output(link ? tmp : file, ops) &&
			ft_strncmp(link ? tmp : file, ".", l) &&
			ft_strncmp(link ? tmp : file, "..", l));
}

void 	recurse(char *dir, t_ops *ops)
{
	DIR				*dpntr;
	struct dirent 	*cur;
	char 			*subdir;

	subdir = ft_strnew(512);
	if (can_output(dir, ops))
		output_dir(dir, ops);
	if (!(dpntr = opendir(dir)))
		return ;
	ft_bzero(subdir, 512);
	while (dpntr && (cur = readdir(dpntr)))
	{
		if (can_recurse(dir, cur->d_name, ops))
		{
			get_path(subdir, dir, cur->d_name);
			if (!subdir)
			{
				ft_bzero(subdir, 512);
				subdir = NULL;
			}
			if (subdir && !S_ISDIR(get_stats(subdir).st_mode))
			{
				ft_bzero(subdir, 512);
				continue ;
			}
			recurse(subdir, ops);
			ft_bzero(subdir, 512);
			subdir = NULL;
		}
	}
	free(subdir);
	subdir = NULL;
	closedir(dpntr);
}

