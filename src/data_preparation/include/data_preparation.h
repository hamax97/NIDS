#include <string>
#include <map>

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
 * Thead intended function.
 * Encodes a column of strings into integers given its unique values.
 */
void*
encode(void* args);

/**
 * Encodes a column of strings into integers given its unique values.
 */
void
encode(std::string** matrix, const int num_rows, const int column,
       std::map<std::string, int>& unique_values);
