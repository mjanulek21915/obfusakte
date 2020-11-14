#ifndef __OBFUSCATE_H__
#define __OBFUSCATE_H__

#define BUFF_SIZE 1000

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define DIV_RNG 128
#define DIV_RMD 32
#define ROT_RMD 18
#define SHUFFLE_RNG 128
#define SHUFFLE_RMD 32

struct							s_cell;
typedef struct s_cell			t_cell;

struct							s_list;
typedef struct s_list			t_list;

typedef struct					s_char_a
{
	char *str;
	int len;
}								t_char_a;

typedef struct					s_data
{
	int dp;
	t_char_a pw1;
	t_char_a pw2;
	t_char_a text;
	t_cell *pw1_list;
	t_cell *pw2_list;
	t_cell *text_list;
}								t_data;

struct							s_cell
{
	t_cell *previous;
	t_cell *next;
	char c;
};

void data_init(t_data *rst, char *pw1_path, char *pw2_path, char *text_path);
void data_free(t_data rst);
void data_free_list(t_cell *list);
void do_exit(char *str);
void data_parse_printout(t_cell *rst, int n);
// void encode(t_data *data, int depth);
void encode(t_data *data);
void decode(t_data *data);


#endif