#ifndef PALLOC_SSTREAM_H
#define PALLOC_SSTREAM_H

#ifndef AMALGAMATION
  #include "vector.h"
#endif

#include <stdlib.h>

#define SSTREAM_INITIAL_LEN 32

struct sstream
{
  char initial[SSTREAM_INITIAL_LEN];
  char *data;
  size_t len;
  size_t total;
};

typedef struct sstream sstream;

struct sstream *sstream_new();
struct sstream *sstream_copy(struct sstream *ctx);
void sstream_delete(struct sstream *ctx);

void sstream_clear(struct sstream *ctx);
size_t sstream_length(struct sstream *ctx);

void sstream_push_cstr(struct sstream *ctx, const char *s);
void sstream_push_int(struct sstream *ctx, int val);
void sstream_push_float(struct sstream *ctx, float val);
void sstream_push_double(struct sstream *ctx, double val);
void sstream_push_char(struct sstream *ctx, char val);
void sstream_push_chars(struct sstream *ctx, char *values, size_t count);

char sstream_at(struct sstream *ctx, size_t i);
void sstream_set(struct sstream *ctx, size_t i, char c);
char *sstream_cstr(struct sstream *ctx);
int sstream_int(struct sstream *ctx);

void sstream_split(struct sstream *ctx, char token,
  vector(struct sstream*) *out);

size_t _sstream_next_po2(size_t len);
void _sstream_ensure_space(struct sstream *ctx, size_t required);

#endif
