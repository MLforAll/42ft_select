/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chlst.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 21:48:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/09 20:06:07 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_select.h"

t_choice	*ft_chnew(char *title, int selected)
{
	t_choice	*ret;

	if (!(ret = (t_choice*)malloc(sizeof(t_choice))))
		return (NULL);
	if (title && !(ret->title = ft_strdup(title)))
	{
		free(ret);
		return (NULL);
	}
	ret->selected = selected;
	ret->next = NULL;
	ret->prev = NULL;
	return (ret);
}

void		ft_chpush(t_choice **headref, t_choice *new)
{
	t_choice	*bw;

	if (!headref)
		return ;
	if (!*headref)
	{
		*headref = new;
		return ;
	}
	bw = *headref;
	while (bw->next)
		bw = bw->next;
	new->prev = bw;
	bw->next = new;
}

void		ft_chdelone(t_choice **headref, t_choice *ch)
{
	t_choice	*bw;

	if (!headref)
		return ;
	bw = *headref;
	if (ch->prev)
		ch->prev->next = ch->next;
	else
	{
		*headref = ch->next;
		ft_strdel(&ch->title);
		free(ch);
	}
}

void		ft_chdel(t_choice **headref)
{
	t_choice	*bw;
	t_choice	*bak;

	if (!headref)
		return ;
	bw = *headref;
	while (bw)
	{
		bak = bw->next;
		ft_strdel(&bw->title);
		free(bw);
		bw = bak;
	}
	*headref = NULL;
}

void		switch_selected(t_choice *head, unsigned int idx)
{
	if (!head)
		return ;
	while (idx-- && head)
		head = head->next;
	head->selected = !head->selected;
}