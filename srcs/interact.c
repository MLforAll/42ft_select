/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 22:55:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/13 21:54:42 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

static t_choice	**lchs;
static t_cursor	*lcsr;

static void	interact(char *buff, t_tkeys *kcmps)
{
	t_choice		*tmp;
	unsigned int	idx;

	if (!lchs || !kcmps)
		return ;
	if (ft_strequ(buff, kcmps->upk) && lcsr->pos > 0)
		lcsr->pos--;
	if (ft_strequ(buff, kcmps->downk) && lcsr->pos < lcsr->max)
		lcsr->pos++;
	if (ft_strequ(buff, " "))
		switch_selected(*lchs, lcsr->pos);
	if (ft_strequ(buff, kcmps->delk) || ft_strequ(buff, kcmps->bsk))
	{
		tmp = *lchs;
		idx = lcsr->pos;
		while (idx-- && tmp)
			tmp = tmp->next;
		ft_chdelone(lchs, tmp);
	}
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
	print_with_csr(*lchs, lcsr);
}

void		chk_keys(t_choice **choices, t_cursor *csr)
{
	t_tkeys			kcmps;
	char			buff[5];

	if (!choices || !csr)
		return ;
	lchs = choices;
	lcsr = csr;
	fill_kcmps(&kcmps);
	print_with_csr(*choices, csr);
	ft_bzero(buff, sizeof(buff));
	while (read(FT_OUT_FD, buff, 4) > 0)
	{
		if (ft_strequ(buff, "\n"))
			break ;
		interact(buff, &kcmps);
		print_with_csr(*choices, csr);
		ft_bzero(buff, sizeof(buff));
	}
}
