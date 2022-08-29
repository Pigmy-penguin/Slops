/*
   Copyright 2022 Pigmy-penguin

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <kernel/types.h>

int strlen(char *str)
{
	int len = 0;

	while (*str++)
		len++;

	return len;
}

int strcmp(char *str1, char *str2)
{
	int i = 0;
	int failed = 0;
	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i]) {
			failed = 1;
			break;
		}
		i++;
	}

	if ((str1[i] == '\0' && str2[i] != '\0')
	    || (str1[i] != '\0' && str2[i] == '\0'))
		failed = 1;

	return failed;
}

char *strcpy(char *dest, const char *src)
{
	do {
		*dest++ = *src++;
	} while (*src != 0);
	return dest;
}

char *strcat(char *dest, const char *src)
{
	while (*dest != 0) {
		dest++;
	}

	do {
		*dest++ = *src++;
	} while (*src != 0);
	return dest;
}

void memset(void *dest, u8 val, u32 len)
{
	u8 *temp = (u8 *) dest;
	for (; len != 0; len--)
		*temp++ = val;
}

void memcpy(void *dest, const void *src, u32 len)
{
	const u8 *sp = (const u8 *)src;
	u8 *dp = (u8 *) dest;
	for (; len != 0; len--)
		*dp++ = *sp++;
}

void itoa(char *buf, u64 base, u64 d)
{
	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	u64 divisor = 10;

	if (base == 'd' && d < 0) {
		*p++ = '-';
		buf++;
		ud = -d;
	} else if (base == 'x') {
		divisor = 16;
	}

	do {
		u64 remainder = ud % divisor;

		*p++ =
		    (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
	} while (ud /= divisor);

	*p = 0;

	//Reverse BUF.
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}
