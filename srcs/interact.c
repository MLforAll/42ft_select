/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 22:55:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/05 23:50:48 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

static void	set_term(int fd, int features, int on)
{
	struct termios	t;

	if (tcgetattr(fd, &t) == -1)
		exit(EXIT_FAILURE);
	if (on)
		t.c_lflag |= features;
	else
		t.c_lflag &= ~features;
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	if (tcsetattr(fd, TCSADRAIN, &t) == -1)
		exit(EXIT_FAILURE);
}

static void	interact(char *buff, t_tkeys *kcmps)
{
	char	*act;

	act = NULL;
	if (ft_strcmp(buff, kcmps->right) == 0)
		act = kcmps->right;
	if (ft_strcmp(buff, kcmps->left) == 0)
		act = kcmps->left;
	if (ft_strcmp(buff, kcmps->up) == 0)
		act = kcmps->up;
	if (ft_strcmp(buff, kcmps->down) == 0)
		act = kcmps->down;
	if (act)
		tputs(act, 1, &putcf);
}

static void	fill_kcmps(t_tkeys *dest)
{
	dest->right = tgetstr("nd", NULL);
	dest->up = tgetstr("up", NULL);
	dest->down = tgetstr("do", NULL);
	dest->left = tgetstr("le", NULL);
	for (char *t = dest->down; *t; t++)
	{
		ft_putnbr(*t);
		ft_putchar('-');
	}
	ft_putchar('\n');
}

void		chk_keys(int fd)
{
	t_tkeys	kcmps;
	char	buff[5];
	int		features;

	fill_kcmps(&kcmps);
	features = ICANON | ECHO | ISIG;
	set_term(fd, features, NO);
	ft_bzero(buff, sizeof(buff));
	while (read(fd, buff, 4) > 0)
	{
		if (*buff == 3)
			break ;
		interact(buff, &kcmps);
		ft_bzero(buff, sizeof(buff));
	}
	set_term(fd, features, YES);
}
