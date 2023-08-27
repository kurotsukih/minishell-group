/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akostrik <akostrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 14:51:15 by akostrik          #+#    #+#             */
/*   Updated: 2022/11/18 18:26:26 by akostrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// c: Le caractère à écrire
// fd: Le descripteur de fichier sur lequel écrire
// fonctions autorisées : write
// Écrit le caractère ’c’ sur le descripteur de fichier donné

// tester ft_putchar_fd   : 1.OK 

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
