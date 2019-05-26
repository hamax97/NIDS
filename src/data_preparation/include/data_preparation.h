#include <string>
#include <map>

#define COLUMNS_TO_ENCODE 4
// Column numbers in matrix
#define PROTOCOL_TYPE 1
#define SERVICE 2
#define FLAG 3
#define ATTACK_TYPE 41

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
