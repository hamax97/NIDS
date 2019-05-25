#include <iostream>
#include <string>
#include <map>
#include <pthread.h>
#include "data_preparation.h"

void*
encode(void* args) {

  arguments *data = (arguments*) args;

  for (int i = 0; i < data->num_rows; ++i) {
    std::string key = data->matrix[i][data->column];
    data->matrix[i][data->column] =
      std::to_string(data->unique_values[key]);
  }

  pthread_exit(NULL);
}

void
encode(std::string** matrix, const int num_rows, const int column,
       std::map<std::string, int>& unique_values) {

  for (int i = 0; i < num_rows; ++i) {
    std::string key = matrix[i][column];
    matrix[i][column] = std::to_string(unique_values[key]);
  }
}
