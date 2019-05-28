#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <random>
#include <pthread.h>
#include "data_preparation.h"

void*
encode(void* args) {

  arguments* data = (arguments*) args;

  for (int i = 0; i < data->num_rows; ++i) {
    std::string key = data->matrix[i][data->column];

    data->matrix[i][data->column] =
      std::to_string(data->unique_values[key]);
  }

  pthread_exit(NULL);
}

test_validation_sets
split_dataset(std::string** matrix,
              const int num_rows,
              const int num_columns,
              const float split_percentage) {

  int num_batches = num_rows / BATCH_SIZE;
  int missing_batch = num_rows % BATCH_SIZE;

  // Ignore last batch if it contains less than 100 rows to ease calculations.
  if (missing_batch != 0) num_batches -= 1;

  int sets_size = std::floor(num_batches * split_percentage);
  int test_set_size = sets_size / 2;
  int valid_set_size = sets_size / 2;
  indexes test_batches[test_set_size];
  indexes valid_batches[valid_set_size];

  // Give indexes to the baches jumping from 100 to 100.
  indexes batches[num_batches];
  batches[0].start = 1;
  batches[0].end = BATCH_SIZE;

  for (int i = 1; i < num_batches; ++i) {
    batches[i].start = batches[i-1].end;
    batches[i].end = batches[i-1].end + BATCH_SIZE;
  }

  int test_last_class = -1; // Represents a batch's class.
  int valid_last_class = -1;
  for (int i = 0; i < test_set_size; ++i) {
    assign_batch(matrix, batches, num_batches,
                 test_batches, i, test_last_class);
    assign_batch(matrix, batches, num_batches,
                 valid_batches, i, valid_last_class);
  }

  test_validation_sets sets;
  // Test set.
  sets.test_matrix = copy_matrix_batches(matrix, test_batches, test_set_size);
  sets.test_matrix_rows = test_set_size * BATCH_SIZE;
  sets.test_matrix_columns = num_columns;

  // Validation set.
  sets.valid_matrix =
    copy_matrix_batches(matrix, valid_batches, valid_set_size);
  sets.valid_matrix_rows = valid_set_size * BATCH_SIZE;
  sets.valid_matrix_columns = num_columns;

  return sets;
}

int
batch_class(std::string** matrix, const indexes batch) {

  int classes[NUM_CLASSES] = {0};

  int max = 0;
  for (int row = batch.start; row < batch.end; ++row) {

    int pos = std::stoi(matrix[row][ATTACK_TYPE]);
    ++classes[pos];

    if (classes[pos] > classes[max])
      max = pos;
  }

  return max;
}

void
assign_batch(std::string** matrix,
             indexes* matrix_batches, const int num_batches,
             indexes* tv_batches, const int index, int& last_class) {

  std::random_device r;
  std::mt19937 generator(r());
  std::uniform_int_distribution<int> uniform_dist(0, num_batches-1);
  while (true) {
    int pos = uniform_dist(generator);

    if (!matrix_batches[pos].in_use) {
      if (batch_class(matrix, matrix_batches[pos]) != last_class) {
        tv_batches[index] = matrix_batches[pos];
        matrix_batches[pos].in_use = true;

        if ((last_class + 1) == NUM_CLASSES) last_class = 0;
        else ++last_class;

        break;
      }
    }

  }
}

std::string**
copy_matrix_batches(std::string** matrix,
                    const indexes* copy_batches, const int num_batches) {

  int num_rows = num_batches * BATCH_SIZE;
  std::string** copy_matrix = new std::string*[num_rows];

  int row = 0;
  for (int batch = 0; batch < num_batches; ++batch) {
    for (int batch_row = copy_batches[batch].start;
        batch_row < copy_batches[batch].end; ++batch_row) {

      copy_matrix[row] = matrix[batch_row];
      ++row;

      matrix[batch_row] = NULL; // Ad-hoc solution, fix it!.
    }

  }

  return copy_matrix;
}
