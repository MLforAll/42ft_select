/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 20:05:30 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/16 02:42:55 by kdumarai         ###   ########.fr       */
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

	len = (csr->nlines > csr->ws.ws_row) ? csr->ws.ws_row : csr->nlines;
	gotostr = tgoto(cmstr, 0, len);
	tputs(gotostr, 1, &putcf);
	while (len--)
	{
		outcap("up");
		outcap("ce");
	}
}

static void	print_elem(t_choice *ch, t_cursor *csr, unsigned ccol, unsigned idx)
{
	char			*buff;
	size_t			padlen;
	size_t			len;
	unsigned int	off;
	int				scroll;

	if (!ch || !csr)
		return ;
	if (ccol > 0)
	{
		if (!ch->prev || !ch->prev->title)
			padlen = FT_PAD_NB;
		else
			padlen = csr->mlen - ch->prev->titlelen + FT_PAD_NB;
		if (!(buff = ft_strnew(padlen)))
			return ;
		ft_memset(buff, ' ', padlen);
		ft_putstr_fd(buff, FT_OUT_FD);
		free(buff);
	}
	if (csr->pos == idx)
		outcap("us");
	if (ch->selected)
		outcap("mr");
	scroll = (csr->pos == idx && ch->titlelen > csr->ws.ws_col);
	len = (ch->titlelen > csr->ws.ws_col) ? csr->ws.ws_col : ch->titlelen;
	off = (scroll) ? csr->scroll_off : 0;
	write(FT_OUT_FD, ch->title + off, (off + len > ch->titlelen) ? ch->titlelen - off : len);
	if (scroll)
		csr->scroll_off = csr->scroll_off + 1 > ch->titlelen ? 0 : csr->scroll_off + 1;
	if (ch->selected || csr->pos == idx)
		outcap("me");
}

void		print_with_csr(t_choice *choices, t_cursor *csr)
{
	unsigned int	idx;
	unsigned int	cline;
	unsigned int	ccol;

	idx = 0;
	cline = 0;
	unsigned int	next = csr->vscroll;
	while (choices && next--)
	{
		choices = choices->next;
		idx++;
	}
	while (choices && cline < csr->nlines)
	{
		ccol = 0;
		while (ccol < csr->ncols)
		{
			print_elem(choices, csr, ccol, idx);
			if (!choices || !(choices = choices->next))
				break ;
			idx++;
			ccol++;
		}
		cline++;
		ft_putstr_fd("\n\r", FT_OUT_FD);
	}
}
