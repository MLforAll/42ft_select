/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/07 20:05:30 by kdumarai          #+#    #+#             */
/*   Updated: 2018/06/14 05:35:58 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "ft_select.h"

/*
** clear_choices
**
** t_env*		environnement
*/

void		clear_choices(t_env *env)
{
	size_t			len;

	len = (env->nlines + 1 > env->ws.ws_row) ? env->ws.ws_row : env->nlines + 1;
	movcap(0, (int)len);
	while (len--)
	{
		outcap("up");
		outcap("ce");
	}
}

/*
** print_padding -- print choice padding (private)
**
** t_choice*	current choice
** t_env*		environnement
*/

static void	print_padding(t_choice *ch, t_env *env)
{
	char			*padbuff;
	size_t			padlen;

	if (!ch->prev || !ch->prev->title)
		padlen = FT_PAD_NB;
	else
		padlen = env->mlen - ch->prev->titlelen + FT_PAD_NB;
	if (!(padbuff = ft_strnew(padlen)))
		return ;
	ft_memset(padbuff, ' ', padlen);
	ft_putstr_fd(padbuff, FT_OUT_FD);
	free(padbuff);
}

/*
** add_color (private)
**
** t_choice*	current choice
** t_env*		environnement
*/

static int	add_color(t_choice *ch, t_env *env)
{
	if (env->support_colors < 8 || ch->color < 1 || ch->color > 9)
		return (FALSE);
	ft_putstr_fd("\033[0;", FT_OUT_FD);
	ft_putnbr_fd(ch->color + 30, FT_OUT_FD);
	ft_putchar_fd('m', FT_OUT_FD);
	return (TRUE);
}

/*
** print_elem -- print choice (private)
**
** t_choice*	choice to print
** t_env*		environnement
** unsigned		column of choice
** unsigned		index of choice
*/

static int	print_elem(t_choice *ch,
						t_env *env,
						unsigned long ccol,
						unsigned long idx)
{
	size_t		len;
	size_t		off;
	int			scroll;
	int			colors;

	if (!ch || !ch->title || !env)
		return (FALSE);
	if (ccol > 0)
		print_padding(ch, env);
	colors = add_color(ch, env);
	(env->pos == idx) ? outcap("us") : 0;
	(ch->selected) ? outcap("mr") : 0;
	scroll = (env->pos == idx && ch->titlelen > env->ws.ws_col);
	len = (ch->titlelen > env->ws.ws_col) ? env->ws.ws_col : ch->titlelen;
	off = (scroll) ? env->scroll_off : 0;
	write(FT_OUT_FD, ch->title + off,
		(off + len > ch->titlelen) ? ch->titlelen - off : len);
	if (scroll)
		env->scroll_off = (env->scroll_off + 1 > ch->titlelen) ? 0
						: env->scroll_off + 1;
	if (ch->selected || env->pos == idx)
		outcap("me");
	if (colors)
		ft_putstr_fd("\033[0;39m", FT_OUT_FD);
	return (scroll);
}

/*
** print_with_env -- print all choices
**
** t_env*		environnement
*/

void		print_with_env(t_env *env)
{
	t_choice		*choices;
	unsigned long	idx;
	unsigned long	cline;
	unsigned long	ccol;
	int				scroll_sw;

	if (!env)
		return ;
	idx = env->vscroll;
	cline = 0;
	choices = ft_chgetidx(env->choices, idx);
	scroll_sw = FALSE;
	while (choices && cline < env->nlines)
	{
		ccol = 0;
		while (choices && ccol < env->ncols)
		{
			print_elem(choices, env, ccol, idx++) ? scroll_sw = TRUE : 0;
			choices = choices->next;
			ccol++;
		}
		cline++;
		ft_putstr_fd("\n\r", FT_OUT_FD);
	}
	set_read_timeout((scroll_sw) ? 2 : 0, NULL);
}
