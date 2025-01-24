#define NULL (char *)0x00;
int strlen(const char *str);
int strcmp(char *c1, char *c2);
void memcpy(char *dest, const char *src, unsigned int count);
int get_str_index(char **arr, char *str, int arr_size);
int stoi(char *buffer);
void int_to_hex_string(unsigned int number, char *buffer, int buff_size);
