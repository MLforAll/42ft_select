/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 19:20:41 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/09 20:00:16 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

static struct termios	saved_t;

void					restore_terminal(void)
{
	outcap("ve");
	outcap("ke");
	outcap("te");
	tcsetattr(FT_OUT_FD, TCSADRAIN, &saved_t);
}

/*
**static void				signal_hdl_restore(int sigc)
**{
**	(void)sigc;
**	restore_terminal();
**}
*/

int						init_terminal(void)
{
	struct termios	t;
	int				success;

	success = 0;
	if (tcgetattr(FT_OUT_FD, &saved_t) == -1)
		return (FALSE);
	t = saved_t;
	t.c_lflag &= ~ICANON | ECHO | ISIG;
	t.c_oflag &= ~OPOST;
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	if (tcsetattr(FT_OUT_FD, TCSADRAIN, &t) == -1)
		return (FALSE);
	success += outcap("ti") + outcap("ks") + outcap("vi");
	return ((success > 0));
}
