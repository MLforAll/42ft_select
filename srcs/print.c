/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 20:05:30 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/07 21:42:10 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "ft_select.h"

void	print_with_csr(t_list *choices, t_cursor *csr)
{
	unsigned int	idx;
	const char		*cmstr = tgetstr("cm", NULL);
	const char		*gotostr = tgoto(cmstr, 0, 0);

	tputs(gotostr, 1, &putcf);
	idx = 0;
	while (choices)
	{
		if (csr->pos == idx)
			outcap("us");
		ft_putendl(choices->content);
		if (csr->pos == idx)
			outcap("ue");
		ft_putchar('\r');
		choices = choices->next;
		idx++;
	}
}
