/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 18:55:49 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/24 15:14:20 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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
	while (++i < argn && args[i][0] == '-')
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

char 	*get_path(char *parent, char *name)
{
	char *tmp;
	char *new;

	tmp = ft_strjoin(parent, "/");
	new = ft_strjoin(tmp, name);
	ft_memdel((void**)&tmp);
	return (new);
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

struct stat get_stats(const char *path)
{
	struct stat	stats;

	lstat(path, &stats);
	return (stats);
}

int		tim(void *a, void *b, void **aux, int len)
{
	
	char 	*s1;
	char 	*s2;
	time_t		t1;
	time_t		t2;
	char		*p;

	if (!aux || !len)
		aux = NULL;
	s1 = (char*)a;
	s2 = (char*)b;
	p = get_path((char*)aux[1], s1);
	t1 = get_stats(p).st_mtime;
	ft_memdel((void**)&p);
	p = get_path((char*)aux[1], s2);
	t2 = get_stats(p).st_mtime;
	ft_memdel((void**)&p);
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

void	output_type(char *path, mode_t m)
{
	char ret[512];

	if (S_ISREG(m))
		ft_putchar('-');
	else if (m & S_IFLNK && readlink(path, ret, 512) >= 0)
		ft_putchar('l');
	else if (S_ISCHR(m))
		ft_putchar('c');
	else if (S_ISBLK(m))
		ft_putchar('b');
	else if (m & S_IFDIR)
		ft_putchar('d');
	else if (m & S_IFSOCK)
		ft_putchar('s');
	else if (m & S_IFIFO)
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

char	*get_month(int d)
{
	char *m;

	if (d == 0)
		m = "Jan";
	else if (d == 1)
		m = "Feb";
	else if (d == 2)
		m = "Mar";
	else if (d == 3)
		m = "Apr";
	else if (d == 4)
		m = "May";
	else if (d == 5)
		m = "Jun";
	else if (d == 6)
		m = "Jul";
	else if (d == 7)
		m = "Aug";
	else if (d == 8)
		m = "Sept";
	else if (d == 9)
		m = "Oct";
	else if (d == 10)
		m = "Nov";
	else if (d == 11)
		m = "Dec";
	else
		m = NULL;
	return (m);
}

void	output_time(time_t mod)
{
	struct tm	*time;
	
	time = localtime(&mod);
	ft_printf("%4s %2d %.2d:%.2d", get_month(time->tm_mon), time->tm_mday, time->tm_hour, time->tm_min);

}

void	output_name(const char *fn, mode_t m)
{
	char buf[512];
	int ret;

	if (m & S_IFLNK)
	{
		ret = readlink(fn, buf, 512);
		if (ret >= 0)
		{
			buf[ret] = '\0';
			ft_printf(" %s -> %s\n", ft_strrchr(fn, '/') + 1, buf);
			ft_bzero(buf, ret);
			return ;
		}
		else
			ft_printf(" %s\n", ft_strrchr(fn, '/') + 1);
	}
	else
		ft_printf(" %s\n", ft_strrchr(fn, '/') + 1);
}

void	output_stats(char *file, void **aux)
{
	struct stat		stats;
	t_ops			*ops;
	char 			*path;
	char 			*pw;
	char			*gw;

	ops = (t_ops*)*aux;
	path = (char*)get_path((char*)aux[1], file);
	if (!ops->l)
		ft_printf("%s\n", file);
	else
	{
		stats = get_stats(path);
		pw = getpwuid(stats.st_uid) ? getpwuid(stats.st_uid)->pw_name : NULL;
		if (!pw)
			ft_printf("path: %s\nuid: %d\n", path, stats.st_uid);
		gw = getgrgid(stats.st_gid) ? getgrgid(stats.st_gid)->gr_name: NULL;
		output_type(path, stats.st_mode);
		output_permissions(stats.st_mode);
		ft_printf(" %d", stats.st_nlink);
		ft_printf("%10s %10s", pw, gw);
		if (stats.st_rdev)
			ft_printf("%3d, %3d ", major(stats.st_rdev), minor(stats.st_rdev));
		else
			ft_printf("%10lld ", stats.st_size);
		output_time(stats.st_mtime);
		output_name(path, stats.st_mode);
	}
	ft_memdel((void**)&path);
}

int		get_blocks(char *path)
{
	struct stat	f;

	stat(path, &f);
	return (f.st_blocks);
}

void	break_func()
{
	return ;
}
/*
**	given a directory, extracts filnames and prints relvant information in propper order
**	depending on the specified options
*/
void	output_dir(char *path, t_ops *ops)
{
	DIR				*dpntr;	
	struct dirent 	*dp;
	void			*aux[2];	
	t_list 			*dlst;
	int				blocks;
	
	if (!ft_strncmp(path, "//nfs/2018/j/jsanders/Day05", 27))
		break_func();
	if (!(dpntr = opendir(path)))
	{
		return ;
	}
	dp = readdir(dpntr);
	dlst = NULL;
	if (ops->R && ft_strncmp(path, ".", ft_strlen(path)))
		ft_printf("\n\n%s\n", path);
	blocks = 0;
	while (dp)
	{
		if (!(!ops->a && dp->d_name[0] == '.'))
		{
			ft_lstadd(&dlst, ft_lstnew(dp->d_name, (ft_strlen(dp->d_name) + 1)));
			if (ops->l)
			{
				aux[1] = (void*)get_path(path, dp->d_name);
				blocks += get_blocks((char*)aux[1]);
				ft_memdel((void**)&aux[1]);
			}
		}
		dp = readdir(dpntr);
	}
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
	closedir(dpntr);
}

int		can_output(char *file, t_ops *ops)
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

int		can_recurse(char *file, t_ops *ops)
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

void	ft_ls(t_ops *ops, char **argv, int argc, int idx)
{
	int 		no_of_dirs;
	int			i;

	no_of_dirs = argc - (idx < 0 ? 1 : idx);
	if (!ops->R && !no_of_dirs)
		output_dir(".", ops);
	if (ops->R && !no_of_dirs)
		recurse(".", ops);
	i = idx - 1;
	while (no_of_dirs-- > 0)
		if (!ops->R)
			output_dir(argv[++i], ops);
		else
			recurse(argv[++i], ops);
}

int	main(int argc, char **argv)
{
	t_ops	*ops;
	int		i;

	init_options(&ops);
	if (!(i = parse_options(argv, argc, ops)))
		return (-1);
	ft_ls(ops, argv, argc, i);
	ft_memdel((void**)&ops);
	return (0);
}
