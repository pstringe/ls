/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/24 17:25:29 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/25 14:14:33 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	output_type(char *path, mode_t m)
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

static void	output_permissions(mode_t m)
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

static void	output_time(time_t mod)
{
	struct tm	*time;
	char		*month;

	time = localtime(&mod);
	ft_printf("%4s %2d %.2d:%.2d", (month = get_month(time->tm_mon)), time->tm_mday, time->tm_hour, time->tm_min);
	month = NULL;
}

static void	output_name(const char *fn, mode_t m)
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
