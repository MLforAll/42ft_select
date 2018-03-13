/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ul_cols.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 21:06:25 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/13 21:13:48 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <curses.h>
#include <term.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int	putcf(int c)
{
	write(1, &c, 1);
	return (1);
}

int	outcap(char *name)
{
	char*cap;

	cap = tgetstr(name, NULL);
	if (!cap)
		return (	FALSE);
	tputs(cap, 1, &putcf);
	return (TRUE);
}

void	ft_putstr(char *s)
{
	write(1, s, strlen(s));
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putendl(char *s)
{
	ft_putstr(s);
	ft_putchar('\n');
}

int		main()
{
	tgetent(NULL, getenv("TERM"));
	outcap("mr");
	ft_putstr("hello");
	outcap("me");
	ft_putendl("\twesh");
	return 0;
}
