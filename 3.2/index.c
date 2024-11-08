#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 50000
#define FLAG "01111110"
#define ESCAPE "01111101"
#define ADDRESS_SIZE 8
#define CONTROL_SIZE 8
#define PROTOCOL_SIZE 16
#define CHECKSUM_SIZE 16
#define MAX_FRAME_SIZE 1518 * 8

void readInput(unsigned char *stream)
{
  fgets((char *)stream, MAX_SIZE, stdin);
}

unsigned char *getContentBetween(unsigned char *start, unsigned char *end)
{
  size_t size;
  unsigned char *result;
  if (start == NULL || end == NULL || end < start)
  {
    return NULL;
  }

  size = end - start;
  result = (unsigned char *)malloc(size + 1);

  if (result == NULL)
  {
    return NULL;
  }

  memcpy(result, start, size);
  result[size] = '\0';

  return result;
}

int extractFrames(unsigned char *input, unsigned char *frames[100])
{
  unsigned char *current, *end, *possibleFrame;
  short frameIndex = 0;
  current = input;
  while (current)
  {
    current = current + 8;
    end = (unsigned char *)strstr((char *)current, FLAG);
    possibleFrame = getContentBetween(current, end);
    current = end;
    if (possibleFrame && strlen((char *)possibleFrame) > 0)
    {
      frames[frameIndex] = possibleFrame;
      frameIndex++;
    }
  }

  return frameIndex;
}

unsigned char *convertBytesToChar(unsigned char *frameBytes)
{
  unsigned char *frame;
  int length;
  length = strlen((char *)frameBytes) / 8;
  frame = (unsigned char *)malloc(length * sizeof(char));
  if (frame != NULL)
  {
    int i;
    for (i = 0; i < length; i++)
    {
      char byte[9];
      unsigned char data;
      char *endptr;
      int j;
      for (j = 0; j < 8; j++)
      {
        byte[j] = frameBytes[(i * 8) + j];
      }
      byte[j] = '\0';
      data = (unsigned char)strtol(byte, &endptr, 2);
      frame[i] = data;
    }
    frame[i] = '\0';
    return frame;
  }
  return NULL;
}

unsigned char *byteDestuffing(unsigned char *payload)
{
  int length, i, index;
  unsigned char *destuffedPayload;
  length = strlen((char *)payload);
  destuffedPayload = (unsigned char *)malloc(length * sizeof(char));
  if (destuffedPayload != NULL)
  {
    i = 0;
    index = 0;
    while (payload[i] != '\0')
    {
      if (payload[i] == 0x7D)
      {
        /* check the next byte and apply XOR with 0x20*/
        i = i + 1;
        destuffedPayload[index++] = payload[i++] ^ 0x20;
      }
      else
      {
        destuffedPayload[index++] = payload[i++];
      }
      destuffedPayload[index] = '\0';
    }
    return destuffedPayload;
  }
  return NULL;
}

int main(void)
{
  unsigned char stream[MAX_SIZE], *frames[100];
  int frameCount, f;
  readInput(stream);
  frameCount = extractFrames(stream, frames);
  for (f = 0; f < frameCount; f++)
  {
    unsigned char *frameBytes, *payload, *start, *end, address, control;
    int frameLength;
    printf("| PPP Frame %d control fields |\n", f);
    printf("%s\n", frames[f]);
    frameBytes = convertBytesToChar(frames[f]);
    frameLength = strlen((char *)frameBytes);

    address = frameBytes[0];
    control = frameBytes[1];
    printf("Address: %d\n", address);
    printf("Control: %d\n", control);

    printf("Protocol: %02X%02X\n", frameBytes[2], frameBytes[3]);
    printf("Checksum: %02X%02X\n", frameBytes[frameLength - 2], frameBytes[frameLength - 1]);
    start = frameBytes + 4;
    end = frameBytes + (frameLength - 2);

    payload = getContentBetween(start, end);
    printf("%s\n", payload);
    payload = byteDestuffing(payload);
    printf("%s\n", payload);
  }
  return 0;
}