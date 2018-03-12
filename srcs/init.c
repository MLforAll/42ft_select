/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/09 19:20:41 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/12 23:01:09 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "ft_select.h"

static struct termios	saved_t;

void					restore_terminal(void)
{
	outcap("ve");
	outcap("ke");
	outcap("te");
	tcsetattr(FT_OUT_FD, TCSADRAIN, &saved_t);
}

static void				signal_hdl_restore(int sigc)
{
	restore_terminal();
	if (sigc == SIGINT)
		exit(EXIT_SUCCESS);
	signal(sigc, SIG_DFL);
	raise(sigc);
}

int						init_terminal(void)
{
	struct termios	t;
	int				success;
	int				sigc;

	success = 0;
	if (tcgetattr(FT_OUT_FD, &saved_t) == -1)
		return (FALSE);
	t = saved_t;
	t.c_lflag &= ~(ICANON | ECHO);
	t.c_oflag &= ~OPOST;
	t.c_cc[VMIN] = 1;
	t.c_cc[VTIME] = 0;
	if (tcsetattr(FT_OUT_FD, TCSADRAIN, &t) == -1)
		return (FALSE);
	if (!(success += outcap("ti") + outcap("ks") + outcap("vi")))
		return (FALSE);
	sigc = 0;
	while (sigc < 32)
	{
		signal(sigc, (sigc != SIGCONT) ? &signal_hdl_restore : &suspend_hdl);
		sigc++;
	}
	return (TRUE);
}
