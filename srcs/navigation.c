/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   navigation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 21:29:38 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/22 15:41:31 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

/*
** mov_page -- switch choice page
**
** t_env*		environement
** int			going backward (YES) or forward (NO)
*/

static void	mov_page(t_env *env, int up)
{
	(void)up;
	env->vscroll = env->nlines * env->ncols \
					* (env->pos / (env->nlines * env->ncols));
	/*if (up)
	{
		if (env->pos == env->max - env->ncols)
		{
			env->vscroll = env->nlines * env->ncols \
					* (env->max / (env->nlines * env->ncols));
			if (env->vscroll >= env->max)
				env->vscroll -= (env->nlines * env->ncols);
		}
		else if (env->pos < env->vscroll)
			env->vscroll -= env->nlines * env->ncols;
	}
	else
	{
		if (env->pos == 0)
			env->vscroll = 0;
		else if (env->pos + 1 > env->vscroll + env->nlines * env->ncols)
			env->vscroll += env->nlines * env->ncols;
	}*/
}

/*
** mov_up -- up key
**
** t_env*		environement
*/

void		mov_up(t_env *env)
{
	if ((long)(env->pos - env->ncols) >= 0)
		env->pos -= env->ncols;
	else if (env->ncols == 1)
		env->pos = env->max - 1;
	if (env->nlines != env->ws.ws_row - 1)
		return ;
	mov_page(env, TRUE);
}

/*
** mov_down -- down key
**
** t_env*		environement
*/

void		mov_down(t_env *env)
{
	if (env->pos < env->max - env->ncols)
		env->pos = env->pos + env->ncols;
	else if (env->pos + 1 == env->max && env->ncols == 1)
		env->pos = 0;
	if (env->nlines != env->ws.ws_row - 1)
		return ;
	mov_page(env, FALSE);
}

/*
** mov_left -- left key
**
** t_env*		environement
*/

void		mov_left(t_env *env)
{
	if (env->ncols < 2)
		return ;
	env->pos = (env->pos > 0) ? env->pos - 1 : env->max - 1;
	mov_page(env, TRUE);
}

/*
** mov_right -- right key
**
** t_env*		environement
*/

void		mov_right(t_env *env)
{
	if (env->ncols < 2)
		return ;
	env->pos = (env->pos < env->max - 1) ? env->pos + 1 : 0;
	mov_page(env, FALSE);
}
