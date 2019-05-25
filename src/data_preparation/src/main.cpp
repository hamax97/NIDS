#include <iostream>
#include <string>
#include <map>
#include <cstring>
#include <cstdlib>
#include <pthread.h>
#include "csv.h"
#include "data_preparation.h"

#define COLUMNS_TO_ENCODE 4
#define PROTOCOL_TYPE 1
#define SERVICE 2
#define FLAG 3
#define ATTACK_TYPE 41

int
main(int argc, char *argv[]) {

  if (argc < 6) {
    std::cerr << "Usage:\n$ " << argv[0] << " file.csv [-h] -l <integer> -c <integer>\n"
	      << "\t-h: Indicates that the .csv file contains column names\n"
      	      << "\t-l: Indicates the number of lines in the file. "
	      << "If '-h' option present, 1 is substracted to this number\n"
	      << "\t-c: Indicates the number of columns in the file.\n"
              << "Note that the arguments must be in the same order presented above.\n";
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
  // unique strings values in each colum
  std::map<std::string, int> unique_values[COLUMNS_TO_ENCODE];
  matrix = read_csv(csv, unique_values);

  // Encode
  pthread_t column_threads[COLUMNS_TO_ENCODE];
  arguments args[COLUMNS_TO_ENCODE];
  for (int thr = 0; thr < COLUMNS_TO_ENCODE; ++thr) {

    args[thr].matrix = matrix;
    args[thr].num_rows = csv.num_lines;
    args[thr].unique_values = unique_values[thr];

    switch (thr) {
    case 0:
      args[thr].column =
        PROTOCOL_TYPE;
      break;
    case 1:
      args[thr].column = SERVICE;
      break;
    case 2:
      args[thr].column = FLAG;
      break;
    case 3:
      args[thr].column = ATTACK_TYPE;
      break;
    }

    int status; // variable to control thread creation
    status = pthread_create(&column_threads[thr], NULL,
                            encode, (void*) &args[thr]);

    if (status != 0){
      std::cerr << "Failed creating thread for column: "
                << args[thr].column << " with status: " << status << std::endl;
      return EXIT_FAILURE;
    }

  }

  for (int thr = 0; thr < COLUMNS_TO_ENCODE; ++thr) {
    int status;
    status = pthread_join(column_threads[thr], NULL);
    if (status != 0) {
      std::cerr << "Failed joining thread with status: "
                << status << std::endl;
      return EXIT_FAILURE;
    }
  }

  // Free allocated space
  free_csv(csv, matrix);

  pthread_exit(NULL);
  //  return EXIT_SUCCESS;
}
