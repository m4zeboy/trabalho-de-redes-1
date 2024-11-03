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

void readInput(char *stream)
{
  fgets(stream, MAX_SIZE, stdin);
}

char *getContentBetween(char *start, char *end)
{
  size_t size;
  char *result;
  if (start == NULL || end == NULL || end < start)
  {
    return NULL;
  }

  size = end - start;
  result = (char *)malloc(size + 1);

  if (result == NULL)
  {
    return NULL;
  }

  memcpy(result, start, size);
  result[size] = '\0';

  return result;
}

int extractFrames(char *input, char *frames[100])
{
  char *start, *current, *end, *possibleFrame;
  short frameIndex = 0;
  current = input;
  while (current)
  {
    start = strstr(current, FLAG);
    current = current + 8;
    end = strstr(current, FLAG);
    possibleFrame = getContentBetween(current, end);
    current = end;
    if (possibleFrame && strlen(possibleFrame) > 0)
    {
      frames[frameIndex] = possibleFrame;
      frameIndex++;
    }
  }

  return frameIndex;
}

char **bitsToBytes(char *payload, int payloadBytesLength)
{
  char **bytes, *start, *end, *byte;
  int i, payloadBitsLength, payloadBytesLength;
  bytes = (char **)malloc(1518 * sizeof(char *));

  start = payload;
  end = payload + 8;
  for (i = 0; i < payloadBytesLength; i++)
  {
    byte = getContentBetween(start, end);
    bytes[i] = byte;
    start = end;
    end = end + 8;
  }
  return bytes;
}

char *deStuffPayload(char *payload)
{
  char **bytes;
  int payloadBitsLength, payloadBytesLength, i;
  payloadBitsLength = strlen(payload);
  payloadBytesLength = payloadBitsLength / 8;

  bytes = bitsToBytes(payload, payloadBytesLength);

  i = 0;
  while (i < payloadBytesLength)
  {
    }
}

void delimitFrame(char *frame)
{

  char *start, *end, *endptr, *addressBinary, *controlBinary, *protocolBinary, *checksumBinary, *payloadBinary, *deStuffedPayload;
  long int address, control, protocol, checksum, frameLength;

  start = frame;
  end = frame + ADDRESS_SIZE;
  /* ADDRESS */
  addressBinary = getContentBetween(start, end);
  address = strtol(addressBinary, &endptr, 2);
  printf("Address: %ld\n", address);

  /* CONTROL*/
  start = end;
  end = end + CONTROL_SIZE;
  controlBinary = getContentBetween(start, end);
  control = strtol(controlBinary, &endptr, 2);
  printf("Control: %ld\n", control);

  /* PROTOCOL*/
  start = end;
  end = end + PROTOCOL_SIZE;
  protocolBinary = getContentBetween(start, end);
  protocol = strtol(protocolBinary, &endptr, 2);
  printf("Control: %lX\n", protocol);

  /* CHECKSUM */
  frameLength = strlen(frame);
  end = frame + frameLength;
  start = end - CHECKSUM_SIZE;
  checksumBinary = getContentBetween(start, end);
  checksum = strtol(checksumBinary, &endptr, 2);
  printf("Checksum: %lX (Binary %s)\n", checksum, checksumBinary);

  /* PAYLOAD */
  end = start;
  start = frame + ADDRESS_SIZE + CONTROL_SIZE + PROTOCOL_SIZE;
  payloadBinary = getContentBetween(start, end);
  deStuffedPayload = deStuffPayload(payloadBinary);
}

int main(void)
{
  char bit_stream[MAX_SIZE], *start, *frames[100];
  short frameCount, f;
  /*
  01111110
  11111111
  00000011
  11000000 00100001
  0101001001100101011001000110010101110011
  00010111 00001111
  01111110
  */
  readInput(bit_stream);
  start = strstr(bit_stream, FLAG);
  frameCount = extractFrames(bit_stream, frames);
  printf("f: %hd\n", frameCount);
  for (f = 0; f < frameCount; f++)
  {
    delimitFrame(frames[f]);
  }
  return 0;
}