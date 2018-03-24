/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interact.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 22:55:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/24 14:53:26 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "ft_select.h"

/*
** delete_curr_elem (private)
**
** char*		keys read
** t_env*		environement
*/

static void	delete_curr_elem(char *buff, t_env *env)
{
	t_choice		*tmp;

	if (!env)
		return ;
	if (!(tmp = ft_chgetidx(env->choices, env->pos)))
		return ;
	ft_chdelone(&env->choices, tmp);
	if (env->pos > 0)
	{
		env->pos -= (ft_strequ(buff, env->kcmps.bksp)
					|| env->pos >= env->max - 1);
		env->max--;
	}
	set_window_prop(env);
}

/*
** select_curr_elem (private)
**
** char*		keys read
** t_env*		environement
*/

static void	select_curr_elem(char *buff, t_env *env)
{
	t_choice		*bw;

	(void)buff;
	if (!env)
		return ;
	if (!(bw = ft_chgetidx(env->choices, env->pos)))
		return ;
	bw->selected = !bw->selected;
	(env->ncols > 1) ? mov_right(buff, env) : mov_down(buff, env);
}

/*
** interact (private)
**
** char*		keys read
** t_env*		environement
*/

static int	interact(char *buff, t_env *env)
{
	const char		*kbuffs[8] = {env->kcmps.upk, env->kcmps.downk,
								env->kcmps.leftk, env->kcmps.rightk, " ",
								env->kcmps.delk, env->kcmps.bksp, NULL};
	static void		(*kfuncs[8])(char *, t_env *) = {&mov_up,
								&mov_down, &mov_left, &mov_right,
								&select_curr_elem, &delete_curr_elem,
								&delete_curr_elem, NULL};
	unsigned int	idx;

	env->scroll_off = 0;
	idx = 0;
	while (kbuffs[idx])
	{
		if (ft_strequ(buff, kbuffs[idx]))
		{
			(kfuncs[idx])(buff, env);
			return (TRUE);
		}
		idx++;
	}
	return (FALSE);
}

/*
** redraw_hdl -- redraw sig handler (when resizing)
**
** unsigned long long	signal code or address of env (init)
*/

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

/*
** chk_keys -- routine that does interaction
**
** t_env*		environement
*/

int			chk_keys(t_env *env)
{
	char			buff[5];
	ssize_t			rb;

	if (!env)
		return (FALSE);
	print_with_env(env);
	ft_bzero(buff, sizeof(buff));
	while ((rb = read(FT_OUT_FD, buff, 4)) != -1)
	{
		if (rb > 0 && (ft_strequ(buff, "\n")
			|| ft_strasciieq(buff, 4) || ft_strasciieq(buff, 3)))
			break ;
		if (rb == 0 || interact(buff, env))
		{
			clear_choices(env);
			mov_page(env);
			print_with_env(env);
		}
		if ((rb > 0 && ft_strasciieq(buff, 27)) || !env->choices)
			return (FALSE);
		(rb > 0) ? ft_bzero(buff, sizeof(buff)) : 0;
	}
	return (TRUE);
}
