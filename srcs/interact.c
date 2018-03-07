/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 22:55:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/07 21:57:26 by kdumarai         ###   ########.fr       */
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

static void	interact(char *buff, t_tkeys *kcmps, t_cursor *csr)
{
	if (ft_strcmp(buff, kcmps->upk) == 0 && csr->pos > 0)
		csr->pos--;
	if (ft_strcmp(buff, kcmps->downk) == 0 && csr->pos < csr->max)
		csr->pos++;
	if (ft_strncmp(buff, " ", 1) == 0)
	{
		ft_putchar('\r');
		outcap("mr");
	}
}

static void	fill_kcmps(t_tkeys *dest)
{
	dest->rightk = tgetstr("kr", NULL);
	dest->upk = tgetstr("ku", NULL);
	dest->downk = tgetstr("kd", NULL);
	dest->leftk = tgetstr("kl", NULL);
}

t_list		*chk_keys(int fd, t_list *choices)
{
	t_list			*ret;
	t_tkeys			kcmps;
	t_cursor		csr;
	char			buff[5];

	ret = NULL;
	fill_kcmps(&kcmps);
	csr.max = ft_lstlen(choices) - 1;
	csr.pos = 0;
	print_with_csr(choices, &csr);
	set_term(fd, NO);
	ft_bzero(buff, sizeof(buff));
	while (read(fd, buff, 4) > 0)
	{
		if (*buff == 3)
			break ;
		interact(buff, &kcmps, &csr);
		print_with_csr(choices, &csr);
		ft_bzero(buff, sizeof(buff));
	}
	set_term(fd, YES);
	return (ret);
}
