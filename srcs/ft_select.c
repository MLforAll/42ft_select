/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:24:55 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/07 21:20:19 by kdumarai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "ft_select.h"

/*
** get_choices
**
** char**		arguments of prgm aka av + 1
*/

static t_list	*get_choices(char **args)
{
	t_list	*ret;
	t_list	*new;

	ret = NULL;
	while (*args)
	{
		if (!(new = ft_lstnew(*args, ft_strlen(*args) + 1)))
			return (NULL);
		ft_lstpush(&ret, new);
		args++;
	}
	return (ret);
}

/*
** fatal
**
** const char*	app's name
** const char*	error message
*/

static void		fatal(const char *app, const char *err)
{
	ft_putstr_fd(app, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(err, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

/*
** main
**
** int			number of args from shell
** char**		arguments from shell
*/

int				main(int ac, char **av)
{
	t_list	*choices;
	char	*termtype;

	signal(SIGINT, SIG_IGN);
	if (!(termtype = getenv("TERM")))
		fatal(av[0], "TERM env var missing!");
	if (tgetent(NULL, termtype) <= 0)
		fatal(av[0], "Invalid terminal!");
	if (ac == 1 || !(choices = get_choices(av + 1)))
		return (EXIT_FAILURE);
	outcap("cl");
	outcap("ks");
	outcap("vi");
	chk_keys(STDIN_FILENO, choices);
	ft_lstdel(&choices, &ft_lstdelf);
	outcap("ve");
	return (EXIT_SUCCESS);
}
