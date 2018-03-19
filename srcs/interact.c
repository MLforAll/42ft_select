/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 22:55:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/19 04:06:12 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

static t_choice	**g_chs;
static t_cursor	*g_csr;

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
}

static void	select_curr_elem(t_choice **ch, t_cursor *csr)
{
	t_choice		*bw;

	if (!ch || !csr)
		return ;
	if (!(bw = ft_chgetidx(*ch, csr->pos)))
		return ;
	bw->selected = !bw->selected;
	mov_down(ch, csr);
}

static int	interact(char *buff, t_tkeys *kcmps, t_choice **ch, t_cursor *csr)
{
	const char		*kbuffs[] = {kcmps->upk, kcmps->downk, " ",
								kcmps->delk, kcmps->bsk, NULL};
	static void		(*kfuncs[])(t_choice **, t_cursor *) = {&mov_up, 
								&mov_down, &select_curr_elem,
								&delete_curr_elem, &delete_curr_elem, NULL};
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

void		redraw_hdl(int sigc)
{
	if (sigc == SIGCONT)
		init_terminal();
	else
		outcap("cl");
	set_window_prop(g_csr);
	print_with_csr(*g_chs, g_csr);
}

int			chk_keys(t_choice **choices, t_cursor *csr, t_tkeys *kcmps)
{
	char			buff[5];
	ssize_t			rb;

	if (!choices || !csr)
		return (FALSE);
	g_chs = choices;
	g_csr = csr;
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
