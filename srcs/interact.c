/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 22:55:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/09 19:44:59 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

static void	interact(char *buff, t_tkeys *kcmps, t_choice **ch, t_cursor *csr)
{
	t_choice		*tmp;
	unsigned int	idx;

	if (!ch)
		return ;
	if (ft_strequ(buff, kcmps->upk) && csr->pos > 0)
		csr->pos--;
	if (ft_strequ(buff, kcmps->downk) && csr->pos < csr->max)
		csr->pos++;
	if (ft_strequ(buff, " "))
		switch_selected(*ch, csr->pos);
	if (ft_strequ(buff, kcmps->delk) || ft_strequ(buff, kcmps->bsk))
	{
		tmp = *ch;
		idx = csr->pos;
		while (idx-- && tmp)
			tmp = tmp->next;
		ft_chdelone(ch, tmp);
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

void		chk_keys(t_choice **choices, t_cursor *csr)
{
	t_tkeys			kcmps;
	char			buff[5];

	if (!choices || !csr)
		return ;
	fill_kcmps(&kcmps);
	print_with_csr(*choices, csr);
	ft_bzero(buff, sizeof(buff));
	while (read(FT_OUT_FD, buff, 4) > 0)
	{
		if (ft_strequ(buff, "\n"))
			break ;
		interact(buff, &kcmps, choices, csr);
		print_with_csr(*choices, csr);
		ft_bzero(buff, sizeof(buff));
	}
}
