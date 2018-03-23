/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   navigation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/16 21:29:38 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/23 17:50:52 by kdumarai         ###   ########.fr       */
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

void	mov_page(t_env *env)
{
	if (env->nlines == 0 || env->ncols == 0)
		return ;
	env->vscroll = env->nlines * env->ncols \
					* (env->pos / (env->nlines * env->ncols));
	if (env->vscroll >= env->max)
		env->vscroll -= (env->nlines * env->ncols);
}

/*
** mov_up -- up key
**
** char*		keys read
** t_env*		environement
*/

void	mov_up(char *buff, t_env *env)
{
	(void)buff;
	if ((long)(env->pos - env->ncols) >= 0)
		env->pos -= env->ncols;
	else if (env->ncols == 1)
		env->pos = env->max - 1;
	if (env->nlines != env->ws.ws_row - 1)
		return ;
}

/*
** mov_down -- down key
**
** char*		keys read
** t_env*		environement
*/

void	mov_down(char *buff, t_env *env)
{
	(void)buff;
	if (env->pos < env->max - env->ncols)
		env->pos = env->pos + env->ncols;
	else if (env->pos + 1 == env->max && env->ncols == 1)
		env->pos = 0;
	if (env->nlines != env->ws.ws_row - 1)
		return ;
}

/*
** mov_left -- left key
**
** char*		keys read
** t_env*		environement
*/

void	mov_left(char *buff, t_env *env)
{
	(void)buff;
	if (env->ncols < 2)
		return ;
	env->pos = (env->pos > 0) ? env->pos - 1 : env->max - 1;
}

/*
** mov_right -- right key
**
** char*		keys read
** t_env*		environement
*/

void	mov_right(char *buff, t_env *env)
{
	(void)buff;
	if (env->ncols < 2)
		return ;
	env->pos = (env->pos < env->max - 1) ? env->pos + 1 : 0;
}
