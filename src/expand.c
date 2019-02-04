/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-jesu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/04 14:07:28 by dde-jesu          #+#    #+#             */
/*   Updated: 2019/02/04 17:39:38 by dde-jesu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "env.h"
#include "ft/str.h"
#include "ft/mem.h"

static void	expand_home(struct s_shell *shell, char *arg, size_t *len,
		size_t *size)
{
	const size_t	i = arg - ((char *)shell->buffer + shell->env_size);
	char			*var;
	size_t			var_len;

	if (*arg == '~' && (arg[1] == '/' || arg[1] == '\0'))
	{
		var = sh_getenv(shell, "HOME", 4);
		var_len = ft_strlen(var);
		if (*size + var_len - 1 + shell->env_size > shell->buffer_size)
			return ;
		ft_memmove(arg + var_len, arg + 1, *size - i);
		ft_memcpy(arg, var, var_len);
		*len += var_len;
		*size += var_len;
		--*len;
		--*size;
	}
}

static void	expand_dollar(struct s_shell *shell, char *arg, size_t *len,
		size_t *size)
{
	const size_t	i = arg - ((char *)shell->buffer + shell->env_size);
	char			*var;
	size_t			var_len;
	char			*dollar;
	char			*end;

	dollar = ft_memchr(arg, '$', *len);
	while (dollar)
	{
		if (!(end = ft_memchr(dollar + 1, '$', arg + *len - (dollar + 1))))
			end = arg + *len;
		var = sh_getenv(shell, dollar + 1, end - dollar - 1);
		var_len = var ? ft_strlen(var) : 0;
		if (*size + var_len + shell->env_size - (end - dollar)
				> shell->buffer_size)
			return ;
		ft_memmove(dollar + var_len, end, *size - (end - arg) - i);
		ft_memcpy(dollar, var, var_len);
		*len += var_len;
		*len -= (end - dollar);
		*size += var_len;
		*size -= (end - dollar);
		dollar = ft_memchr(dollar + var_len, '$', *len - (dollar - arg));
	}
}

size_t		perform_expansion(struct s_shell *shell, size_t size)
{
	size_t	i;
	char	*arg;
	size_t	len;

	i = 0;
	while (i < size)
	{
		arg = (char *)shell->buffer + shell->env_size + i;
		len = ft_strlen(arg);
		expand_home(shell, arg, &len, &size);
		expand_dollar(shell, arg, &len, &size);
		if (len == 0)
		{
			ft_memmove(arg + 1, arg, size - i - 1);
			size--;
		}
		else
			i += len + 1;
	}
	return (size);
}
