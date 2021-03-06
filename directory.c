/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directory.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/24 17:30:57 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/28 20:36:15 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	get_files(t_list **dlst, char *path, t_ops *ops)
{
	struct dirent	*dp;
	DIR				*dpntr;
	int				blocks;
	char			full_path[512];

	if (!(S_ISDIR(get_stats(path).st_mode)))
		return (die(-2, path));
	if (!(dpntr = opendir(path)))
		return (die(-1, path));
	blocks = 0;
	while ((dp = readdir(dpntr)))
	{
		if (!(!ops->a && dp->d_name[0] == '.'))
		{
			ft_lstadd(dlst, ft_lstnew(dp->d_name, (ft_strlen(dp->d_name)) + 1));
			if (ops->l)
			{
				get_path(full_path, path, dp->d_name);
				blocks += get_blocks(full_path);
			}
		}
	}
	closedir(dpntr);
	return (blocks);
}

static void	sort_files(t_list **dlst, t_ops *ops, void **aux)
{
	if (!ops->t && !ops->r)
		ft_lstsort(*dlst, lex, aux, 2);
	else if (ops->r)
		ft_lstsort(*dlst, rev, aux, 2);
	else if (ops->t)
		ft_lstsort(*dlst, tim, aux, 2);
}

/*
**	given a directory, extracts filnames and prints relvant information in
**	propper order depending on the specified options
*/

void		output_dir(char *path, t_ops *ops)
{
	void			*aux[2];
	int				blocks;
	t_list			*dlst;

	if (ops->r_r && ft_strncmp(path, ".", ft_strlen(path)))
		ft_printf("\n%s\n", path);
	dlst = NULL;
	blocks = get_files(&dlst, path, ops);
	if (ops->l)
		ft_printf("total %d\n", blocks);
	aux[0] = (void*)ops;
	aux[1] = (void*)path;
	sort_files(&dlst, ops, aux);
	ft_lstforeach(dlst, output_stats, aux, 2);
	aux[0] = NULL;
	aux[1] = NULL;
	ft_lstdstry(&dlst, NULL);
}
