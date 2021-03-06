#include "holberton.h"

/**
 * pr_char - prints a char
 * @ap: list of variadic input
 * @mask: indicates flags
 * @width: field width (spacing)
 * @precis: precision (digits)
 *
 * Return: 1 (length of one char)
 */
int pr_char(va_list ap, int mask, int width,  __attribute__((unused))int precis)
{
	char c = (char)va_arg(ap, int);
	int i = 0;
	int j;
	int count;

	if ((mask & W_ASTERISK) != 0)
		width = va_arg(ap, int);

	if ((mask & HYPHEN) == 0)
	{
		for (count = 0; count < width - 1; count++)
			i += write(STDOUT_FILENO, " ", 1);
	}

	j = write(STDOUT_FILENO, &c, 1);

	if ((mask & HYPHEN) != 0)
	{
		for (count = 0; count < width - 1; count++)
			i += write(STDOUT_FILENO, " ", 1);
	}

	if (i != -1 && j != -1)
		return (i + j);


	return (-1);
}

/**
 * pr_str - prints a string
 * @ap: list of variadic input
 * @mask: indicates flags
 * @width: field width (spacing)
 * @precis: precision (digits)
 *
 * Return: length of string
 */
int pr_str(va_list ap, int mask, int width, int precis)
{
	char *str;
	int len;
	int count;
	int i = 0;
	int j;

	if ((mask & W_ASTERISK) != 0)
		width = va_arg(ap, int);

	if ((mask & P_ASTERISK) != 0)
		precis = va_arg(ap, int);

	str = va_arg(ap, char *);
	if (str == NULL)
		return (write(STDOUT_FILENO, "(null)", 6));

	len = _strlen(str);

	if ((mask & PRECISION) != 0 && precis >= 0 && precis < len)
		len = precis;

	if ((mask & HYPHEN) == 0)
	{
		for (count = 0; count < width - len; count++)
			i += write(STDOUT_FILENO, " ", 1);
	}

	j = write(STDOUT_FILENO, str, len);

	if ((mask & HYPHEN) != 0)
	{
		for (count = 0; count < width - len; count++)
			i += write(STDOUT_FILENO, " ", 1);
	}

	if (i != -1 && j != -1)
		return (i + j);

	return (-1);
}

/**
 * pr_int - prints an int
 * @ap: list of variadic input
 * @mask: indicates flags
 * @width: field width (spacing)
 * @precis: precision (digits)
 *
 * Return: digits in int that are printed
 */
int pr_int(va_list ap, int mask, int width, int precis)
{
	int i_var;
	long int li_var;

	int digits;
	int i = 0;
	int j;
	int len;
	int is_neg = FALSE;
	char *int_buf;
	char *tmp0;
	char *tmp1;

	if (mask_check(mask, ZERO) == TRUE &&
	    (mask_check(mask, HYPHEN) == TRUE ||
	     mask_check(mask, PRECISION) == TRUE))
	{
		mask = mask ^ ZERO;
	}

	if (mask_check(mask, W_ASTERISK) == TRUE)
		width = va_arg(ap, int);

	if (mask_check(mask, P_ASTERISK) == TRUE)
		precis = va_arg(ap, int);

	if (mask_check(mask, LONG) == TRUE)
	{
		li_var = va_arg(ap, long int);
		int_buf = li_itoa(li_var);
		if (li_var < 0)
			is_neg = TRUE;
	}
	else
	{
		i_var = va_arg(ap, int);
		int_buf = i_itoa(i_var);
		if (i_var < 0)
			is_neg = TRUE;
	}

	digits = _strlen(int_buf);
	if (is_neg == TRUE)
		digits--;

	if (precis > digits || (mask_check(mask, ZERO) == TRUE && width > digits))
	{
		if (precis > digits)
			len = precis - digits;
		else if (is_neg == TRUE || mask_check(mask, PLUS) == TRUE ||
			 mask_check(mask, SPACE) == TRUE)
			len = width - digits - 1;
		else
			len = width - digits;

		tmp0 = malloc(len + 1);
		_memset(tmp0, len, '0');
		if (is_neg == TRUE)
			tmp1 = str_concat(tmp0, int_buf + 1);
		else
			tmp1 = str_concat(tmp0, int_buf);
		free(tmp0);
		free(int_buf);
		int_buf = tmp1;
		if (is_neg == TRUE)
		{
			int_buf = str_concat("-", tmp1);
			free(tmp1);
		}
	}

	if (is_neg == FALSE && (mask_check(mask, PLUS) == TRUE ||
				mask_check(mask, SPACE) == TRUE))
	{
		if (mask_check(mask, PLUS) == TRUE)
			tmp0 = str_concat("+", int_buf);
		else
			tmp0 = str_concat(" ", int_buf);

		free(int_buf);
		int_buf = tmp0;
	}

	if (mask_check(mask, HYPHEN) == TRUE)
		j = write(STDOUT_FILENO, int_buf, _strlen(int_buf));

	if (width > _strlen(int_buf))
	{
		len = width - _strlen(int_buf);
		tmp0 = malloc(len + 1);
		_memset(tmp0, len, ' ');
		i = write(STDOUT_FILENO, tmp0, len);
		free(tmp0);
	}

	if (mask_check(mask, HYPHEN) == FALSE)
		j = write(STDOUT_FILENO, int_buf, _strlen(int_buf));

	free(int_buf);

	if (i != -1 && j != -1)
		return (i + j);

	return (-1);
}

