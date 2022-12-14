/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoll-pe <bmoll-pe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 23:29:10 by bmoll-pe          #+#    #+#             */
/*   Updated: 2022/12/14 05:04:37 by bmoll-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//	---- minishell.c
void 	error(t_node *node, char *error);
void    free_tree(t_node *node);
_Bool	init_node(t_node **node);

//	---- parser.c
_Bool	parser(t_node *node, char *input, size_t len);

#endif