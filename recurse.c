/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recurse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/24 17:20:56 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/24 17:21:46 by pstringe         ###   ########.fr       */
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

static int		can_recurse(char *file, t_ops *ops)
{
	int		l;

	l = ft_strlen(file);
	return (can_output(file, ops) && ft_strncmp(file, ".", l) && ft_strncmp(file, "..", l));
}

void 	recurse(char *dir, t_ops *ops)
{
	DIR				*dpntr;
	struct dirent 	*cur;
	char 			*subdir;
	char			pass[512];

	if (can_output(dir, ops))
		output_dir(dir, ops);
	dpntr = opendir(dir);
	while (dpntr && (cur = readdir(dpntr)))
	{
		if (can_recurse(cur->d_name, ops))
		{
			subdir = get_path(dir, cur->d_name);
			if (!opendir(subdir))
			{
				ft_memdel((void**)&subdir);
				continue ;
			}
			ft_strncpy(pass, subdir, 512);
			ft_memdel((void**)&subdir);
			recurse(pass, ops);
		}
	}
	//assert(ft_strncmp(dir, "//nfs/2018/j/jsanders/Day05", 27));
	if (dir)
		ft_memdel((void**)&cur);
	if (cur)
		ft_memdel((void**)&cur);
	if (dpntr)
		closedir(dpntr);
}
