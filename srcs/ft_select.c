/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdumarai <kdumarai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/05 18:24:55 by kdumarai          #+#    #+#             */
/*   Updated: 2018/03/14 19:07:06 by kdumarai         ###   ########.fr       */
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

static size_t	get_choices(t_choice **dest, t_cursor *csr, char **args)
{
	size_t		ret;
	size_t		len;
	t_choice	*new;

	if (!dest)
		return (0);
	ret = 0;
	*dest = NULL;
	while (*args)
	{
		len = ft_strlen(*args);
		if (len > csr->mlen)
			csr->mlen = len;
		if (!(new = ft_chnew(*args, NO)))
			return (0);
		ft_chpush(dest, new);
		args++;
		ret++;
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
	t_choice	*choices;
	t_cursor	csr;
	char		*termtype;

	if (!(termtype = getenv("TERM")))
		fatal(av[0], "TERM env var missing!");
	if (tgetent(NULL, termtype) <= 0)
		fatal(av[0], "Invalid terminal!");
	ft_bzero(&csr, sizeof(t_cursor));
	if (ac == 1 || !(csr.max = get_choices(&choices, &csr, av + 1)))
		return (EXIT_FAILURE);
	set_window_prop(&csr);
	if (!init_terminal())
		fatal(av[0], "Error setting the terminal");
	chk_keys(&choices, &csr);
	restore_terminal();
	return_res(choices);
	ft_chdel(&choices);
	return (EXIT_SUCCESS);
}
