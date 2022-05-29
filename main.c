#include "structs.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add_last(void **arr, int *len, data_structure *data)
{

  long vector_length = 0; // lungimea totala a tuturor void *data
  head header;
  int i;
  void *aux = *arr;
  for (i = 0; i < *len; i++)
  {
    header = *(head *)(aux);
    aux = aux + sizeof(head) + header.len;
    vector_length += header.len;
  }

  *arr = realloc(*arr, vector_length + (*len) * sizeof(head) + sizeof(head) + data->header->len);
  if (*arr == NULL)
  {
    return -1;
  }

  if (memcpy(*arr + vector_length + (*len) * sizeof(head), data->header,
             sizeof(head)) != *arr + vector_length + (*len) * sizeof(head))
  {
    return -1;
  }
  if (memcpy(*arr + vector_length + (*len) * sizeof(head) + sizeof(head),
             data->data, data->header->len) !=
      *arr + vector_length + (*len) * sizeof(head) + sizeof(head))
  {
    return -1;
  }

  (*len) = (*len) + 1;
  return 0;
}

int add_at(void **arr, int *len, data_structure *data, int index)
{
  long vector_length = 0;
  long index_len = 0;
  head header;
  int i;
  void *aux = *arr;
  for (i = 0; i < index; i++)
  {
    header = *(head *)(aux);
    aux = aux + sizeof(head) + header.len;
    vector_length += header.len;
    index_len += header.len;
  }

  for (i = index; i < *len; i++)
  {
    header = *(head *)(aux);
    aux = aux + sizeof(head) + header.len;
    vector_length += header.len;
  }
  aux = *arr + index * sizeof(head) + index_len;
  size_t size = sizeof(head) * (*len - index) + vector_length - index_len;
  void *copy = malloc(size);
  if (memcpy(copy, aux, size) != copy)
  {
    return -1;
  }
  *arr = realloc(*arr, vector_length + (*len) * sizeof(head) + sizeof(head) +
                           data->header->len);
  if (*arr == NULL)
  {
    free(copy);
    return -1;
  }
  if (memcpy(*arr + index * sizeof(head) + index_len, data->header,
             sizeof(head)) != (*arr + index * sizeof(head) + index_len))
  {
    free(copy);
    return -1;
  }
  if (memcpy(*arr + index * sizeof(head) + index_len + sizeof(head), data->data,
             data->header->len) !=
      *arr + index * sizeof(head) + index_len + sizeof(head))
  {
    free(copy);
    return -1;
  }

  if (memcpy(*arr + index * sizeof(head) + index_len + sizeof(head) +
                 data->header->len,
             copy, size) != *arr + index * sizeof(head) + index_len +
                                sizeof(head) + data->header->len)
  {
    free(copy);
    return -1;
  }

  (*len) = (*len) + 1;
  free(copy);
  return 0;
}

void find(void *data_block, int len, int index)
{
  int i;
  head header;
  void *data;
  char *dedicator;

  for (i = 0; i < index; i++)
  {
    header = *(head *)data_block;
    data_block = data_block + sizeof(head) + header.len;
  }

  header = *(head *)data_block;
  data = data_block + sizeof(head);
  dedicator = (char *)data;
  printf("Tipul %hhu\n", header.type);
  switch (header.type)
  {
  case 1:

    printf("%s pentru %s\n", dedicator,
           (char *)(data + strlen(dedicator) + 1 + 2 * sizeof(int8_t)));
    printf("%" PRId8 "\n", *(int8_t *)(data + strlen(dedicator) + 1));
    printf("%" PRId8 "\n\n",
           *(int8_t *)(data + strlen(dedicator) + 1 + sizeof(int8_t)));
    break;
  case 2:

    printf("%s pentru %s\n", dedicator,
           (char *)(data + strlen(dedicator) + 1 + sizeof(int16_t) +
                    sizeof(int32_t)));
    printf("%" PRId16 "\n", *(int16_t *)(data + strlen(dedicator) + 1));
    printf("%" PRId32 "\n\n",
           *(int32_t *)(data + strlen(dedicator) + 1 + sizeof(int16_t)));
    break;
  case 3:

    printf("%s pentru %s\n", dedicator,
           (char *)(data + strlen(dedicator) + 1 + sizeof(int32_t) +
                    sizeof(int32_t)));
    printf("%" PRId32 "\n", *(int32_t *)(data + strlen(dedicator) + 1));
    printf("%" PRId32 "\n\n",
           *(int32_t *)(data + strlen(dedicator) + 1 + sizeof(int32_t)));
    break;

  default:
    break;
  }
}

