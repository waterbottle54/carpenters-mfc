#include "stdafx.h"
#include "expression.h"
#include <wchar.h>
#include <wtypes.h>
#include <math.h>
#include <stdio.h>
#pragma warning (disable:4996)



double factor(wchar_t* expression, bool* valid)
{
	wchar_t* pc;
	double numeral;

	*valid = true;

	// calculate sine factor
	pc = wcswcs(expression, L"sin");
	if (pc != NULL)
	{
		numeral = _wtof(expression + 3);
		return sin(numeral);
	}
	
	// calculate cosine factor
	pc = wcswcs(expression, L"cos");
	if (pc != NULL)
	{
		numeral = _wtof(expression + 3);
		return cos(numeral);
	}

	// calculate tangent factor
	pc = wcswcs(expression, L"tan");
	if (pc != NULL)
	{
		numeral = _wtof(expression + 3);
		return tan(numeral);
	}

	// calculate logarithmic factor
	pc = wcswcs(expression, L"log");
	if (pc != NULL)
	{
		numeral = _wtof(expression + 3);
		if (numeral > 0) return log(numeral);
		else 
		{ 
			*valid = false; 
			return 0; 
		}
	}

	// calculate number factor
	return _wtof(expression);
}

double term(wchar_t* expression, bool* valid)
{
	wchar_t *pc, *po, c;
	double	result, result_f;
	
	for (pc = expression + 1, po = expression - 1;; po = pc++)
	{
		// find factor from term
		pc = wcspbrk(pc, L"*/^");
		if (!pc) pc = expression + wcslen(expression);
		c = *pc; *pc = L'\0';

		// calculate result with factor
		result_f = factor(po + 1, valid);
		if (*valid == true)
		{
			if (po == expression - 1) result = result_f;
			else if (*po == L'*') result *= result_f;
			else if (*po == L'/' && result_f != 0) result /= result_f;
			else if (*po == L'^') result = pow(result, result_f);
			else *valid = false;
		}

		// end calculation
		*pc = c;
		if (*valid == false) return 0;
		else if (*pc == L'\0') break;
	}
	return result;
}

double poly(wchar_t* expression, bool* valid)
{
	wchar_t *pc, *po, c;
	double result, result_t;

	for (pc = expression + 1, po = expression - 1;; pc++)
	{
		// find term from polynomial
		pc = wcspbrk(pc, L"+-");
		if (!pc) pc = expression + wcslen(expression);
		else if (!iswdigit(*(pc - 1))) continue;
		c = *pc; *pc = L'\0';
		
		// calculate result with term
		result_t = term(po + 1, valid);
		if (*valid == true)
		{
			if (po == expression - 1) result = result_t;
			else if (*po == L'+') result += result_t;
			else if (*po == L'-') result -= result_t;
			else *valid = false;
		}

		// stop or continue calculation
		*pc = c;
		if (*valid == false) return 0;
		else if (*pc == L'\0') break;
		else po = pc;
	}
	return result;
}


wchar_t* unnest(wchar_t* open, wchar_t* close, bool topmost, bool* valid)
{
	wchar_t str[32];
	wchar_t *pc, *ps;
	int		depth, offset;
	double	result;

	if (topmost == true)
		*valid = true;

	// unnest all sub-nested brakets
	depth = 0;
	for (pc = open + 1, ps = NULL; pc < close & *valid; pc++)
	{
		if (*pc == L'(')
		{
			if (depth++ == 0)
				ps = pc;
		}
		else if (*pc == L')')
		{
			if (depth-- == 1)
			{
				offset = unnest(ps, pc, false, valid) - pc;
				pc += offset;
				close += offset;
			}
		}
	}

	// unnest this braket
	if (!topmost && *valid)
	{
		*close = L'\0';
		result = poly(open + 1, valid);
		*close = L')';
		if (*valid == true)
		{
			swprintf(str, 32, L"%lf", result);
			*close = L')';
			memmove(open + wcslen(str), close + 1, wcslen(close)*sizeof(wchar_t));
			wcsncpy(open, str, wcslen(str));
			close = open + wcslen(str) - 1;
		}
	}
	return close;
}

double formula(const wchar_t* expression, bool* valid)
{
	static wchar_t copy[1024];
	double result;

	// make copy of given expression
	wcscpy(copy, expression);
	
	// unnest and calculate expression
	unnest(copy-1, copy + wcslen(copy), true, valid);
	if (*valid == true)
	{
		result = poly(copy, valid);
		if (*valid == true) return result;
	}
	return 0;
}


void subtitute(wchar_t* expression, const wchar_t* vars, va_list ap)
{
	wchar_t* pc;
	wchar_t str[32];
	double val;

	for (; *vars; vars++)
	{
		// retrieve value of each variable
		val = va_arg(ap, double);
		swprintf(str, L"%lf", val);
		// subtitute value to each variables
		pc = wcschr(expression, *vars);
		while (pc != NULL)
		{
			memmove(pc + wcslen(str), pc + 1,
				(expression + wcslen(expression) - pc) * sizeof(wchar_t));
			wcsncpy(pc, str, wcslen(str));
			pc = wcschr(pc + wcslen(str), *vars);
		}
	}
}

double function(const wchar_t* expression, bool* valid, const wchar_t* vars, ...)
{
	static wchar_t copy[1024];
	double result;
	va_list ap;
	
	// make copy of given expression
	wcscpy(copy, expression);

	// subtitute value to variables in expression
	va_start(ap, vars);
	subtitute(copy, vars, ap);
	va_end(ap);

	// unnest and calculate expression
	unnest(copy - 1, copy + wcslen(copy), true, valid);
	if (*valid == true)
	{
		result = poly(copy, valid);
		if (*valid == true) return result;
	}
	return 0;
}


