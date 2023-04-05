/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: test <test@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 19:09:58 by bmoll-pe          #+#    #+#             */
/*   Updated: 2023/04/04 13:52:13 by test             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "defines.h"
#include "utils.h"
#include "signals.h"
#include "readline.h"

typedef struct s_sypar	t_sypar;
static int8_t	get_operator_group(char *str);
static bool		dquote_expander(char **to_expand);
static int8_t	syntax_parser(char **input);
static bool		print_syntax_error(char *to_print, int8_t size);
static void		syntax_parser_first_part(t_sypar *sypar);
static bool		syntax_parser_operator_condition(t_sypar *sypar);
static bool		syntax_parser_while(t_sypar *sypar);
char			*get_redirect_start(char *data, char *promt_init);

struct s_sypar
{
	char	*iter;
	char	*last_oper;
	int		squote;
	int		dquote;
	int		operator;
	int		bracket;
	size_t	count;
};

bool	syntax_check(char **input)
{
	int8_t	util;

	util = syntax_parser(input);
	while (util == 2)
		util = syntax_parser(input);
	return (util);
}

static int8_t	syntax_parser(char **input)
{
	t_sypar	sypar;

	sypar.iter = *input;
	sypar.last_oper = *input;
	sypar.count = 0;
	sypar.squote = -1;
	sypar.dquote = -1;
	sypar.operator = -1;
	sypar.bracket = 0;
	if (syntax_parser_while(&sypar))
		return (1);
	if (sypar.operator == 32 && sypar.count)
		return (print_syntax_error(sypar.iter - sypar.count + 1, 1));
	if (!sypar.count && sypar.operator == 2)
		return (print_syntax_error("newline", 7));
	if (sypar.dquote > 0 || sypar.squote > 0 || sypar.bracket
		|| (!sypar.count && (sypar.operator == 1 || sypar.operator == 31)))
	{
		if (dquote_expander(input))
			return (1);
		return (2);
	}
	return (0);
}

static void	syntax_parser_first_part(t_sypar *sypar)
{
	if ((!get_operator_group(sypar->iter) || isscaped(sypar->iter))
		&& (!ft_isspace(*(sypar->iter)) || isscaped(sypar->iter)))
		sypar->count += 1;
	if (sypar->squote < 0 && isquote(sypar->iter, 34))
		sypar->dquote *= -1;
	else if (sypar->dquote < 0 && isquote(sypar->iter, 39))
		sypar->squote *= -1;
	else if (sypar->squote < 0 && sypar->dquote < 0
		&& *(sypar->iter) == '(' && !isscaped(sypar->iter))
		sypar->bracket -= 1;
	else if (sypar->squote < 0 && sypar->dquote < 0
		&& *(sypar->iter) == ')' && !isscaped(sypar->iter))
		sypar->bracket += 1;
}

static bool	syntax_parser_while(t_sypar *sypar)
{
	while (*(sypar->iter))
	{
		syntax_parser_first_part(sypar);
		if (sypar->bracket > 0)
			return (print_syntax_error(")", 1));
		if (sypar->dquote < 0 && sypar->squote < 0
			&& get_operator_group(sypar->iter) != 0)
			if (syntax_parser_operator_condition(sypar))
				return (1);
		sypar->iter++;
	}
	return (0);
}

static bool	syntax_parser_operator_condition(t_sypar *sypar)
{
	if (sypar->operator == 2 && !sypar->count)
		return (print_syntax_error("newline", 7));
	if (sypar->operator == -1 && !sypar->count
		&& get_operator_group(sypar->iter) == 1)
		return (print_syntax_error(sypar->iter, 2));
	if (sypar->operator == 31
		&& !sypar->count && get_operator_group(sypar->iter) == 1)
		return (print_syntax_error(sypar->iter, 2));
	if (sypar->operator == 31
		&& !sypar->count && get_operator_group(sypar->iter) == 32)
		return (print_syntax_error(sypar->iter, 1));
	if (sypar->operator == 32 && sypar->count && get_operator_group(sypar->iter) == 2 && get_redirect_start(sypar->iter, sypar->last_oper) == sypar->iter)
	{
		// printf("%c || %p == %p\n", *sypar->iter, get_redirect_start(sypar->iter, sypar->last_oper), sypar->iter);
		return (print_syntax_error(sypar->iter, 1));
	}
	if (sypar->operator == 32 && sypar->count && get_operator_group(sypar->iter) != 2)
		return (print_syntax_error(sypar->iter, 1));
	if (sypar->operator == 1
		&& !sypar->count && get_operator_group(sypar->iter) == 1)
		return (print_syntax_error(sypar->iter, 1));
	sypar->operator = get_operator_group(sypar->iter);
	if (sypar->operator != 31 && sypar->operator != 32 && *(sypar->iter)
		&& *(sypar->iter + 1) && *(sypar->iter) == *(sypar->iter + 1))
		sypar->iter++;
	sypar->last_oper = sypar->iter;
	sypar->count = 0;
	return (0);
}

static int8_t	get_operator_group(char *str)
{
	if (!*str)
		return (1);
	if (*str == '|' && *(str + 1) && *(str + 1) == '|' && !isscaped(str))
		return (1);
	if (*str == '&' && *(str + 1) && *(str + 1) == '&' && !isscaped(str))
		return (1);
	if (*str == '|' && (!*(str + 1) || *(str + 1) != '|') && !isscaped(str))
		return (1);
	if (*str == '<' && (!*(str + 1) || *(str + 1) != '<') && !isscaped(str))
		return (2);
	if (*str == '<' && *(str + 1) && *(str + 1) == '<' && !isscaped(str))
		return (2);
	if (*str == '>' && (!*(str + 1) || *(str + 1) != '>') && !isscaped(str))
		return (2);
	if (*str == '>' && *(str + 1) && *(str + 1) == '>' && !isscaped(str))
		return (2);
	if (*str == ')' && !isscaped(str))
		return (32);
	if (*str == '(' && !isscaped(str))
		return (31);
	return (0);
}

static bool	dquote_expander(char **to_expand)
{
	char	*line;
	int		stdin_copy;

	stdin_copy = dup(STDIN_FILENO);
	init_signals(HERE_DOC);
	line = readline("> ");
	init_signals(NO_INTERACTIVE);
	if (g_global.is_ctrlc)
	{
		dup2(stdin_copy, STDIN_FILENO);
		g_global.is_ctrlc = 0;
		return (1);
	}
	if (!line)
		return (print_error(ft_strdup("unexpected EOF while looking for \
		matching `\"\'\nba.sh: syntax error: unexpected end of file"), 1, 258));
	*to_expand = ft_strjoin_free(ft_strjoin_free(*to_expand,
				ft_strdup("\n")), line);
	if (*to_expand)
		return (0);
	return (1);
}

static bool	print_syntax_error(char *to_print, int8_t size)
{
	write(2, "ba.sh: syntax error near unexpected token `", 43);
	write(2, to_print, size);
	write(2, "'\n", 2);
	g_global.num_return_error = 258;
	return (1);
}
