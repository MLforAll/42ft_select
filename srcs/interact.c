/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 22:55:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/15 23:17:26 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

static t_choice	**g_chs;
static t_cursor	*g_csr;

static int	interact(char *buff, t_tkeys *kcmps, t_cursor *csr)
{
	t_choice		*tmp;
	unsigned int	idx;

	if (!g_chs || !kcmps)
		return (FALSE);
	csr->scroll_off = 0;
	if (ft_strequ(buff, kcmps->upk))
		csr->pos = (csr->pos > 0) ? csr->pos - 1 : csr->max - 1;
	else if (ft_strequ(buff, kcmps->downk))
		csr->pos = (csr->pos < csr->max - 1) ? csr->pos + 1 : 0;
	else if (ft_strequ(buff, " "))
	{
		switch_selected(*g_chs, csr->pos);
		csr->pos = (csr->pos < csr->max - 1) ? csr->pos + 1 : 0;
	}
	else if (ft_strequ(buff, kcmps->delk) || ft_strequ(buff, kcmps->bsk))
	{
		tmp = *g_chs;
		idx = csr->pos;
		while (idx-- && tmp)
			tmp = tmp->next;
		ft_chdelone(g_chs, tmp);
	}
	else
		return (FALSE);
	return (TRUE);
}

static void	fill_kcmps(t_tkeys *dest)
{
	dest->rightk = tgetstr("kr", NULL);
	dest->upk = tgetstr("ku", NULL);
	dest->downk = tgetstr("kd", NULL);
	dest->leftk = tgetstr("kl", NULL);
	dest->delk = tgetstr("kD", NULL);
	(dest->bsk)[0] = 127;
	(dest->bsk)[1] = '\0';
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

void		chk_keys(t_choice **choices, t_cursor *csr)
{
	t_tkeys			kcmps;
	char			buff[5];
	ssize_t			rb;

	if (!choices || !csr)
		return ;
	g_chs = choices;
	g_csr = csr;
	fill_kcmps(&kcmps);
	print_with_csr(*choices, csr);
	ft_bzero(buff, sizeof(buff));
	while ((rb = read(FT_OUT_FD, buff, 4)) != -1)
	{
		if (rb > 0 && ft_strequ(buff, "\n"))
			break ;
		if (rb == 0 || interact(buff, &kcmps, csr))
		{
			clear_choices(csr);
			print_with_csr(*choices, csr);
		}
		if (rb > 0)
			ft_bzero(buff, sizeof(buff));
	}
}
