/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aitoraudicana <aitoraudicana@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 23:36:42 by bmoll-pe          #+#    #+#             */
/*   Updated: 2023/01/27 12:29:56 by aitoraudica      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bmlib.h"
#include "structs.h"
#include "minishell.h"
#include <unistd.h>

char	*get_redirect_end(char *data);
char	**expander(char *data, t_master *master);

_Bool	parser(t_node **list, char *parse_str, int reset, t_master *master)
{
	ssize_t		i;
	t_node		*node;
	char		*last_operator;
	char		*aux;

	(void) reset;
	if (!parse_str)
		return (1);
	i = 0;
	last_operator = parse_str;
	while (parse_str[i])
	{
		i += ffwd(&parse_str[i]);
		if (get_operator(&parse_str[i]))
		{
			if (!create_node(list, last_operator, &(parse_str[i]), master))
				return (1);
			if (get_operator(&parse_str[i]) > TCOL)
				i++;
			if (parse_str[i])
				i++;
			last_operator = &parse_str[i];
		}
		else if (parse_str[i] == '(')
		{
			aux = last_operator;
			while (*aux == ' ' && aux < &parse_str[i])
				aux++;
			if (aux != &parse_str[i])
				return (EXIT_FAILURE);
			node = create_node(list, &parse_str[i], &parse_str[i
					+ get_close_bracket(&parse_str[i]) + 1], master);
			if (node == NULL)
				return (1);
			if (parser (&(node->child), ft_substr(parse_str, i + 1,
						get_close_bracket(&parse_str[i]) - 1), 0, master))
				return (1);
			set_top(node->child, node);
			i += get_close_bracket(&parse_str[i]);
			aux = ft_substr(&parse_str[i], 1, ffwd(&parse_str[i]));
			i += ffwd(&parse_str[i]);
			node->operator = get_operator(&parse_str[i]);
			while (*aux)
			{
				if (extract_redirect(&aux, node, master))
					return (EXIT_FAILURE);
			}
			i++;
			if (node->operator > TCOL)
				i++;
			last_operator = &parse_str[i];
		}
		if (!parse_str[i])
			break ;
	}
	free (parse_str);
	return (0);
}

ssize_t	ffwd(char *start)
{
	size_t	count;

	count = 0;
	while (start[count] && !get_operator(&start[count]))
	{
		while (start[count] && ft_isspace(start[count]))
			count++;
		if (!start[count] || get_operator(&start[count])
			|| (start[count] == '(' && !isscaped(&start[count])))
			break ;
		if (isquote(&start[count], 34))
			while (start[count] && !isquote(&start[count], 34))
				count += 1;
		else if (isquote(&start[count], 39))
			while (&start[count] && !isquote(&start[count], 39))
				count += 1;
		if (start[count])
			count++;
	}
	return (count);
}

t_node	*create_node(t_node **list, char *start, char *end, t_master *master)
{
	t_node	*new_node;
	t_node	*temp;
	char	*raw_data;
	static	int	node_id = 0;
	char	**no_tokens;

	if (*(end + 1) == '\0')
		end++;
	new_node = malloc (sizeof(t_node));
	if (!new_node)
		return (NULL);
	ft_bzero(new_node, sizeof(t_node));
	new_node->node_id = node_id;
	new_node->subshell = false;
	if (*start == '(')
		new_node->subshell = true;
	raw_data = ft_substr(start, 0, end - start);
	new_node->data = extract_redirects_and_clean(raw_data, new_node, master);
	/// DEBUG HASTA QUE FUNCIONE EL NUEVO TOKENIZER
	//new_node->data = ft_substr(start, 0, end - start);
	no_tokens = malloc(sizeof(char *));
	no_tokens[0] = ft_strdup("");
	new_node->tokens = no_tokens;
	//new_node->tokens = tokenizer(new_node->data);
	/////////////////////////////////////////////////////
	new_node->operator = get_operator(end);
	if (*list)
	{
		temp = *list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
		new_node->prev = temp;
	}
	else
		*list = new_node;
	return (new_node);
}
