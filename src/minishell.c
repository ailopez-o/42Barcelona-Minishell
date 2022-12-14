/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ailopez- <ailopez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 13:06:45 by bruno             #+#    #+#             */
/*   Updated: 2022/12/14 18:39:09 by ailopez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minishell.h"

t_node	*hardcode_tree();
void 	print_tree(t_node *node);

int	main(int argc, char **argv, char **env)
{
	t_node	*node;

	(void)argv;
	(void)env;
	
	if (argc != 1)
		return (0);
	if (init_node(&node) || parser(node, readline("ba.sh $ "), 0xffffffff))
	{
		error(node, "ba.sh: error parsing input\n");
		exit (1);
	}
	print_tree(node);
	exit (0);
}

void    free_tree(t_node *node)
{
    if (node->right)
        free_tree(node->right);
    if (node->left)
        free_tree(node->left);
    if (!node->right && !node->right)
        free (node);
    else
        free (node);
}

void error(t_node *node, char *error)
{
    printf("ERRROR: %s\n", error);
    free_tree(node);
}

_Bool	init_node(t_node **node)
{
	static int node_id = 0;
	
	node_id ++;
	*node = malloc(sizeof(t_node));
	if (!*node)
		return (1);
	node[0]->node_id = node_id; 
	node[0]->type = 0;
	node[0]->start = NULL;
	node[0]->end = NULL;
	node[0]->top = NULL;
	node[0]->right = NULL;
	node[0]->left = NULL;
	node[0]->redirect = NULL;
	return (0);
}
