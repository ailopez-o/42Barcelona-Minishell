/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: test <test@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 18:44:39 by ailopez-          #+#    #+#             */
/*   Updated: 2023/03/05 12:56:07 by test             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BULTIN_UTILS_H
# define BULTIN_UTILS_H

bool	isalphanum(char *str);
char	*get_current_pwd(void);
int		get_export_values(t_node *node, char **name, char **value);

#endif
