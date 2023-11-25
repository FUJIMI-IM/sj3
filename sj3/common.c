/*
 * Copyright (c) 1994  Sony Corporation
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of Sony Corporation
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * from Sony Corporation.
 *
 */

/*
 * $SonyRCSfile: common.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:03:10 $
 */



#include "conversion.h"

int	allways_buff = 0;		
int	no_flush = 0;			
int	HHlevel = 2;
int	Hlevel = 2;			
int	oasys = 0;			
int	flush_conversion;		

Conversion	*current_conversion;
Conversion	*default_conversion;

Conversion *
AllocConversion ()
{
	register Conversion	*c;

	if ((c = (Conversion *)malloc (sizeof(Conversion))) == NULL) {
		fprintf (stderr, "conversion: can't alloc memory\n\r");
		return (0);
	}

	c->Imode = MODE_ZHIRA;
	c->BuffMode = BUFF;
	c->ha_point = 0;
	c->out_point = 0;
	c->span_point = 0;
	c->word_regist = 0;
	c->Cflag = 1;

	c->SavedRow = 0;
	c->SavedCol = 0;
	c->CurrentVcol = 0;
	c->MaxVcol = 0;
	c->wrap = 1;
	return (c);
}

static Conversion	*push_conversion;

FreeConversion (c)
Conversion	*c;
{
	if (c == push_conversion)
		push_conversion = 0;
	free (c);
}

SetConversion (c)
Conversion	*c;
{
	PushConversion (current_conversion);
	current_conversion = c;
}

PushConversion (c)
Conversion	*c;
{
	push_conversion = c;
}

PopConversion ()
{
	if (push_conversion)
		current_conversion = push_conversion;
}

Conversion *
GetConversion ()
{
	return (current_conversion);
}

InitConversion ()
{
	default_conversion = AllocConversion ();
	DefaultConversion ();
}

DefaultConversion ()
{
	current_conversion = default_conversion;
}

EndConversion ()
{
	free (default_conversion);
}

int
wcbyte(str)
register wchar16_t *str;
{
	register count=0;

	while (*str) {
		if (WcHighMASK & *str++) 
		        count += 2;
		else
		        count++;
	}
	return (count);
}
