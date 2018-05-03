/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 18:55:49 by pstringe          #+#    #+#             */
/*   Updated: 2018/05/03 11:36:05 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

typedef struct	s_dir
{
	char			*p;
	DIR				*d;
	t_list			*f;
}				t_dir;

typedef struct	s_ops
{
	int	l;
	int	a;
	int	t;
	int	r;
	int	R;
}				t_ops;

void 	init_options(t_ops **ops)
{
	t_ops *o;

	o = ft_memalloc(sizeof(t_ops));
	o->l = 0;
	o->a = 0;
	o->t = 0;
	o->r = 0;
	o->R = 0;
	*ops = o;
}

int 	parse_options(char **args, int argn, t_ops *ops)
{
	int	i;
	int	j;
	
	if (argn == 1)
		return (-1);
	i = 0;
	while (args[++i][0] == '-' && i < argn)
	{
		j = 0;
		while (args[i][++j])
		{
			if (args[i][j] == 'l')
				ops->l = 1;
			else if (args[i][j] == 'a')
				ops->a = 1;
			else if (args[i][j] == 't')
				ops->t = 1;
			else if (args[i][j] == 'r')
				ops->r = 1;
			else if (args[i][j] == 'R')
				ops->R = 1;
			else
				return (0);
		}
	}
	return (i);
}

void	display(t_dir *dir)
{
	t_list *files;

	ft_putendl(dir->p);
	ft_putendl("");
	files = dir->f;
	while (files)
	{
		ft_putendl((char*)files->content);
		files = files->next;
	}
}

char 	*get_path(t_dir *parent, char *name)
{
	char *tmp;
	char *new;

	tmp = ft_strjoin(parent->p, "/");
	new = ft_strjoin(tmp, name);
	ft_memdel((void**)&tmp);
	return (new);
}

t_dir	*get_dir(char *path)
{
	t_dir			*dir;
	DIR				*dp;

	if (!(dp = opendir(path)))
		return (NULL);
	dir = ft_memalloc(sizeof(t_dir));
	dir->d = dp;
	dir->p = path;
	return (dir);
}

t_dir		*is_dir(char *name, t_dir *parent, t_ops *ops)
{
	if (!strncmp(name, "..", ft_strlen(name)) || !strncmp(name, ".", ft_strlen(name)))
		return (NULL);
	if (name[0] == '.' && !ops->a)
		return (NULL);
	return (ops->R ? get_dir(get_path(parent, name)) : NULL);
}

void 	recurse(t_dir *dir, t_queue *dirs, t_ops *ops)
{
	struct dirent 	*cur;
	t_dir			*sub_dir;

	while ((cur = readdir(dir->d)))
	{
		ft_lstadd(&dir->f, ft_lstnew(cur->d_name, sizeof(struct dirent)));
		if ((sub_dir = is_dir(cur->d_name, dir, ops)))
		{
			ft_enqueue(dirs, sub_dir, sizeof(t_dir));
			recurse(sub_dir, dirs, ops);
		}
	}
	closedir(dir->d);
}

void	ft_ls(t_ops *ops, char *path)
{
	t_queue		*dirs;
	t_dir		*cur_dir;
	
	cur_dir = get_dir(path);
	dirs = ft_queuenw(cur_dir, sizeof(t_dir));
	recurse(cur_dir, dirs, ops);
	while ((cur_dir = ft_dequeue(dirs)))
		display(cur_dir);
}

int	main(int argc, char **argv)
{
	t_ops	*ops;
	int		i;

	init_options(&ops);
	if (!(i = parse_options(argv, argc, ops)))
		return (-1);
	while (i < argc)
		ft_ls(ops, argv[i++]);
	return (0);
}
