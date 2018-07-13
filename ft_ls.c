/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 18:55:49 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/12 18:46:03 by pstringe         ###   ########.fr       */
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

	if (!aux || !len)
		aux = NULL;
	s1 = (char*)a;
	s2 = (char*)b;
	t1 = get_stats(get_path((char*)aux[1], s1)).st_mtime;
	t2 = get_stats(get_path((char*)aux[1], s2)).st_mtime;
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
    ft_putchar((m & S_IFDIR) ? 'd' : '-');
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
	//time_t		cur;
	struct tm	*time;
	
	time = localtime(&mod);
	ft_printf("%4s %2d %.2d:%.2d", get_month(time->tm_mon), time->tm_mday, time->tm_hour, time->tm_min);

}

void	output_name(const char *fn, mode_t m)
{
	char buf[512];
	int count;

	if (m & S_IFLNK || m & S_IFDIR)
	{
		count = readlink(fn, buf, 512);
		if (count >= 0)
		{
			buf[count] = '\0';
			ft_printf(" %s -> %s\n", fn, buf);
			return ;
		}
		else
			ft_printf(" %s\n", fn);
	}
}

void	output_stats(char *file, void **aux)
{
	struct stat		stats;
	t_ops			*ops;
	char 			*path;

	//char			*tmp;

	ops = (t_ops*)*aux;
	path = (char*)get_path(aux[1], file);
	if (!ops->l)
		ft_printf("%s\n", file);
	else
	{
		stats = get_stats(path);
		output_type(stats.st_mode);
		output_permissions(stats.st_mode);
		ft_printf(" %d", stats.st_nlink);
		ft_printf("%10s %10s", getpwuid(stats.st_uid)->pw_name, getgrgid(stats.st_gid)->gr_name);
		ft_printf("%10ld ", (long)stats.st_size);
		output_time(stats.st_mtime);
		output_name(file, stats.st_mode);
	}
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
