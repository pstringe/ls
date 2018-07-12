/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 18:55:49 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/12 15:10:23 by pstringe         ###   ########.fr       */
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

int		lex(void *a, void *b, void **aux, int len)
{
	const char 	*s1;
	const char 	*s2;
	size_t		l1;
	size_t		l2;

	if (!aux || !len)
		aux = NULL;
	s1 = (const char*)a;
	s2 = (const char*)b;
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	return(ft_strncmp(s1, s2, (l1 >= l2 ? l1 : l2)));
}

struct stat get_stats(const char *path, const char *file)
{
	struct stat	stats;
	char		*tmp;

	tmp = ft_strjoin(path, file);
	lstat(tmp, &stats);
	return (stats);
}

int		tim(void *a, void *b, void **aux, int len)
{
	
	const char 	*s1;
	const char 	*s2;
	time_t		t1;
	time_t		t2;

	if (!aux || !len)
		aux = NULL;
	s1 = (const char*)a;
	s2 = (const char*)b;
	t1 = get_stats(NULL, s1).st_mtime;
	t2 = get_stats(NULL, s2).st_mtime;
	return(t1 < t2);
}

int		rev(void *a, void *b, void **aux, int len)
{
	const char 	*s1;
	const char 	*s2;
	size_t		l1;
	size_t		l2;

	if (!aux || !len)
		aux = NULL;
	s1 = (const char*)a;
	s2 = (const char*)b;
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	return(ft_strncmp(s2, s1, (l1 >= l2 ? l1 : l2)));
}

void	output_type(mode_t m)
{
	if (m == S_IFREG)
		ft_putchar('-');
	else if (m == S_IFDIR)
		ft_putchar('d');
	else if (m == S_IFLNK)
		ft_putchar('l');
	else if (m == S_IFCHR)
		ft_putchar('c');
	else if (m == S_IFBLK)
		ft_putchar('b');
	else if (m == S_IFSOCK)
		ft_putchar('s');
	else if (m == S_IFIFO)
		ft_putchar('s');
}

void	output_permissions(mode_t m)
{
    ft_putchar((m & S_IRUSR) ? 'r' : '-');
    ft_putchar((m & S_IWUSR) ? 'w' : '-');
    ft_putchar((m & S_IXUSR) ? 'x' : '-');
    ft_putchar((m & S_IRGRP) ? 'r' : '-');
    ft_putchar((m & S_IWGRP) ? 'w' : '-');
    ft_putchar((m & S_IXGRP) ? 'x' : '-');
    ft_putchar((m & S_IROTH) ? 'r' : '-');
    ft_putchar((m & S_IWOTH) ? 'w' : '-');
    ft_putchar((m & S_IXOTH) ? 'x' : '-');
}

void	output_time(time_t mod)
{
	//time_t		cur;
	struct tm	*time;
	
	time = localtime(&mod);
	ft_printf("%5d%2d %.2d:%.2d", time->tm_mon, time->tm_mday, time->tm_hour, time->tm_min);

}

void	output_name(const char *fn, mode_t mode)
{
	char buf[512];
	int count;

	if (mode & S_IFLNK)
	{
		count = readlink(fn, buf, 512);
		if (count >= 0)
		{
			buf[count] = '\0';
			ft_printf(" %s -> %s\n", fn, buf);
		}
		return ;
	}
	else
		ft_printf(" %s\n", fn);
}

void	output_stats(char *file, void **aux)
{
	struct stat		stats;
	t_ops			*ops;
	char 			*path;
	struct passwd	*tmp_pw;

	//char			*tmp;

	ops = (t_ops*)*aux;
	path = (char*)aux + 1;
	if (!ops->l)
		ft_printf("%s\n", file);
	stats = get_stats(path, file);
	output_type(stats.st_mode);
	output_permissions(stats.st_mode);
	ft_printf(" %d", stats.st_nlink);
	tmp_pw = getpwuid(stats.st_uid);
	printf("%10s", tmp_pw->pw_name);
	ft_printf("%10s", getgrgid(stats.st_gid)->gr_name);
	ft_printf("%10ld ", (long)stats.st_size);
	output_time(stats.st_mtime);
	output_name(file, stats.st_mode);
}

void	output_dir(char *path, t_ops *ops)
{
	DIR*	dpntr;	
	struct 	dirent *dp;
	void	*aux[2];	
	t_list 	*dlst;

	dpntr = opendir(path);
	dp = readdir(dpntr);
	dlst = NULL;
	while (dp)
	{
		if (!(!ops->a && dp->d_name[0] =='.'))
			ft_lstadd(&dlst, ft_lstnew(dp->d_name, ft_strlen(dp->d_name)));
		dp = readdir(dpntr);
	}
	aux[0] = (void*)ops;
	aux[1] = (void*)path;
	if (!ops->t && !ops->r)
		ft_lstsort(dlst, lex, aux, 2);
	else if (ops->r)
		ft_lstsort(dlst, rev, aux, 2);
	else if (ops->t)
		ft_lstsort(dlst, tim, aux, 2);
	ft_lstforeach(dlst, output_stats, aux, 2);
	closedir(dpntr);
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

/*
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
*/

void	ft_ls(t_ops *ops, char **argv, int argc, int idx)
{
	int 		no_of_dirs;
	int			i;
	/*
	t_queue		*dirs;
	t_dir		*cur_dir;
	
	cur_dir = get_dir(path);
	dirs = ft_queuenw(cur_dir, sizeof(t_dir));
	recurse(cur_dir, dirs, ops);
	while ((cur_dir = ft_dequeue(dirs)))
		display(cur_dir);
	*/
	no_of_dirs = argc - (idx < 0 ? 1 : idx);
	if (!ops->R && !no_of_dirs)
		output_dir(".", ops);
	i = idx - 1;
	while (no_of_dirs--)
		output_dir(argv[++i], ops);
	/*
	if (ops->R)
	{
		recurse(".", ops);
		return ;
	}
	*/
}

int	main(int argc, char **argv)
{
	t_ops	*ops;
	int		i;

	init_options(&ops);
	if (!(i = parse_options(argv, argc, ops)))
		return (-1);
	ft_ls(ops, argv, argc, i);
	return (0);
}
