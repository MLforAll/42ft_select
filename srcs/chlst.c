/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chlst.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 21:48:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/22 16:44:25 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_select.h"

/*
** ft_chnew -- create new chain
**
** char*	title of item
** init		state of item
*/

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

/*
** ft_chpush -- add element to end of list
**
** t_choice**	head of existing list
** t_choice*	new chain
*/

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

/*
** ft_chdelone -- delete one element of list
**
** t_choice**	head of list
** t_choice*	chain to del
*/

void		ft_chdelone(t_choice **headref, t_choice *ch)
{
	t_choice	*bw;

	if (!headref || !ch)
		return ;
	bw = *headref;
	if (ch->prev)
	{
		ch->prev->next = ch->next;
		if (ch->next)
			ch->next->prev = ch->prev;
	}
	else
	{
		*headref = ch->next;
		if (*headref)
			(*headref)->prev = NULL;
	}
	ft_strdel(&ch->title);
	free(ch);
}

/*
** ft_chdel -- delete list
**
** t_choice**	head of list to del
*/

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

/*
** ft_chgetidx -- get a chain by idx
**
** t_choice*	head of list
** unsigned int	index of chain
*/

t_choice	*ft_chgetidx(t_choice *head, unsigned int idx)
{
	while (head && idx--)
		head = head->next;
	return (head);
}
