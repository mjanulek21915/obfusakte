#include "obfuscate.h"

void enc_rot_x(char *c, t_cell **pw_a, t_cell **pw_b)
{
	int mult;
	int div;
	int rot;
	int rst;
	int temp;
	char a = (*pw_a)->c;
	char b = (*pw_b)->c;

	mult = a - b;

	if (!mult)
		mult = 1;

	div = (((a * b) + mult) % DIV_RNG) + DIV_RMD;

	rot = (((a * b * mult) + mult) % div) + ROT_RMD;

	*pw_a = (*pw_a)->next;
	*pw_b = (*pw_b)->next;
fprintf(stderr, "enc_rot_x:\nrot = %d\nchar = %d\n", rot, (int)*c); 
	temp = (*c + rot) % 128;
	if (temp < 0)
		temp = temp + 128;
	*c = temp;
fprintf(stderr, " char = %d\n\n", (int)*c);

}

void dec_rot_x(char *c, t_cell **pw_a, t_cell **pw_b)
{
	int mult;
	int div;
	int rot;
	int rst;
	int temp;
	char a = (*pw_a)->c;
	char b = (*pw_b)->c;

	mult = a - b;

	if (!mult)
		mult = 1;

	div = (((a * b) + mult) % DIV_RNG) + DIV_RMD;

	rot = ((a * b * mult) + mult) % div + ROT_RMD;
fprintf(stderr, "dec_rot_x:\nrot = %d\nchar = %d\n", rot, (int)*c);

	*pw_a = (*pw_a)->next;
	*pw_b = (*pw_b)->next;

	// *c = (*c - rot - 32) % 91;
fprintf(stderr, " char = %d\n\n", (int)*c);
	temp = (*c - rot) % 128;
	if (temp < 0)
		temp = temp + 128;
	*c = temp;
fprintf(stderr, " char = %d\n\n", (int)*c);

}

void enc_shuffle(char *str, int pos, int len, t_cell **pw_a, t_cell **pw_b)
{
	int tgt;
	char temp;

	tgt = (pos + ((((*pw_a)->c - (*pw_b)->c) % SHUFFLE_RNG) + SHUFFLE_RMD)) % len;	
	if (tgt < 0)
		tgt = tgt + len;
// fprintf(stderr, "enc_shuffle:\npos = %d\nlen = %d\n\n", pos, len);
	temp = str[pos];
	str[pos] = str[tgt];
	str[tgt] = temp;

	*pw_a = (*pw_a)->previous;
	*pw_b = (*pw_b)->previous;
}

void dec_shuffle(char *str, int pos, int len, t_cell **pw_a, t_cell **pw_b)
{
	int tgt;
	char temp;

	tgt = (pos + ((((*pw_a)->c - (*pw_b)->c) % SHUFFLE_RNG) + SHUFFLE_RMD)) % len;
	
	if (tgt < 0)
		tgt = tgt + len;
// fprintf(stderr, "dec_shuffle:\npos = %d\nlen = %d\n\n", pos, len);
	temp = str[pos];

	str[pos] = str[tgt];
	str[tgt] = temp;

	*pw_a = (*pw_a)->next;
	*pw_b = (*pw_b)->next;

}



void encode(t_data *data)
{
	char *str = data->text.str;

	int len = data->text.len;
	int pos = 0;

	t_cell *pw_1 = data->pw1_list;
	t_cell *pw_2 = data->pw2_list;

	int i = 0;

	while (pos < len)
	{
		enc_rot_x(str + pos, &pw_1, &pw_2);
		pos++;
	}

	pw_1 = pw_1->previous;
	pw_2 = pw_2->previous;

	while (pos--)
		enc_shuffle(str, pos, len, &pw_1, &pw_2);
}

void decode(t_data *data)
{
	char *str = data->text.str;

	int len = data->text.len;
	int pos = 0;

	t_cell *pw_1 = data->pw1_list;
	t_cell *pw_2 = data->pw2_list;

	int i = 0;

	while (pos < len)
	{
		dec_shuffle(str, pos, len, &pw_1, &pw_2);
		pos++;
	}

	pw_1 = data->pw1_list;
	pw_2 = data->pw2_list;
	pos = 0;

	while (pos < len)
	{
		dec_rot_x(str + pos, &pw_1, &pw_2);
		pos++;
	}
}
