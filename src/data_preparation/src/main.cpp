#include <iostream>
#include <string>
#include <map>
#include <cstring>
#include <cstdlib>
#include <pthread.h>
#include "csv.h"
#include "data_preparation.h"

/**
 * Reads the command line passed arguments.
*/
void read_command_line_options(int argc, char** argv);

/**
 * Creates threads and calls a encode function in each one.
 */
void encode_dataset();

// Unique strings values in each column that will be encoded.
std::map<std::string, int> unique_values[COLUMNS_TO_ENCODE];
std::string **matrix; // CSV matrix representation.
csv_properties csv; // CSV file properties.
std::string output_csv;

int
main(int argc, char *argv[]) {

  if (argc < 9) {
    std::cerr << "Usage:\n$ " << argv[0]
              << " -f /path/to/file.csv [-h] -l <integer> -c <integer> "
              << " -o /path/to/output.csv\n"
	      << "\t-h: Indicates that the .csv file contains a header\n"
      	      << "\t-l: Indicates the number of lines in the file. "
	      << "If '-h' option is present, 1 is substracted to this number\n"
	      << "\t-c: Indicates the number of columns in the file.\n"
              << "\t-o: Indicates the output location for the modified csv\n";
    return EXIT_FAILURE;
  }

  // Take command line arguments.
  read_command_line_options(argc, argv);

  // Read file.
  matrix = read_csv(csv, unique_values);

  // Encode.
  encode_dataset();

  // Write to CSV.
  if (csv.has_header)
    write_csv(matrix, csv.num_rows, csv.num_columns, output_csv, csv.header);
  else
    write_csv(matrix, csv.num_rows, csv.num_columns, output_csv);

  // Free allocated space.
  free_csv(csv, matrix);

  pthread_exit(NULL);
  //  return EXIT_SUCCESS;
}

void
read_command_line_options(int argc, char** argv) {

  int index = 1;
  while (index < argc) {

    if (strcmp(argv[index], "-f") == 0) {
      csv.file = argv[index+1];
      index += 2;
    } else if (strcmp(argv[index], "-h") == 0) {
      csv.has_header = true;
      ++index;
    } else if (strcmp(argv[index], "-l") == 0) {
      csv.num_rows = atoi(argv[index+1]);
      index += 2;
    } else if (strcmp(argv[index], "-c") == 0) {
      csv.num_columns = atoi(argv[index+1]);
      index += 2;
    } else if (strcmp(argv[index], "-o") == 0) {
      output_csv = argv[index+1];
      index += 2;
    }
  }
}

void
encode_dataset() {
  pthread_t column_threads[COLUMNS_TO_ENCODE];
  arguments args[COLUMNS_TO_ENCODE];
  for (int thr = 0; thr < COLUMNS_TO_ENCODE; ++thr) {

    args[thr].matrix = matrix;
    args[thr].num_rows = csv.num_rows;
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
      exit(EXIT_FAILURE);
    }

  }

  for (int thr = 0; thr < COLUMNS_TO_ENCODE; ++thr) {
    int status;
    status = pthread_join(column_threads[thr], NULL);
    if (status != 0) {
      std::cerr << "Failed joining thread with status: "
                << status << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}
