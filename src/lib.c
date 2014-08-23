/*

    File: ftpproxy/lib.c
    
    Copyright (C) 1999 by Wolfgang Zekoll  <wzk@quietsche-entchen.de>
    Copyright (C) 2000, 2005  Andreas Schoenberg  <asg@ftpproxy.org>

    This source is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2, or (at your option)
    any later version.

    This source is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "lib.h"



void *allocate(size_t size)
{
	void	*p;

	if ((p = malloc(size)) == NULL) {
		fprintf (stderr, "%s: memory allocation error\n", program);
		exit (-1);
		}

	memset(p, 0, size);
	return (p);
}

void *reallocate(void *p, size_t size)
{
	if ((p = realloc(p, size)) == NULL) {
		fprintf (stderr, "%s: memory allocation error\n", program);
		exit (-1);
		}

	return (p);
}

char *strlwr(char *string)
{
	int c;

	while ((c = *string) != 0) {
		*string++ = tolower(c);
		}

	return (string);
}	

char *strupr(char *string)
{
	int c;

	while ((c = *string) != 0) {
		*string++ = toupper(c);
		}

	return (string);
}	

char *skip_ws(char *string)
{
	int c;

	while (isspace(c = *string))
		string++;

	return (string);
}

char *noctrl(char *buffer)
{
	int	len, i;
	unsigned char *p;

	// TODO: Use iscntrl here. iscntrl(32) returns 0 so figure out why space is converted to null byte too.
	if ((p = (unsigned char *) buffer) == NULL)
		return (NULL);

	len = strlen((char *) p);
        for (i=len-1; i>=0; i--) {
		if (p[i] <= 32)
			p[i] = '\0';
		else
			break;
		}

	return ((char *) p);
}

char *get_word(char **from, char *to, int maxlen)
{
	unsigned int c;
	unsigned char *p;
	int	k;

	maxlen -= 2;
	while ((c = **from) != 0  &&  c <= 32)
		*from += 1;

	*(p = (unsigned char *) to) = k = 0;
	while ((c = **from) != 0) {
		if (c == ' '  ||  c == '\t'  ||  c < 32)
			break;

		*from += 1;
		if (k < maxlen)
			p[k++] = c;
		}

	p[k] = 0;
	return (to);
}

char *get_quoted(char **from, int delim, char *to, int max)
{
	unsigned int c;
	int	k;

	to[0] = k = 0;
	max -= 2;
	
	while ((c = **from) != 0) {
		*from += 1;
		if (c == delim)
			break;

		if (k < max)
			to[k++] = c;
		}

	to[k] = 0;
	return (to);
}

int split(char *line, char *ptr[], int sep, int max)
{
	char	*p;
	int	k, c;

	if (line[0] == 0) {
		ptr[0] = line;
		return (0);
		}

	p = line;
	k = 0;
	while (*p != 0	&&  k < max) {
		if (sep == ' ') {
			while ((c = *p) == ' '	||  c == '\t')
				p++;
			}

		ptr[k++] = p;
		if (sep == ' ') {
			while ((c = *p) != 0  &&  c != '\t'  &&  c != ' ')
				p++;
			}
		else {
			while ((c = *p) != 0  &&  c != sep)
				p++;
			}

		if (*p != 0)
			*p++ = 0;
		}

	return (k);
}

char *copy_string(char *y, char *x, int len)
{
	x = skip_ws(x);
	noctrl(x);

	len -= 2;
	if (strlen(x) >= len)
		x[len] = 0;

	if (y != x)
		strcpy(y, x);
		
	return (y);
}

int strpcmp(char *string, char *pattern)
{
	if (*string == 0) {
		if (*pattern == 0)
			return (0);
		else if (*pattern == '*')
			return (strpcmp(string, pattern + 1));
		else
			return (1);
		}

	if (*pattern == '*') {
		while (strpcmp(string, pattern + 1) != 0) {
			string++;
			if (*string == 0) {
				if (*(pattern + 1) != 0)
					return (1);
				else
					return (0);
				}
			}

		return (0);
		}
	else if (*pattern == '?')
		return (strpcmp(string + 1, pattern + 1));
	else if (tolower(*string) == tolower(*pattern))
		return (strpcmp(string + 1, pattern + 1));

	return (1);
}

char *strxchr(char *string, char *delim, int right)
{
	char	*p;

	if (right == 0) {
		int	c;

		p = string;
		while ((c = *p) != 0) {
			if (strchr(delim, c) != NULL)
				return (p);

			p++;
			}
		}
	else {
		p = string + strlen(string) - 1;
		while (p >= string) {
			if (strchr(delim, *p) != NULL)
				return (p);

			p--;
			}
		}

	return (NULL);
}

