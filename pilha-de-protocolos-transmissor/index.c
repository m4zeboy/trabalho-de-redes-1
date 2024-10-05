#include <stdio.h>

typedef struct byte
{
  short data[8];
  short size;
} Byte;

void high_level_layer(char *input)
{
  scanf("%[^\n]", input);
}

int mid_level_layer(char *input, int *output)
{
  int i, size;
  i = 0;
  size = 0;
  while (input[i] != '\0')
  {
    size++;
    output[i] = input[i];
    i++;
  }

  return size;
}

/* return the size of array */
short int_to_binary(int input, short *output)
{
  int i = 0;
  while (input > 0)
  {
    output[i] = input % 2;
    input = input / 2;
    i++;
  }

  return i - 1;
}

void low_level_layer(int *input, int input_size, Byte output[80])
{
  int i;
  for (i = 0; i < input_size; i++)
  {
    output[i].size = int_to_binary(input[i], output[i].data);
  }
}

int main(void)
{
  char high_level_data[80];

  int mid_level_data[80];

  Byte low_level_data[80];

  int i, size;
  Byte size_in_binary;

  high_level_layer(high_level_data);

  size = mid_level_layer(high_level_data, mid_level_data);

  size_in_binary.size = int_to_binary(size, size_in_binary.data);

  low_level_layer(mid_level_data, size, low_level_data);

  for (i = size_in_binary.size; i >= 0; i--)
  {
    printf("%d", size_in_binary.data[i]);
  }
  printf(" ");

  for (i = 0; i < size; i++)
  {
    int j;
    for (j = low_level_data[i].size; j >= 0; j--)
    {
      printf("%d", low_level_data[i].data[j]);
    }
    printf(" ");
  }

  return 0;
}