/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/19 23:36:29 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/22 19:07:10 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "ft_select.h"

/*
** set_window_prop
**
** t_env*	destination struct
*/

void		set_window_prop(t_env *dest)
{
	t_choice	*bw;

	if (!dest)
		return ;
	bw = dest->choices;
	dest->mlen = 0;
	while (bw)
	{
		if (bw->titlelen > dest->mlen)
			dest->mlen = bw->titlelen;
		bw = bw->next;
	}
	ft_bzero(&dest->ws, sizeof(struct winsize));
	ioctl(FT_OUT_FD, TIOCGWINSZ, &dest->ws);
	if (dest->max < dest->ws.ws_row || dest->mlen > dest->ws.ws_col
		|| dest->mlen <= 0)
		dest->ncols = 1;
	else
		dest->ncols = dest->ws.ws_col / (dest->mlen + FT_PAD_NB);
	if (dest->ncols <= 0)
		dest->ncols = 1;
	dest->nlines = dest->max / dest->ncols + (dest->max % dest->ncols);
	(dest->nlines >= dest->ws.ws_row) ? dest->nlines = dest->ws.ws_row - 1 : 0;
	mov_page(dest);
}

/*
** init_display
**
** t_env*	cursor
*/

int			init_restore_display(t_env *env, int init)
{
	if (!init)
	{
		outcap("ve");
		outcap("ke");
		outcap("te");
		return (TRUE);
	}
	if (!env)
		return (FALSE);
	if (!outcap("ks"))
		return (FALSE);
	if (!outcap("ti") && !outcap("cl"))
		return (FALSE);
	outcap("vi");
	signal(SIGTSTP, (void(*)(int))&signal_hdl);
	signal(SIGCONT, (void(*)(int))&redraw_hdl);
	signal(SIGWINCH, (void(*)(int))&redraw_hdl);
	set_window_prop(env);
	return (TRUE);
}

/*
** fill_kcmps
**
** t_tkeys*		struct containing term movkeys caps
*/

int			fill_kcmps(t_tkeys *dest)
{
	if (!dest)
		return (FALSE);
	dest->rightk = tgetstr("kr", NULL);
	dest->upk = tgetstr("ku", NULL);
	dest->downk = tgetstr("kd", NULL);
	dest->leftk = tgetstr("kl", NULL);
	dest->delk = tgetstr("kD", NULL);
	(dest->bksp)[0] = 127;
	(dest->bksp)[1] = '\0';
	return ((dest->rightk && dest->upk && dest->downk
			&& dest->leftk && dest->delk));
}
