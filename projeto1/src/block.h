#ifndef _BMP_H_
#define _BMP_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  double content[8][8]; /* Reserved */
  int block_height;
  int block_width;
} Block;

