/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/24 17:56:20 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/28 19:56:07 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		lex(void *a, void *b, void **aux, int len)
{
	const char	*s1;
	const char	*s2;
	size_t		l1;
	size_t		l2;

	if (!aux || !len)
		aux = NULL;
	s1 = (const char*)a;
	s2 = (const char*)b;
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	return (ft_strncmp(s1, s2, (l1 >= l2 ? l1 : l2)));
}

int		tim(void *a, void *b, void **aux, int len)
{
	char	*s1;
	char	*s2;
	time_t	t1;
	time_t	t2;
	char	p[512];

	if (!aux || !len)
		aux = NULL;
	s1 = (char*)a;
	s2 = (char*)b;
	get_path(p, (char*)aux[1], s1);
	t1 = get_stats(p).st_mtime;
	get_path(p, (char*)aux[1], s2);
	t2 = get_stats(p).st_mtime;
	return (t1 < t2);
}

int		rev(void *a, void *b, void **aux, int len)
{
	const char	*s1;
	const char	*s2;
	size_t		l1;
	size_t		l2;

	if (!aux || !len)
		aux = NULL;
	s1 = (const char*)a;
	s2 = (const char*)b;
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	return (ft_strncmp(s2, s1, (l1 >= l2 ? l1 : l2)));
}
