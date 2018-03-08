/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 20:05:30 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/08 22:23:47 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "ft_select.h"

void	clear_choices(size_t len)
{
	const char		*cmstr = tgetstr("cm", NULL);
	const char		*gotostr = tgoto(cmstr, 0, 0);

	tputs(gotostr, 1, &putcf);
	while (len--)
		outcap("dl");
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
		ft_putendl_fd(choices->title, STDIN_FILENO);
		if (choices->selected || csr->pos == idx)
			outcap("me");
		ft_putchar_fd('\r', STDIN_FILENO);
		choices = choices->next;
		idx++;
	}
}

void	return_res(t_list *res)
{
	while (res)
	{
		ft_putstr(res->content);
		res = res->next;
		if (res)
			ft_putchar(' ');
	}
}
