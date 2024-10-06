#include <stdio.h>
#include <stdlib.h>

typedef struct byte
{
  short data[8];
  short size;
} Byte;

int power(int x, int n)
{
  if (n == 0)
  {
    return 1;
  }
  return x * power(x, n - 1);
}

int receive_low_level_layer(Byte low__level_layer_data[80])
{
  int i, j, k;
  char input[720];
  scanf("%[^\n]", input);
  i = 0;
  j = 0;
  k = 0;
  while (input[i] != '\0')
  {
    if (input[i] == ' ')
    {
      low__level_layer_data[j].size = k;
      j++;
      k = 0;
    }
    else
    {
      low__level_layer_data[j].data[k] = input[i] - '0';
      k++;
    }
    i++;
  }
  low__level_layer_data[j].size = k;
  return j + 1;
}

void mid_level_layer(Byte input[80], int size, int output[80])
{
  int i, j, pow, sum;

  for (i = 0; i < size; i++)
  {
    pow = 0;
    sum = 0;
    for (j = input[i].size - 1; j >= 0; j--)
    {
      sum += input[i].data[j] * power(2, pow);
      pow++;
    }
    output[i] = sum;
  }
}

void high_level_layer(int input[80], int size, char output[80])
{
  int i;
  for (i = 0; i < size; i++)
  {
    output[i] = input[i];
  }
  output[i] = '\0';
}

int main(void)
{
  int size, i, j;
  Byte low__level_layer_data[80];
  int mid__level_layer_data[80];
  char high__level_layer_data[80];

  for (i = 0; i < 80; i++)
  {
    low__level_layer_data[i].size = 0;
    for (j = 0; j < 8; j++)
    {
      low__level_layer_data[i].data[j] = '\0';
    }
  }

  size = receive_low_level_layer(low__level_layer_data);

  mid_level_layer(low__level_layer_data, size, mid__level_layer_data);

  high_level_layer(mid__level_layer_data, size, high__level_layer_data);

  printf("%s", high__level_layer_data);
  return 0;
}