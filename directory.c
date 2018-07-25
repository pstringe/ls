/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/24 17:30:57 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/24 18:48:14 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	get_files(t_list **dlst, char *path, t_ops *ops)
{
	struct dirent	*dp;
	DIR				*dpntr;	
	int 			blocks;
	char			*file_path;

	if (!(dpntr = opendir(path)))
		return (0);
	dp = readdir(dpntr);
	blocks = 0;
	while (dp)
	{
		if (!(!ops->a && dp->d_name[0] == '.'))
		{
			ft_lstadd(dlst, ft_lstnew(dp->d_name, (ft_strlen(dp->d_name) + 1)));
			if (ops->l)
			{
				file_path = get_path(path, dp->d_name);
				blocks += get_blocks(file_path);
				ft_memdel((void**)&file_path);
			}
		}
		dp = readdir(dpntr);
	}
	closedir(dpntr);
	return (blocks);
}

static void	break_func()
{
	return ;
}

/*
**	given a directory, extracts filnames and prints relvant information in propper order
**	depending on the specified options
*/
void	output_dir(char *path, t_ops *ops)
{
	void			*aux[2];	
	int				blocks;
	t_list			*dlst;

	if (!ft_strncmp(path, "//nfs/2018/j/jsanders/Day05", 27))
		break_func();
	if (ops->R && ft_strncmp(path, ".", ft_strlen(path)))
		ft_printf("\n\n%s\n", path);
	dlst = NULL;
	blocks = get_files(&dlst, path, ops);
	if (ops->l)
		ft_printf("total %d\n", blocks);
	aux[0] = (void*)ops;
	aux[1] = (void*)path;
	if (!ops->t && !ops->r)
		ft_lstsort(dlst, lex, aux, 2);
	else if (ops->r)
		ft_lstsort(dlst, rev, aux, 2);
	else if (ops->t)
		ft_lstsort(dlst, tim, aux, 2);
	ft_lstforeach(dlst, output_stats, aux, 2);
	aux[0] = NULL;
	aux[1] = NULL;
	ft_lstdstry(&dlst, NULL);
}
