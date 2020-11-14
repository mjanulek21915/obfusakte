#include "obfuscate.h"

void do_exit(char *str)
{
	fprintf(stderr, "%s\n", str);
	exit(1);
}

static void parse_do_exit(char *label)
{
	fprintf(stderr, "%s", label);
	exit(1);
}

void data_free_list(t_cell *list)
{
	t_cell *tgt;
	t_cell *current;

	if (!list)
		parse_do_exit("parse_free_list no list error");
	list->previous->next = NULL;
	current = list;
	while (current)
	{
		tgt = current->next;
		free(current);
		current = tgt;
	}
}

void data_free(t_data rst)
{
	if (rst.pw1.str)
		free(rst.pw1.str);
	if (rst.pw2.str)
		free(rst.pw2.str);
	if (rst.text.str)
		free(rst.text.str);
	if (rst.pw1_list)
		data_free_list(rst.pw1_list);
	if (rst.pw2_list)
		data_free_list(rst.pw2_list);
	if (rst.text_list)
		data_free_list(rst.text_list);
}


static void file_do_exit(char *label_1, char *text_1, char *label_2, char *text_2)
{
	if (*text_1)
		fprintf(stderr, "%s%s\n", label_1, text_1);
	if (*text_2);
		fprintf(stderr, "%s%s\n", label_2, text_2);
	exit(1);
}

static int file_get_len(char *path)
{
	int len = 0;
	int fd = open(path, O_RDONLY);
	char buff[BUFF_SIZE];
	int rst = BUFF_SIZE;
	while ((rst = read(fd, buff, BUFF_SIZE)) == BUFF_SIZE)
	{
		if (rst < 0)
			file_do_exit("", "file_get_len fd error", "path = ", path);
		len = len + rst;
	}
	len = len + rst;
	close (fd);
	return (len);
}

static t_char_a file_read(char *path)
{
	int fd = open(path, O_RDONLY);
	t_char_a rst;
	rst.len = file_get_len(path);
	if (!rst.len)
		file_do_exit("", "file_read rst.len = 0", "path = ", path);

	if (!(rst.str = malloc(sizeof(char) * (rst.len + 1))))
		file_do_exit("file_read malloc error", "", "path = ", path);

	if ((rst.len = read(fd, rst.str, rst.len)) != rst.len)
	{
		fprintf(stderr, "file_read error code %d\n", rst.len);
		file_do_exit("", "", "path = ", path);
	}
	rst.str[rst.len] = 0;
	return (rst);
}



static int primarize_get_prints(char *str)
{
	int rst = 0;

	while (*str)
	{
		if (*str > 32 && *str < 127)
			rst = rst + 1;
		str++;
	}
	return (rst);
}

static int primarize_get_prime_is_prime(int n)
{
	int z = 3;
	if (n < 4)
		return (1);
	if (n % 2 == 0)
		return (0);
	while (z < n / 2)
	{
		if (n % z == 0)
			return 0;
		z = z + 2;
	}
	return (1);
}

static int primarize_get_prime(int n)
{
	while (!primarize_get_prime_is_prime(n))
		n = n - 1;
	return (n);
}

static void primarize_strncopy(char *str, char *rst, int len)
{
	while (*str && len)
	{
		if (*str > 32 && *str < 127)
		{
			len--;
			*rst++ = *str;
		}
		str++;
	}
	*rst = 0;
}

static void primarize(t_char_a *str)
{
	char *rst;
	str->len = primarize_get_prime(primarize_get_prints(str->str));

	if (!(rst = malloc(sizeof(char) * (str->len + 1))))
		file_do_exit("", "primarize malloc error", "", "");
	primarize_strncopy(str->str, rst, str->len);
	free(str->str);
	str->str = rst;
}

static t_cell *parse_to_list_create(t_cell *current, char c)
{
	t_cell *new;

	if (!current)
		parse_do_exit("parse_to_list_create no \"current\"");
	if (!(new = malloc(sizeof(t_cell))))
		parse_do_exit("parse_to_list_create malloc error");
	
	new->previous = current;
	new->next = current->next;
	current->next = new;
	new->next->previous = new;
	new->c = c;
	return (new);
}

static t_cell *parse_to_list(char *str)
{
	t_cell *origin;
	t_cell *current;

	if (!str || !*str)
		parse_do_exit("parse_to_list no str error");
	if (!(origin = malloc(sizeof(t_cell))))
		parse_do_exit("parse_to_list malloc error");

	current = origin;
	origin->next = origin;
	origin->previous = origin;
	origin->c = *str++;

	while (*str)
		current = parse_to_list_create(current, *str++);
	return (origin);
}


void data_parse_printout(t_cell *rst, int n)
{
	t_cell *origin;
	t_cell *current;

	if (!rst)
		parse_do_exit("data_parse_printout no list error\n");

	origin = rst;
	current = origin;
	printf("%c", current->c);
	current = current->next;
	if (!n)
	{
		while (current != origin)
		{
			printf("%c", current->c);
			current = current->next;
		}
	}
	else
	{
		if (n > 1)
			n--;
		while (n--)
		{
			printf("%c", current->c);
			current = current->next;
		}
	}
}


void data_init(t_data *rst, char *pw1_path, char *pw2_path, char *text_path)
{
	rst->pw1.str = NULL;
	rst->pw2.str = NULL;
	rst->text.str = NULL;

	rst->pw1 = file_read(pw1_path);
	rst->pw2 = file_read(pw2_path);
	rst->text = file_read(text_path);
	if (!rst->pw1.str || !rst->pw2.str || !rst->text.str || rst->pw1.len < 1 
	|| rst->pw2.len < 1 || rst->text.len < 1)
		file_do_exit("data init", "len error", "", "");
	primarize(&(rst->pw1));
	primarize(&(rst->pw2));
	rst->pw1_list = NULL;
	rst->pw2_list = NULL;
	rst->text_list = NULL;
	rst->pw1_list = parse_to_list(rst->pw1.str);
	rst->pw2_list = parse_to_list(rst->pw2.str);
	rst->text_list = parse_to_list(rst->text.str);
}