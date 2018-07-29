/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/24 17:25:29 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/28 20:25:01 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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
	else if (S_ISSOCK(m))
		ft_putchar('s');
	else if (m & S_IFDIR)
		ft_putchar('d');
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

void	output_time(time_t mod)
{
	struct tm	*time;
	char		*month;
	char		*format;

	format = "%4s %2d %.2d:%.2d";
	time = localtime(&mod);
	month = get_month(time->tm_mon);
	ft_printf(format, month, time->tm_mday, time->tm_hour, time->tm_min);
	month = NULL;
}

void	output_name(const char *fn, mode_t m)
{
	char	buf[512];
	int		ret;

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
