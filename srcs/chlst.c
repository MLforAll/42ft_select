/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chlst.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 21:48:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/19 03:40:58 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_select.h"

t_choice	*ft_chnew(char *title, int selected)
{
	t_choice	*ret;

	if (!(ret = (t_choice*)malloc(sizeof(t_choice))))
		return (NULL);
	ft_bzero(ret, sizeof(t_choice));
	if (title)
	{
		ret->titlelen = ft_strlen(title);
		if (!(ret->title = (char*)malloc(sizeof(char) * (ret->titlelen + 1))))
		{
			free(ret);
			return (NULL);
		}
		ft_strcpy(ret->title, title);
	}
	ret->selected = selected;
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

	if (!headref || !ch)
		return ;
	bw = *headref;
	if (ch->prev)
		ch->prev->next = ch->next;
	else
	{
		*headref = ch->next;
		if (*headref)
			(*headref)->prev = NULL;
	}
	ft_strdel(&ch->title);
	free(ch);
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

t_choice	*ft_chgetidx(t_choice *head, unsigned int idx)
{
	while (head && idx--)
		head = head->next;
	return (head);
}