/**
 * pr_unsigned - prints an unsigned int
 * @ap: list of variadic input
 * @mask: indicates flags
 * @width: field width (spacing)
 * @precis: precision (digits)
 *
 * Return: digits in number that are printed
 */
int pr_unsigned(va_list ap, int mask, int width, int precis)
{
	unsigned int u_var;
	unsigned long int lu_var;

	int digits;
	int i = 0;
	int j;
	int len;
	char *int_buf;
	char *tmp0;
	char *tmp1;

	if (mask_check(mask, ZERO) == TRUE &&
	    (mask_check(mask, HYPHEN) == TRUE ||
	     mask_check(mask, PRECISION) == TRUE))
	{
		mask = mask ^ ZERO;
	}

	if (mask_check(mask, W_ASTERISK) == TRUE)
		width = va_arg(ap, int);

	if (mask_check(mask, P_ASTERISK) == TRUE)
		precis = va_arg(ap, int);

	if (mask_check(mask, LONG) == TRUE)
	{
		lu_var = va_arg(ap, unsigned long int);
		int_buf = lu_itoa(lu_var);
	}
	else
	{
		u_var = va_arg(ap, unsigned int);
		int_buf = u_itoa(u_var);
	}

	digits = _strlen(int_buf);

	if (precis > digits || (mask_check(mask, ZERO) == TRUE && width > digits))
	{
		if (precis > digits)
			len = precis - digits;
		else
			len = width - digits;

		tmp0 = malloc(len + 1);
		_memset(tmp0, len, '0');
		tmp1 = str_concat(tmp0, int_buf);
		free(tmp0);
		free(int_buf);
		int_buf = tmp1;
	}

	if (mask_check(mask, HYPHEN) == TRUE)
		j = write(STDOUT_FILENO, int_buf, _strlen(int_buf));

	if (width > _strlen(int_buf))
	{
		len = width - _strlen(int_buf);
		tmp0 = malloc(len + 1);
		_memset(tmp0, len, ' ');
		i = write(STDOUT_FILENO, tmp0, len);
		free(tmp0);
	}

	if (mask_check(mask, HYPHEN) == FALSE)
		j = write(STDOUT_FILENO, int_buf, _strlen(int_buf));

	free(int_buf);

	if (i != -1 && j != -1)
		return (i + j);

	return (-1);
}

/**
 * pr_format - returns an error code to central function in the case of no conversion specifier
 * @ap: list of variadic input
 * @mask: indicates flags
 * @width: field width (spacing)
 * @precis: precision (digits)
 *
 * Return: -2 (Always Failure)
 */
int pr_format(__attribute__((unused))va_list ap,
	      __attribute__((unused))int mask, __attribute__((unused))int width,
	      __attribute__((unused))int precis)
{
	return (-2);
}
