/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 22:55:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/09 14:54:15 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

static void	set_term(int fd, int on)
{
	struct termios	t;
	const tcflag_t	lflag_chg = ICANON | ECHO | ISIG;
	const tcflag_t	oflag_chg = OPOST;

	if (tcgetattr(fd, &t) == -1)
		exit(EXIT_FAILURE);
	if (on)
	{
		t.c_lflag |= lflag_chg;
		t.c_oflag |= oflag_chg;
	}
	else
	{
		t.c_lflag &= ~lflag_chg;
		t.c_oflag &= ~oflag_chg;
	}
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	if (tcsetattr(fd, TCSADRAIN, &t) == -1)
		exit(EXIT_FAILURE);
}

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

t_list		*chk_keys(int fd, t_choice *choices, t_cursor *csr)
{
	t_list			*ret;
	t_tkeys			kcmps;
	char			buff[5];

	ret = NULL;
	fill_kcmps(&kcmps);
	print_with_csr(choices, csr);
	set_term(fd, NO);
	ft_bzero(buff, sizeof(buff));
	while (read(fd, buff, 4) > 0)
	{
		if (ft_strequ(buff, "\n"))
		{
			clear_choices(csr->max);
			return_res(ret);
			break ;
		}
		interact(buff, &kcmps, &choices, csr);
		print_with_csr(choices, csr);
		ft_bzero(buff, sizeof(buff));
	}
	set_term(fd, YES);
	return (ret);
}
