/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 19:31:04 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/27 12:01:31 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H
# include "libft.h"
# include "ft_printf.h"
# include <stdio.h>
# include <stdarg.h>
# include <assert.h>
# include <sys/stat.h>
# include <time.h>
# include <pwd.h>
# include <sys/types.h>
# include <dirent.h>
# include <grp.h>
# include <uuid/uuid.h>

typedef struct	s_ops
{
	int	l;
	int	a;
	int	t;
	int	r;
	int	R;
}				t_ops;

/*
**	recurse
*/

void			recurse(char *dir, t_ops *ops);

/*
**	stats
*/

struct stat		get_stats(const char *path);
int				get_blocks(char *path);

/*
** output
*/

void			output_stats(char *file, void **aux);
void			output_dir(char *path, t_ops *ops);

/*
** help
*/

int				die(int c, char *path);
void 			get_path(char path[512], char *parent, char *name);
char			*get_month(int d);

/*
**	sort
*/

int				lex(void *a, void *b, void **aux, int len);
int				tim(void *a, void *b, void **aux, int len);
int				rev(void *a, void *b, void **aux, int len);

#endif
