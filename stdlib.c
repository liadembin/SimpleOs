int strlen(const char *str) {
  int cnt = 0;
  while (str[cnt] != '\0')
    cnt++;
  return cnt;
}
int strcmp(char *c1, char *c2) {
  int p = 0;
  while (c1[p] != '\0' && c2[p] != '\0') {
    if (c1[p] != c2[p++])
      return 0;
  }
  return c1[p] == c2[p] && c2[p] == '\0';
}
void memcpy(char *dest, const char *src, unsigned int count) {
  for (unsigned int i = 0; i < count; i++) {
    *(dest + i) = *(src + i);
  }
}
int get_str_index(char **arr, char *str, int arr_size) {
  for (int i = 0; i < arr_size; i++) {
    if (strcmp(arr[i], str))
      return i;
  }
  return -1;
}
int stoi(char *buffer) {
  int res = 0;
  int ptr = 0;
  while (buffer[ptr] != '\0') {
    res *= 10;
    res += buffer[ptr++] - '0';
  }
  return res;
}
void int_to_hex_string(unsigned int number, char *buffer, int buff_size) {
  for (int i = 0; i < buff_size; i++) {
    buffer[i] = ' ';
  }
  buffer[buff_size - 1] = '\0';
  int index = buff_size - 2;

  if (number == 0) {
    buffer[index--] = '0';
  } else {
    while (number > 0 && index >= 0) {
      int digit = number % 16;
      if (digit < 10) {
        buffer[index] = '0' + digit;
      } else {
        buffer[index] = 'A' + (digit - 10);
      }
      index--;
      number /= 16;
    }
  }

  if (index >= 0) {
    int shift = index + 1;
    for (int i = 0; i < buff_size - shift; i++) {
      buffer[i] = buffer[i + shift];
    }
  }
}
void memset(char *buffer, char value, int size) {
  for (int i = 0; i < size; i++) {
    buffer[i] = value;
  }
}
