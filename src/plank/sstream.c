#ifndef AMALGAMATION
  #include "sstream.h"
  #include "palloc.h"
#endif

#include <string.h>
#include <stdio.h>

size_t _sstream_next_po2(size_t len)
{
  size_t curr = 2;

  while(1)
  {
    if(curr > len)
    {
      return curr;
    }

    curr = curr * 2;
  }

  abort();
}

void _sstream_ensure_space(struct sstream *ctx, size_t required)
{
  size_t npo2 = 0;

  if(ctx->total >= required + 1)
  {
    return;
  }

  npo2 = _sstream_next_po2(required + 1);

  if(ctx->data == ctx->initial)
  {
    ctx->data = malloc(sizeof(char) * npo2);
    ctx->data[0] = '\0';

    if(ctx->len > 0)
    {
      strcpy(ctx->data, ctx->initial);
    }
  }
  else
  {
    ctx->data = realloc(ctx->data, sizeof(char) * npo2);

    if(!ctx->data)
    {
      abort();
    }
  }

  ctx->total = npo2;
}

struct sstream *sstream_new()
{
  struct sstream *rtn = NULL;

  rtn = palloc(struct sstream);
  rtn->data = rtn->initial;
  rtn->len = 0;
  rtn->total = SSTREAM_INITIAL_LEN;

  return rtn;
}

struct sstream *sstream_copy(struct sstream *ctx)
{
  struct sstream *rtn = NULL;

  rtn = sstream_new();
  sstream_push_cstr(rtn, sstream_cstr(ctx));

  return rtn;
}

void sstream_clear(struct sstream *ctx)
{
  ctx->data[0] = '\0';
  ctx->len = 0;
}

void sstream_delete(struct sstream *ctx)
{
  if(ctx->data != NULL && ctx->data != ctx->initial)
  {
    free(ctx->data);
  }

  pfree(ctx);
}

void sstream_push_int(struct sstream *ctx, int val)
{
  char buffer[128] = {0};

  sprintf(buffer, "%i", val);
  sstream_push_cstr(ctx, buffer);
}

void sstream_push_float(struct sstream *ctx, float val)
{
  char buffer[128] = {0};

  sprintf(buffer, "%f", val);
  sstream_push_cstr(ctx, buffer);
}

void sstream_push_double(struct sstream *ctx, double val)
{
  char buffer[128] = {0};

  sprintf(buffer, "%f", val);
  sstream_push_cstr(ctx, buffer);
}

void sstream_push_char(struct sstream *ctx, char val)
{
  _sstream_ensure_space(ctx, ctx->len + 1);
  ctx->data[ctx->len] = val;
  ctx->data[ctx->len + 1] = '\0';
  ctx->len++;
}

void sstream_push_cstr(struct sstream *ctx, const char *s)
{
  size_t len = 0;

  len = strlen(s);
  _sstream_ensure_space(ctx, ctx->len + len);
  memcpy(ctx->data + ctx->len, s, len + 1);
  ctx->len += len;
}

char *sstream_cstr(struct sstream *ctx)
{
  return ctx->data;
}

size_t sstream_length(struct sstream *ctx)
{
  return ctx->len;
}

int sstream_int(struct sstream *ctx)
{
  return atoi(ctx->data);
}

char sstream_at(struct sstream *ctx, size_t i)
{
  if(i >= ctx->len)
  {
    printf("Error: Index out of bounds\n");
    abort();
  }

  return ctx->data[i];
}

void sstream_set(struct sstream *ctx, size_t i, char c)
{
  if(i >= ctx->len)
  {
    printf("Error: Index out of bounds\n");
    abort();
  }

  ctx->data[i] = c;
}

void sstream_push_chars(struct sstream *ctx, char *values, size_t count)
{
  char *tmp = NULL;

  tmp = (char *)malloc(sizeof(char) * (count + 1));
  tmp[count] = 0;
  memcpy(tmp, values, sizeof(char) * count);
  sstream_push_cstr(ctx, tmp);
  /* TODO: Allocate size and use memcpy */

  free(tmp);
}

/*
void sstream_split(struct sstream *ctx, char token,
  vector(struct sstream*) *out)
{
  size_t i = 0;
  struct sstream *curr = NULL;

  curr = sstream_new();

  for(i = 0; i < sstream_length(ctx); i++)
  {
    if(sstream_at(ctx, i) == token)
    {
      vector_push_back(out, curr);
      curr = sstream_new();
    }
    else
    {
      sstream_push_char(curr, sstream_at(ctx, i));
    }
  }

  if(sstream_length(curr) > 0)
  {
    vector_push_back(out, curr);
  }
  else
  {
    sstream_delete(curr);
  }
}
*/
