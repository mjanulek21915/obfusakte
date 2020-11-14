#include "obfuscate.h"





int main(int ac, char **av)
{
	t_data data;
	data_init(&data, "aaa", "bbb", "ccc");
	printf("---------------------------------main : \n[%s]\n---------------------------------\n", data.text.str);
	encode(&data);
	// printf("%s\n", data.pw1.str);
	printf("---------------------------------main : \n[%s]\n---------------------------------\n", data.text.str);
	decode(&data);
	// printf("%s\n", data.pw2.str);
	printf("---------------------------------main : \n[%s]\n---------------------------------\n", data.text.str);
	// data_parse_printout(data.text_list, 0);
	data_free(data);


}
