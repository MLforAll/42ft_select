/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 20:05:30 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/19 20:36:30 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "ft_select.h"

void		clear_choices(t_cursor *csr)
{
	size_t			len;

	len = (csr->nlines > csr->ws.ws_row) ? csr->ws.ws_row : csr->nlines;
	movcap(0, len);
	while (len--)
	{
		outcap("up");
		outcap("ce");
	}
}

static void	print_padding(t_choice *ch, t_cursor *csr)
{
	char			*padbuff;
	size_t			padlen;

	if (!ch->prev || !ch->prev->title)
		padlen = FT_PAD_NB;
	else
		padlen = csr->mlen - ch->prev->titlelen + FT_PAD_NB;
	if (!(padbuff = ft_strnew(padlen)))
		return ;
	ft_memset(padbuff, ' ', padlen);
	ft_putstr_fd(padbuff, FT_OUT_FD);
	free(padbuff);
}

static int	print_elem(t_choice *ch, t_cursor *csr, unsigned ccol, unsigned idx)
{
	size_t			len;
	unsigned int	off;
	int				scroll;

	if (!ch || !csr)
		return (FALSE);
	if (ccol > 0)
		print_padding(ch, csr);
	if (csr->pos == idx)
		outcap("us");
	if (ch->selected)
		outcap("mr");
	scroll = (csr->pos == idx && ch->titlelen > csr->ws.ws_col);
	len = (ch->titlelen > csr->ws.ws_col) ? csr->ws.ws_col : ch->titlelen;
	off = (scroll) ? csr->scroll_off : 0;
	write(FT_OUT_FD, ch->title + off,
		(off + len > ch->titlelen) ? ch->titlelen - off : len);
	if (scroll)
		csr->scroll_off = (csr->scroll_off + 1 > ch->titlelen) ? 0
						: csr->scroll_off + 1;
	if (ch->selected || csr->pos == idx)
		outcap("me");
	return (scroll);
}

void		print_with_csr(t_choice *choices, t_cursor *csr)
{
	unsigned int	idx;
	unsigned int	cline;
	unsigned int	ccol;
	int				scroll_sw;

	idx = csr->vscroll;
	cline = 0;
	if (idx > 0)
		choices = ft_chgetidx(choices, idx);
	scroll_sw = FALSE;
	while (choices && cline < csr->nlines)
	{
		ccol = 0;
		while (choices && ccol < csr->ncols)
		{
			if (print_elem(choices, csr, ccol, idx))
				scroll_sw = TRUE;
			choices = choices->next;
			idx++;
			ccol++;
		}
		cline++;
		ft_putstr_fd("\n\r", FT_OUT_FD);
	}
	set_read_timeout((scroll_sw) ? 2 : 0, NULL);
}

/*
** set_window_prop
**
** t_cursor*	destination struct
*/

void		set_window_prop(t_cursor *dest)
{
	ft_bzero(&dest->ws, sizeof(struct winsize));
	ioctl(FT_OUT_FD, TIOCGWINSZ, &dest->ws);
	if (dest->max < dest->ws.ws_row || dest->mlen > dest->ws.ws_col
		|| dest->mlen <= 0)
		dest->ncols = 1;
	else
		dest->ncols = dest->ws.ws_col / (dest->mlen + FT_PAD_NB);
	if (dest->ncols <= 0)
		dest->ncols = 1;
	dest->nlines = dest->max / dest->ncols + (dest->max % dest->ncols);
	if (dest->nlines > dest->ws.ws_row)
		dest->nlines = dest->ws.ws_row - 1;
	dest->vscroll = 0;
}
