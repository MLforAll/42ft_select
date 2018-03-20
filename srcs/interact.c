/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 22:55:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/20 01:00:01 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "ft_select.h"

static void	delete_curr_elem(t_choice **ch, t_cursor *csr)
{
	t_choice		*tmp;

	if (!ch || !csr)
		return ;
	if (!(tmp = ft_chgetidx(*ch, csr->pos)))
		return ;
	ft_chdelone(ch, tmp);
	if (csr->pos > 0)
	{
		csr->pos--;
		csr->max--;
	}
	set_window_prop(csr);
}

static void	select_curr_elem(t_choice **ch, t_cursor *csr)
{
	t_choice		*bw;

	if (!ch || !csr)
		return ;
	if (!(bw = ft_chgetidx(*ch, csr->pos)))
		return ;
	bw->selected = !bw->selected;
	if (csr->ncols > 1)
		mov_right(ch, csr);
	else
		mov_down(ch, csr);
}

static int	interact(char *buff, t_tkeys *kcmps, t_choice **ch, t_cursor *csr)
{
	const char		*kbuffs[] = {kcmps->upk, kcmps->downk,
								kcmps->leftk, kcmps->rightk, " ",
								kcmps->delk, kcmps->bsk, NULL};
	static void		(*kfuncs[])(t_choice **, t_cursor *) = {&mov_up,
								&mov_down, &mov_left, &mov_right,
								&select_curr_elem, &delete_curr_elem,
								&delete_curr_elem, NULL};
	unsigned int	idx;

	if (!ch || !kcmps)
		return (FALSE);
	csr->scroll_off = 0;
	idx = 0;
	while (kbuffs[idx])
	{
		if (ft_strequ(buff, kbuffs[idx]))
		{
			(kfuncs[idx])(ch, csr);
			return (TRUE);
		}
		idx++;
	}
	return (FALSE);
}

void		redraw_hdl(unsigned long long sigc)
{
	static t_choice	**ch = NULL;
	static t_cursor	*csr = NULL;

	if (!ch)
		ch = (t_choice**)sigc;
	else if (!csr)
		csr = (t_cursor*)sigc;
	if (!ch || !csr || sigc > INT_MAX)
		return ;
	if (sigc == SIGCONT)
	{
		set_signals();
		init_restore_terminal(YES);
		init_display(csr);
	}
	else
	{
		outcap("cl");
		set_window_prop(csr);
	}
	print_with_csr(*ch, csr);
}

int			chk_keys(t_choice **choices, t_cursor *csr, t_tkeys *kcmps)
{
	char			buff[5];
	ssize_t			rb;

	if (!choices || !csr)
		return (FALSE);
	print_with_csr(*choices, csr);
	ft_bzero(buff, sizeof(buff));
	while ((rb = read(FT_OUT_FD, buff, 4)) != -1)
	{
		if (rb > 0 && (ft_strequ(buff, "\n") || ft_strasciieq(buff, 4)))
			break ;
		if ((rb > 0 && ft_strasciieq(buff, 27)) || !*choices)
			return (FALSE);
		if (rb == 0 || interact(buff, kcmps, choices, csr))
		{
			clear_choices(csr);
			print_with_csr(*choices, csr);
		}
		(rb > 0) ? ft_bzero(buff, sizeof(buff)) : 0;
	}
	return (TRUE);
}
