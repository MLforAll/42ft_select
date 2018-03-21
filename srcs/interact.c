/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 22:55:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/21 08:39:08 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "ft_select.h"

static void	delete_curr_elem(t_env *env)
{
	t_choice		*tmp;

	if (!env)
		return ;
	if (!(tmp = ft_chgetidx(env->choices, env->pos)))
		return ;
	ft_chdelone(&env->choices, tmp);
	if (env->pos > 0)
	{
		env->pos--;
		env->max--;
	}
	set_window_prop(env);
}

static void	select_curr_elem(t_env *env)
{
	t_choice		*bw;

	if (!env)
		return ;
	if (!(bw = ft_chgetidx(env->choices, env->pos)))
		return ;
	bw->selected = !bw->selected;
	if (env->ncols > 1)
		mov_right(env);
	else
		mov_down(env);
}

static int	interact(char *buff, t_tkeys *kcmps, t_env *env)
{
	const char		*kbuffs[] = {kcmps->upk, kcmps->downk,
								kcmps->leftk, kcmps->rightk, " ",
								kcmps->delk, kcmps->bsk, NULL};
	static void		(*kfuncs[])(t_env *) = {&mov_up,
								&mov_down, &mov_left, &mov_right,
								&select_curr_elem, &delete_curr_elem,
								&delete_curr_elem, NULL};
	unsigned int	idx;

	if (!env || !kcmps)
		return (FALSE);
	env->scroll_off = 0;
	idx = 0;
	while (kbuffs[idx])
	{
		if (ft_strequ(buff, kbuffs[idx]))
		{
			(kfuncs[idx])(env);
			return (TRUE);
		}
		idx++;
	}
	return (FALSE);
}

void		redraw_hdl(unsigned long long sigc)
{
	static t_env	*env = NULL;

	if (!env)
	{
		env = (t_env*)sigc;
		return ;
	}
	if (!env || sigc > INT_MAX)
		return ;
	if (sigc == SIGCONT)
	{
		set_signals();
		init_restore_terminal(YES, NULL);
		init_restore_display(env, YES);
	}
	else
	{
		outcap("cl");
		set_window_prop(env);
	}
	print_with_env(env);
}

int			chk_keys(t_env *env, t_tkeys *kcmps)
{
	char			buff[5];
	ssize_t			rb;

	if (!env)
		return (FALSE);
	print_with_env(env);
	ft_bzero(buff, sizeof(buff));
	while ((rb = read(FT_OUT_FD, buff, 4)) != -1)
	{
		if (rb > 0 && (ft_strequ(buff, "\n") || ft_strasciieq(buff, 4)))
			break ;
		if (rb == 0 || interact(buff, kcmps, env))
		{
			clear_choices(env);
			print_with_env(env);
		}
		if ((rb > 0 && ft_strasciieq(buff, 27)) || !env->choices)
			return (FALSE);
		(rb > 0) ? ft_bzero(buff, sizeof(buff)) : 0;
	}
	return (TRUE);
}
