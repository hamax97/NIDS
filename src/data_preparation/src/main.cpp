#include <iostream>
#include <string>
#include <map>
#include <cstring>
#include <cstdlib>
#include <pthread.h>
#include "csv.h"

#define COLUMNS_TO_ENCODE 4

void exists(std::map<std::string, int>& unique_values, const std::string key, const int index);

int
main(int argc, char *argv[]) {

  if (argc < 6) {
    std::cerr << "Usage:\n$ " << argv[0] << " file.csv [-h] -l <integer> -c <integer>\n"
	      << "\t-h: Indicates that the .csv file contains column names\n"
      	      << "\t-l: Indicates the number of lines in the file. "
	      << "If '-h' option present, 1 is substracted to this number\n"
	      << "\t-c: Indicates the number of columns in the file.\n";
    return EXIT_FAILURE;
  }

  // CSV file properties
  csv_properties csv;
  csv.file = argv[1];

  if (strcmp(argv[2], "-h") == 0) csv.header = true;

  if (csv.header) {
    csv.num_lines = atoi(argv[4]);
    csv.num_columns = atoi(argv[6]);
  } else {
    csv.num_lines = atoi(argv[3]);
    csv.num_columns = atoi(argv[5]);
  }

  // Read file
  std::string **matrix;
  std::map<std::string, int> unique_values[COLUMNS_TO_ENCODE]; // unique strings values in each colum
  matrix = read_csv(csv, unique_values);

  // Encode
  // p_thread column_threads[COLUMNS_TO_ENCODE];
  // for (int thr = 0; thr < COLUMNS_TO_ENCODE; ++thr) {
  //   int status; // variable to control thread creation

  //   status = pthread_create(&column_threads[thr], NULL, encode, &);
  // }

  // Free allocated space
  free_csv(csv, matrix);

  return EXIT_SUCCESS;
}
