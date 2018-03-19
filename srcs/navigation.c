/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   navigation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 21:29:38 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/19 03:27:07 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

void	mov_up(t_choice **ch, t_cursor *csr)
{
	(void)ch;
	csr->pos = (csr->pos > 0) ? csr->pos - 1 : csr->max - 1;
	if (csr->nlines != csr->ws.ws_row - 1)
		return ;
	if (csr->pos == csr->max - 1)
		csr->vscroll = csr->nlines * csr->ncols \
					* (csr->max / (csr->nlines * csr->ncols));
	else if ((csr->pos + 1) % (csr->nlines * csr->ncols) == 0 && csr->pos > 0)
		csr->vscroll -= csr->nlines * csr->ncols;
}

void	mov_down(t_choice **ch, t_cursor *csr)
{
	(void)ch;
	csr->pos = (csr->pos < csr->max - 1) ? csr->pos + 1 : 0;
	if (csr->nlines != csr->ws.ws_row - 1)
		return ;
	if (csr->pos == 0)
		csr->vscroll = 0;
	else if (csr->pos % (csr->nlines * csr->ncols) == 0 && csr->pos > 0)
		csr->vscroll += csr->nlines * csr->ncols;
}

void	mov_left(t_choice **ch, t_cursor *csr)
{
	(void)ch;
	(void)csr;
}

void	mov_right(t_choice **ch, t_cursor *csr)
{
	(void)ch;
	(void)csr;
}
