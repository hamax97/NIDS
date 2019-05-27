#include <string>
#include <map>

#define COLUMNS_TO_ENCODE 4
// Column numbers in matrix
#define PROTOCOL_TYPE 1
#define SERVICE 2
#define FLAG 3
#define ATTACK_TYPE 41
#define BATCH_SIZE 100 // Number of rows that represent one connection.
#define NUM_CLASSES 23 // Number of different tcp connection types.

/**
 * Arguments that will be passed to the thread function
 */
typedef struct {
  std::string** matrix = NULL;
  int num_rows = 0;
  int column = 0;
  std::map<std::string, int> unique_values;
} arguments;

/**
 * Contains the starting and ending indexes of a batch in a dataset.
*/
typedef struct {
  int start;
  int end;
  bool in_use = false;
} indexes;

typedef struct {
  std::string** test_matrix;
  int test_matrix_rows;
  int test_matrix_columns;

  std::string** valid_matrix;
  int valid_matrix_rows;
  int valid_matrix_columns;
} test_validation_sets;

/**
 * Thead intended function.
 * Encodes a column of strings into integers given its unique values.
 */
void*
encode(void* args);

/**
 * Groups in batches of 100 the dataset contained in 'matrix'. Those batches are
 * then shuffled and assigned to a test and validation sets. These sets have
 * the classes balanced. The number of batches assigned to these tests is
 * calculated by 'split_percentage', 50% percent of these split is for the
 * test set, and 50% for the validation set.
*/
test_validation_sets split_dataset(std::string** matrix,
                                   const int num_rows,
                                   const int num_columns,
                                   const float split_percentage);

/**
 * Given a batch (start and end indexes) returns the id of class that appears
 * the most.
*/
int batch_class(std::string** matrix, const indexes batch);

/**
 * Assigns a batch from the original dataset 'matrix' to a train or validation
 * set.
 */
void assign_batch(std::string** matrix,
                  indexes* matrix_batches, const int num_batches,
                  indexes* tv_batches, const int index, int& last_class);

/**
 * Copies the specified batches of 'matrix' into a new contiguos matrix. This
 * to ease writing to a .csv file.
 */
std::string** copy_matrix_batches(std::string** matrix,
                                  const indexes* copy_batches,
                                  const int num_batches);
