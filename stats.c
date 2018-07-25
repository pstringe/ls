/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/24 17:33:24 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/24 18:01:37 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

struct stat get_stats(const char *path)
{
	struct stat	stats;

	lstat(path, &stats);
	return (stats);
}

int		get_blocks(char *path)
{
	struct stat	f;

	stat(path, &f);
	return (f.st_blocks);
}

