/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 20:05:30 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/14 22:01:48 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

void		clear_choices(t_cursor *csr)
{
	const char		*cmstr = tgetstr("cm", NULL);
	char			*gotostr;
	size_t			len;

	len = csr->nlines;
	gotostr = tgoto(cmstr, 0, len);
	tputs(gotostr, 1, &putcf);
	while (len--)
	{
		outcap("up");
		outcap("ce");
	}
}

static void print_padding(t_choice *ch, t_cursor *csr)
{
	char	*buff;
	size_t	padlen;

	if (!ch || !ch->title)
		padlen = FT_PAD_NB;
	else
		padlen = csr->mlen - ft_strlen(ch->title) + FT_PAD_NB;
	if (!(buff = ft_strnew(padlen)))
		return ;
	ft_memset(buff, ' ', padlen);
	ft_putstr_fd(buff, FT_OUT_FD);
	free(buff);
}

void		print_with_csr(t_choice *choices, t_cursor *csr)
{
	unsigned int	idx;
	unsigned int	cline;
	unsigned int	ccol;

	idx = 0;
	cline = 0;
	clear_choices(csr);
	while (cline < csr->nlines)
	{
		ccol = 0;
		while (ccol < csr->ncols)
		{
			if (ccol > 0)
				print_padding(choices->prev, csr);
			if (csr->pos == idx)
				outcap("us");
			if (choices->selected)
				outcap("mr");
			ft_putstr_fd(choices->title, FT_OUT_FD);
			if (choices->selected || csr->pos == idx)
				outcap("me");
			if (!(choices = choices->next))
				break ;
			idx++;
			ccol++;
		}
		cline++;
		ft_putstr_fd("\n\r", FT_OUT_FD);
	}
}

void		return_res(t_choice *choices)
{
	int				first;

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
	if (!first)
		ft_putchar('\n');
}
