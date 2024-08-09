/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkannan <nkannan@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 09:03:05 by nkannan           #+#    #+#             */
/*   Updated: 2024/05/19 09:05:01 by nkannan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*tmp_list;
	t_list	*del_list;

	if (lst == NULL || del == NULL)
		return ;
	tmp_list = *lst;
	while (tmp_list != NULL)
	{
		del_list = tmp_list;
		tmp_list = tmp_list->next;
		ft_lstdelone(del_list, del);
	}
	*lst = NULL;
}