int delete_at(void **arr, int *len, int index)
{
  long vector_length = 0; // lungimea totala a tuturor void *data
  long len_to_index = 0;
  // lungimea totala a tuturor void *data pana la index inclusiv
  unsigned int index_len; // lungimea void *data pentru index
  head header;
  int i;
  void *aux = *arr;

  if ((*len) == 1)
  {
    free(*arr);
    *arr = NULL;
    (*len) = 0;
    return 0;
  }

  for (i = 0; i <= index; i++)
  {
    header = *(head *)(aux);
    aux = aux + sizeof(head) + header.len;
    vector_length += header.len;
    len_to_index += header.len;
  }
  index_len = header.len; // ultimul header din for reprezinta indexul

  for (i = index + 1; i < *len; i++)
  {
    header = *(head *)(aux);
    aux = aux + sizeof(head) + header.len;
    vector_length += header.len;
  }
  aux = *arr + (index + 1) * sizeof(head) + len_to_index;
  if (memcpy(aux - sizeof(head) - index_len, aux,
             sizeof(head) * ((*len) - 1 - index) + vector_length -
                 len_to_index) != aux - sizeof(head) - index_len)
  {
    return -1;
  }

  *arr = realloc(*arr, sizeof(head) * ((*len) - 1) + vector_length - index_len);
  if (*arr == NULL)
  {
    return -1;
  }
  (*len) = (*len) - 1;
  return 0;
}
void print(void *arr, int len)
{
  int i;
  void *aux = arr;
  head header;
  char *dedicator;
  void *data;
  for (i = 0; i < len; i++)
  {
    header = *(head *)aux;
    data = aux + sizeof(head);
    dedicator = (char *)data;
    printf("Tipul %hhu\n", header.type);
    switch (header.type)
    {
    case 1:

      printf("%s pentru %s\n", dedicator,
             (char *)(data + strlen(dedicator) + 1 + 2 * sizeof(int8_t)));
      printf("%" PRId8 "\n", *(int8_t *)(data + strlen(dedicator) + 1));
      printf("%" PRId8 "\n\n",
             *(int8_t *)(data + strlen(dedicator) + 1 + sizeof(int8_t)));
      break;
    case 2:

      printf("%s pentru %s\n", dedicator,
             (char *)(data + strlen(dedicator) + 1 + sizeof(int16_t) +
                      sizeof(int32_t)));
      printf("%" PRId16 "\n", *(int16_t *)(data + strlen(dedicator) + 1));
      printf("%" PRId32 "\n\n",
             *(int32_t *)(data + strlen(dedicator) + 1 + sizeof(int16_t)));
      break;
    case 3:

      printf("%s pentru %s\n", dedicator,
             (char *)(data + strlen(dedicator) + 1 + sizeof(int32_t) +
                      sizeof(int32_t)));
      printf("%" PRId32 "\n", *(int32_t *)(data + strlen(dedicator) + 1));
      printf("%" PRId32 "\n\n",
             *(int32_t *)(data + strlen(dedicator) + 1 + sizeof(int32_t)));
      break;

    default:
      break;
    }

    aux = aux + sizeof(head) + header.len;
  }
}

