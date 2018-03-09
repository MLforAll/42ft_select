/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 20:05:30 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/09 19:58:59 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "ft_select.h"

void	clear_choices(size_t len)
{
	const char		*cmstr = tgetstr("cm", NULL);
	const char		*gotostr = tgoto(cmstr, 0, len);

	tputs(gotostr, 1, &putcf);
	while (len--)
	{
		outcap("ce");
		outcap("up");
	}
}

void	print_with_csr(t_choice *choices, t_cursor *csr)
{
	unsigned int	idx;

	idx = 0;
	clear_choices(csr->max);
	while (choices)
	{
		if (csr->pos == idx)
			outcap("us");
		if (choices->selected)
			outcap("mr");
		ft_putendl_fd(choices->title, FT_OUT_FD);
		if (choices->selected || csr->pos == idx)
			outcap("me");
		ft_putchar_fd('\r', FT_OUT_FD);
		choices = choices->next;
		idx++;
	}
}

void	return_res(t_choice *choices)
{
	int			first;

	first = YES;
	while (choices)
	{
		if (choices->selected)
		{
			if (!first)
				ft_putchar(' ');
			else
				first = NO;
			ft_putstr(choices->title);
		}
		choices = choices->next;
	}
	ft_putchar('\n');
}
