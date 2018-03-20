/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   navigation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 21:29:38 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/20 02:07:16 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

static void	mov_page(t_cursor *csr, int up)
{
	if (up)
	{
		if (csr->pos == csr->max - csr->ncols)
			csr->vscroll = csr->nlines * csr->ncols \
					* (csr->max / (csr->nlines * csr->ncols));
		else if (csr->pos < csr->vscroll)
			csr->vscroll -= csr->nlines * csr->ncols;
	}
	else
	{
		if (csr->pos == 0)
			csr->vscroll = 0;
		else if (csr->pos + 1 > csr->vscroll + csr->nlines * csr->ncols)
			csr->vscroll += csr->nlines * csr->ncols;
	}
}

void		mov_up(t_choice **ch, t_cursor *csr)
{
	(void)ch;
	csr->pos = (csr->pos > 0) ? csr->pos - csr->ncols : csr->max - csr->ncols;
	if (csr->nlines != csr->ws.ws_row - 1)
		return ;
	mov_page(csr, TRUE);
}

void		mov_down(t_choice **ch, t_cursor *csr)
{
	(void)ch;
	csr->pos = (csr->pos < csr->max - csr->ncols) ? csr->pos + csr->ncols : 0;
	if (csr->nlines != csr->ws.ws_row - 1)
		return ;
	mov_page(csr, FALSE);
}

void		mov_left(t_choice **ch, t_cursor *csr)
{
	(void)ch;
	if (csr->ncols < 2)
		return ;
	csr->pos = (csr->pos > 0) ? csr->pos - 1 : csr->max - 1;
	mov_page(csr, TRUE);
}

void		mov_right(t_choice **ch, t_cursor *csr)
{
	(void)ch;
	if (csr->ncols < 2)
		return ;
	csr->pos = (csr->pos < csr->max - 1) ? csr->pos + 1 : 0;
	mov_page(csr, FALSE);
}
