/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 22:55:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/06 18:39:12 by kdumarai         ###   ########.fr       */
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

static void	interact(char *buff, t_tkeys *kcmps)
{
	char	*act;

	act = NULL;
	if (ft_strcmp(buff, kcmps->rightk) == 0)
		act = kcmps->rightm;
	if (ft_strcmp(buff, kcmps->leftk) == 0)
		act = kcmps->leftm;
	if (ft_strcmp(buff, kcmps->upk) == 0)
		act = kcmps->upm;
	if (ft_strcmp(buff, kcmps->downk) == 0)
		act = kcmps->downm;
	if (act)
		tputs(act, 1, &putcf);
}

static void	fill_kcmps(t_tkeys *dest)
{
	dest->rightk = tgetstr("kr", NULL);
	dest->rightm = tgetstr("nd", NULL);
	dest->upk = tgetstr("ku", NULL);
	dest->upm = tgetstr("up", NULL);
	dest->downk = tgetstr("kd", NULL);
	dest->downm = tgetstr("do", NULL);
	dest->leftk = tgetstr("kl", NULL);
	dest->leftm = tgetstr("le", NULL);
}

void		chk_keys(int fd)
{
	t_tkeys	kcmps;
	char	buff[5];

	fill_kcmps(&kcmps);
	set_term(fd, NO);
	ft_bzero(buff, sizeof(buff));
	while (read(fd, buff, 4) > 0)
	{
		if (*buff == 3)
			break ;
		interact(buff, &kcmps);
		ft_bzero(buff, sizeof(buff));
	}
	set_term(fd, YES);
}