int main()
{
  // the vector of bytes u have to work with
  // good luck :)
  void *arr = NULL;
  int len = 0;
  char *delimitator;
  int tip;
  char *dedicator;
  char *dedicat;
  head *header;
  void *data;
  data_structure *structura;
  int index;
  int exit = 0;

  size_t lenline = 256;
  // char *input = (char *)malloc(lenline * sizeof(char));
  char *input = NULL;

  while (getline(&input, &lenline, stdin) != 0)
  {

    delimitator = strtok(input, " \n");
    if (strcmp(delimitator, "insert") == 0)
    {
      delimitator = strtok(NULL, " ");

      tip = atoi(delimitator);

      switch (tip)
      {
      case 1:
        dedicator = strtok(NULL, " ");
        int8_t suma1 = atoi(strtok(NULL, " "));
        int8_t suma2 = atoi(strtok(NULL, " "));
        dedicat = strtok(NULL, " \n");
        header = (head *)malloc(sizeof(head));
        header->type = tip;
        header->len =
            strlen(dedicator) + strlen(dedicat) + 2 * sizeof(int8_t) + 2;
        data = (void *)malloc(header->len * 1);

        strcpy(data, dedicator);
        //	*(data+strlen(dedicator))=suma1;
        memcpy(data + strlen(dedicator) + 1, &suma1, sizeof(int8_t));
        memcpy(data + strlen(dedicator) + 1 + sizeof(int8_t), &suma2,
               sizeof(int8_t));
        //	memcpy(data+strlen(dedicator)+1+sizeof(int8_t)+sizeof(int8_t),
        //dedicat, strlen(dedicat));
        strcpy(data + strlen(dedicator) + 1 + sizeof(int8_t) + sizeof(int8_t),
               dedicat);
        structura = malloc(sizeof(data_structure));
        structura->header = header;
        structura->data = data;
        if (add_last(&arr, &len, structura) != 0)
        {
          printf("nu s-a putut efectua insert-ul");
          exit = 1;
        }
        break;
      case 2:
        dedicator = strtok(NULL, " ");
        int16_t suma21 = atoi(strtok(NULL, " "));
        int32_t suma22 = atoi(strtok(NULL, " "));
        dedicat = strtok(NULL, " \n");
        header = malloc(sizeof(head));
        header->type = tip;
        header->len = strlen(dedicator) + strlen(dedicat) + sizeof(int16_t) +
                      sizeof(int32_t) + 2;
        data = malloc(header->len * 1);
        strcpy(data, dedicator);
        //	*(data+strlen(dedicator))=suma1;
        memcpy(data + strlen(dedicator) + 1, &suma21, sizeof(int16_t));
        memcpy(data + strlen(dedicator) + 1 + sizeof(int16_t), &suma22,
               sizeof(int32_t));
        // memcpy(data+strlen(dedicator)+sizeof(int16_t)+sizeof(int32_t),
        // dedicat, strlen(dedicat));
        strcpy(data + strlen(dedicator) + 1 + sizeof(int16_t) + sizeof(int32_t),
               dedicat);
        structura = malloc(sizeof(data_structure));
        structura->header = header;
        structura->data = data;
        if (add_last(&arr, &len, structura) != 0)
        {
          printf("nu s-a putut efectua insert-ul");
          exit = 1;
        }
        break;
      case 3:
        dedicator = strtok(NULL, " ");
        int32_t suma31 = atoi(strtok(NULL, " "));
        int32_t suma32 = atoi(strtok(NULL, " "));
        dedicat = strtok(NULL, " \n");
        header = malloc(sizeof(head));
        header->type = tip;
        header->len =
            strlen(dedicator) + strlen(dedicat) + 2 * sizeof(int32_t) + 2;
        data = malloc(header->len * 1);
        strcpy(data, dedicator);
        //	*(data+strlen(dedicator))=suma1;
        memcpy(data + strlen(dedicator) + 1, &suma31, sizeof(int32_t));
        memcpy(data + strlen(dedicator) + 1 + sizeof(int32_t), &suma32,
               sizeof(int32_t));
        // memcpy(data+strlen(dedicator)+sizeof(int32_t)+sizeof(int32_t),
        // dedicat, strlen(dedicat));
        strcpy(data + strlen(dedicator) + 1 + sizeof(int32_t) + sizeof(int32_t),
               dedicat);
        structura = malloc(sizeof(data_structure));
        structura->header = header;
        structura->data = data;
        if (add_last(&arr, &len, structura) != 0)
        {
          printf("nu s-a putut efectua insert-ul");
          exit = 1;
        }
        break;

      default:

        break;
      }
    }

    else if (strcmp(delimitator, "insert_at") == 0)
    {
      delimitator = strtok(NULL, " ");

      index = atoi(delimitator);
      delimitator = strtok(NULL, " ");

      tip = atoi(delimitator);
      switch (tip)
      {
      case 1:
        dedicator = strtok(NULL, " ");
        int8_t suma1 = atoi(strtok(NULL, " "));
        int8_t suma2 = atoi(strtok(NULL, " "));
        dedicat = strtok(NULL, " \n");
        header = malloc(sizeof(head));
        header->type = tip;
        header->len =
            strlen(dedicator) + strlen(dedicat) + 2 * sizeof(int8_t) + 2;
        data = malloc(header->len * 1);
        strcpy(data, dedicator);
        //	*(data+strlen(dedicator))=suma1;
        memcpy(data + strlen(dedicator) + 1, &suma1, sizeof(int8_t));
        memcpy(data + strlen(dedicator) + 1 + sizeof(int8_t), &suma2,
               sizeof(int8_t));
        //	memcpy(data+strlen(dedicator)+1+sizeof(int8_t)+sizeof(int8_t),
        //dedicat, strlen(dedicat));
        strcpy(data + strlen(dedicator) + 1 + sizeof(int8_t) + sizeof(int8_t),
               dedicat);
        structura = malloc(sizeof(data_structure));
        structura->header = header;
        structura->data = data;
        ////nou:

        if (index < 0)
        {
          printf("nu s-a putut efectua insert-ul");
          exit = 1;
        }

        else if (index >= len)
        {
          add_last(&arr, &len, structura);
        }
        else if (index >= 0 || index < len)
        {
          add_at(&arr, &len, structura, index);
        }

        break;
      case 2:

        dedicator = strtok(NULL, " ");
        int16_t suma21 = atoi(strtok(NULL, " "));
        int32_t suma22 = atoi(strtok(NULL, " "));
        dedicat = strtok(NULL, " \n");
        header = malloc(sizeof(head));
        header->type = tip;
        header->len = strlen(dedicator) + strlen(dedicat) + sizeof(int16_t) +
                      sizeof(int32_t) + 2;
        data = malloc(header->len * 1);
        strcpy(data, dedicator);
        memcpy(data + strlen(dedicator) + 1, &suma21, sizeof(int16_t));
        memcpy(data + strlen(dedicator) + 1 + sizeof(int16_t), &suma22,
               sizeof(int32_t));
        // memcpy(data+strlen(dedicator)+sizeof(int16_t)+sizeof(int32_t),
        // dedicat, strlen(dedicat));
        strcpy(data + strlen(dedicator) + 1 + sizeof(int16_t) + sizeof(int32_t),
               dedicat);
        structura = malloc(sizeof(data_structure));
        structura->header = header;
        structura->data = data;
        ////nou:

        if (index < 0)
        {
          printf("nu s-a putut efectua insert-ul");
          exit = 1;
        }

        else if (index >= len)
        {
          add_last(&arr, &len, structura);
        }
        else if (index >= 0 || index < len)
        {
          add_at(&arr, &len, structura, index);
        }
        break;
      case 3:
        dedicator = strtok(NULL, " ");
        int32_t suma31 = atoi(strtok(NULL, " "));
        int32_t suma32 = atoi(strtok(NULL, " "));
        dedicat = strtok(NULL, " \n");
        header = malloc(sizeof(head));
        header->type = tip;
        header->len =
            strlen(dedicator) + strlen(dedicat) + 2 * sizeof(int32_t) + 2;
        data = malloc(header->len * 1);
        strcpy(data, dedicator);
        memcpy(data + strlen(dedicator) + 1, &suma31, sizeof(int32_t));
        memcpy(data + strlen(dedicator) + 1 + sizeof(int32_t), &suma32,
               sizeof(int32_t));
        strcpy(data + strlen(dedicator) + 1 + sizeof(int32_t) + sizeof(int32_t),
               dedicat);
        structura = malloc(sizeof(data_structure));
        structura->header = header;
        structura->data = data;

        if (index < 0)
        {
          printf("nu s-a putut efectua insert-ul");
          exit = 1;
        }
        else if (index >= len)
        {
          add_last(&arr, &len, structura);
        }
        else if (index >= 0 || index < len)
        {
          add_at(&arr, &len, structura, index);
        }
        break;

      default:
        break;
      }
    }

    else if (strcmp(delimitator, "find") == 0)
    {
      delimitator = strtok(NULL, " ");

      index = atoi(delimitator);

      if (index >= 0 && index < len)
      {
        find(arr, len, index);
      }
    }

    else if (strcmp(delimitator, "delete_at") == 0)
    {
      delimitator = strtok(NULL, " ");

      index = atoi(delimitator);
      //	printf("%d index", index);

      if (index < 0 || index >= len)
      {
        printf("eroare");
        exit = 1;
      }
      else if (index >= 0 && index < len)
      {
        if (delete_at(&arr, &len, index) != 0)
        {
          printf("eroare stergere");
        }
      }
    }

    else if (strcmp(delimitator, "print") == 0)
    {
      print(arr, len);
    }

    if (strcmp(delimitator, "exit") == 0 || exit == 1)
    {
      break;
    }

    if (data != NULL)
      free(data);
    if (header != NULL)
      free(header);
    if (structura != NULL)
      free(structura);
  }
  if (arr != NULL)
  {
    free(arr);
  }

  if (data != NULL)
    free(data);
  if (header != NULL)
    free(header);
  if (structura != NULL)
    free(structura);
  free(input);

  return 0;
}
