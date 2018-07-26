/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recurse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/24 17:20:56 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/26 15:39:23 by pstringe         ###   ########.fr       */
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
	//char			pass[512];
	char			tmp[512];
	
	if (can_output(dir, ops))
		output_dir(dir, ops);
	if ((dpntr = opendir(dir)) == NULL)
		return ;
	while (dpntr && (cur = readdir(dpntr)))
	{
		if (can_recurse(dir, cur->d_name, ops))
		{
			if (S_ISLNK(get_stats(dir).st_mode) && readlink(dir, tmp, 512) >= 0)
				subdir = get_path(dir, tmp);
			else
				subdir = get_path(dir, cur->d_name);
			if (!opendir(subdir))
			{
				ft_memdel((void**)&subdir);
				continue ;
			}
			//ft_strncpy(pass, subdir, 512);
			recurse(subdir, ops);
			ft_memdel((void**)&subdir);
		}
	}
	//assert(ft_strncmp(dir, "//nfs/2018/j/jsanders/Day05", 27));
	if (dir)
		ft_memdel((void**)&cur);
	if (cur)
		ft_memdel((void**)&cur);
	closedir(dpntr);
}

