/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoll-pe <bmoll-pe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 23:29:10 by bmoll-pe          #+#    #+#             */
/*   Updated: 2022/12/21 20:14:20 by bmoll-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//	---- minishell.c
void	error(t_node *node, char *error);
t_node	*free_tree(t_node *node);
_Bool	init_node(t_node **node, int mode);

//	---- parser.c
_Bool	parser(t_node **list, char *parse_str, int reset);

//	---- utils.c
//void 	print_tree(t_node *node);
void	free_split(char	**split);
void	print_parse_tree(t_node *node);
void	logtrace(char *str, int param1, int param2, int param3);

//	---- executor.c
int		executor(t_node *node);

//	---- env.c
int		env_new_value(t_env **list, char *name, char *value);
void	env_set_value(t_env *list, char *name, char *value);
void	env_unset_value(t_env *list, char *name);
char	*env_get_value(t_env *list, char *name);
t_env	*env_search(t_env *list, char *name);
void	print_env(t_env *env_list);
void	env_free_list(t_env *list);
t_env	*env_parser(char **env);

#endif
