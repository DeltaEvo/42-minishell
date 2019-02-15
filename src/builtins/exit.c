/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/20 15:42:36 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/02/15 10:37:16 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#define USAGE "Usage: exit [n]\n"

static bool		ft_atoi(const char *str, int *res)
{
	long	result;
	char	mod;

	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	result = 0;
	mod = *str == '-' || *str == '+' ? *str++ == '-' : 0;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + *str++ - '0';
		if (result > ((long)INT_MAX + mod))
			return (false);
	}
	if (*str != '\0')
		return (false);
	*res = (mod ? -result : result);
	return (true);
}

int				builtin_exit(int argc, char **argv)
{
	int	code;

	code = 0;
	if (argc == 1)
		code = 0;
	else if (argc > 2 || !ft_atoi(argv[1], &code))
	{
		write(2, USAGE, sizeof(USAGE) - 1);
		return (1);
	}
	exit(code);
}
