/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chlst.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 21:48:16 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/09 14:30:20 by kdumarai         ###   ########.fr       */
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
	return (ret);
}

void		ft_chpush(t_choice **headref, t_choice *new)
{
	t_choice	**ptr;

	if (!headref)
		return ;
	ptr = headref;
	while (*ptr)
		ptr = &(*ptr)->next;
	*ptr = new;
}

void		ft_chdelone(t_choice **headref, t_choice *ch)
{
	t_choice	*bw;
	t_choice	*prev;
	t_choice	*tmp;

	if (!headref)
		return ;
	bw = *headref;
	prev = NULL;
	while (bw)
	{
		if (bw == ch)
		{
			if (prev)
				prev->next = bw->next;
			else
				*headref = bw->next;
			tmp = bw->next;
			ft_strdel(&bw->title);
			free(bw);
			bw = tmp;
		}
		else
		{
			prev = bw;
			bw = bw->next;
		}
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
