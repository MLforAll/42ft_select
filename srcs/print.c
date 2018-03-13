/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 20:05:30 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/13 21:58:17 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "ft_select.h"

void		clear_choices(size_t len)
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
	struct winsize	ws;
	size_t			ncols;
	size_t			nlines;
	unsigned int	ccol;

	idx = 0;
	ioctl(FT_OUT_FD, TIOCGWINSZ, &ws);
	ncols = ws.ws_col / csr->mlen;
	if (csr->max < ws.ws_row)
		ncols = 1;
	nlines = csr->max / ncols;
	clear_choices(csr->max);
	while (nlines--)
	{
		ccol = 0;
		while (ccol < ncols)
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
