/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pstringe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/30 18:55:49 by pstringe          #+#    #+#             */
/*   Updated: 2018/07/28 19:09:12 by pstringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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
				return (die(-3, &args[i][j]));
		}
	}
	return (i);
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
