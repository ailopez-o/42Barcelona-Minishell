/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aitoraudicana <aitoraudicana@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 18:46:28 by ailopez-          #+#    #+#             */
/*   Updated: 2023/03/08 21:03:33 by aitoraudica      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "wildcard.h"
#include "utils.h"
#include "env.h"

int	quotes_sub_handler(t_tokener *tk, char type)
{
	if (type == 39)
		tk->is_quoted = !tk->is_quoted;
	if (type == 34)
		tk->is_dbl_quoted = !tk->is_dbl_quoted;
	if (!tk->is_quoted)
		tk->is_quoted_dolar = 0;
	if ((type == 39 && !tk->is_quoted) || (type == 34 && !tk->is_dbl_quoted))
	{
		tk->data++;
		return (0);
	}
	if (is_isolated_quotes(tk->data, type))
	{
		if (type == 39)
			tk->is_quoted = !tk->is_quoted;
		if (type == 34)
			tk->is_dbl_quoted = !tk->is_dbl_quoted;
		while (*tk->data == type)
			tk->data++;
		if (*tk->data)
			tk->data++;
		return (1);
	}
	tk->data++;
	return (0);
}

char	*expand_wildcard(t_tokener *tk, char *new_data)
{
	int		pos;
	char	*expanded;
	char	*word;
	char	*word_init;
	int		to_delete;
	char	*temp;

	word_init = get_word_init(tk->data, tk->full_data, LIM_INIT);
	to_delete = tk->data - word_init;
	temp = ft_substr(new_data, 0, ft_strlen(new_data) - to_delete);
	free(new_data);
	new_data = temp;
	tk->data = get_word_init(tk->data, tk->full_data, 0);
	pos = get_word_end(tk->data, LIM_WILDCARD) - tk->data;
	word = ft_substr(tk->data, 0, pos);
	expanded = expand_str_wildcard(word);
	free (word);
	tk->data = str_pro_join(tk->data + pos, expanded, 0);
	tk->exp_mode = 2;
	tk->end_expansion = tk->data + ft_strlen(expanded);
	free(expanded);
	free(tk->original_promt);
	tk->original_promt = tk->data;
	return (new_data);
}

char	*dolar_expansion(char **data, t_env *env_list)
{
	char	*expanded;
	int		pos;
	char	*word;
	char	*value;
	
	expanded = ft_strdup("");
	while (*data && **data == '$')
	{
		(*data)++;
		if (**data == '?')
		{
			expanded = ft_itoa(global.num_return_error);
			(*data)++;
		}
		else
		{
			pos = get_word_end(*data, LIM_DOLLAR) - *data;
			word = ft_substr(*data, 0, pos);
			value = env_get_value(env_list, word);
			free(word);
			if (value != NULL)
				expanded = ft_strjoin_free(expanded, value);
			*data += pos;
		}
	}
	return (expanded);
}

char	*str_dollar_expander(char *data, bool no_expand, t_env *env_list)
{
	char	*new_data;
	char	*data_ini;
	
	data_ini = data;
	if (no_expand)
		return (data);
	new_data = ft_strdup("");
	while (*data)
	{
		if (*data == '\\' && data++)
			new_data = ft_chrjoin(new_data, *data++);
		else if (*data == '$')
			new_data = ft_strjoin_free(new_data, dolar_expansion(&data, env_list));
		else
			new_data = ft_chrjoin(new_data, *data++);
	}
	free(data_ini);
	return (new_data);
}

char	*tknexp_init(char *data, t_tokener *tk, t_master *master, int reset)
{
	if (reset && !data)
		return (NULL);
	if (reset)
	{
		if (tk->original_promt)
			free(tk->original_promt);
		tk->data = ft_strdup(data);
		tk->original_promt = tk->data;
		tk->master = master;
		tk->is_quoted = 0;
		tk->is_dbl_quoted = 0;
		tk->exp_mode = 0;
		tk->end_expansion = NULL;
		tk->return_token = 0;
	}
	tk->full_data = tk->data;
	spaces_clean(&tk->data);
	if (!*tk->data)
	{
		return (NULL);
	}
	return (ft_strdup(""));
}
