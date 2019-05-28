#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include "csv.h"
#include <sys/stat.h>

void
write_csv(std::string** matrix,
          int num_rows, int num_columns,
          std::string output_file,
          std::string header) {

  std::ofstream csv_out_file;
  csv_out_file.open(output_file, std::ios::out | std::ios::trunc);

  if (!csv_out_file.is_open()) {
    std::cerr << "Could not open file: " << output_file << std::endl;
    exit(EXIT_FAILURE);
  }

  if (header != "") csv_out_file << header << "\n";

  for (int i = 0; i < num_rows; ++i) {

    if (matrix[i] != NULL) { // Ad-hoc solution, fix it!.
      for (int j = 0; j < num_columns; ++j) {
        csv_out_file << matrix[i][j];

        if ((j + 1) != num_columns)
          csv_out_file << ",";

      }
      csv_out_file << "\n";
    }
  }

  csv_out_file.close();
}

std::string**
read_csv(csv_properties& csv_properties,
	 std::map<std::string, int>* unique_values) {

  std::string** matrix;

  // Open csv file
  if(!file_exists(csv_properties.file)) {
    std::cerr << "File: " << csv_properties.file << "does not exist\n";
    exit(EXIT_FAILURE);
  }

  std::ifstream csv_file;
  csv_file.open(csv_properties.file, std::ios_base::in);

  if (!csv_file.is_open()) {
    std::cerr << "Could not open file: " << csv_properties.file << "\n";
    exit(EXIT_FAILURE);
  }

  // Read file
  std::string line;

  // Header
  if (csv_properties.has_header) {
    csv_properties.num_rows -= 1; // header line
    std::getline(csv_file, csv_properties.header); // read header
  }

  matrix = new std::string*[csv_properties.num_rows];
  int uv_pt_index, uv_svc_index, uv_flag_index, uv_at_index;
  if (csv_properties.encode) {
    uv_pt_index = uv_svc_index = uv_flag_index = uv_at_index = 0;
  }
  int row = 0;
  while (std::getline(csv_file, line)) {

    matrix[row] = new std::string[csv_properties.num_columns];

    // Parse each line
    std::stringstream values(line);

    for (int i = 0; i < csv_properties.num_columns; ++i) {
      std::getline(values, matrix[row][i], ',');

      if (csv_properties.encode) {
	// Only four columns will be encoded:
	// - 'protocol_type' -> column 1
	// - 'service' -> column 2
	// - 'flag' -> column 3
	// - 'attack_type' -> column 41
	switch (i) {
	case 1:
	  key_exists(unique_values[0], matrix[row][i], uv_pt_index);
	  break;
	  
	case 2:
	  key_exists(unique_values[1], matrix[row][i], uv_svc_index);
	  break;
	  
	case 3:
	  key_exists(unique_values[2], matrix[row][i], uv_flag_index);
	  break;
	  
	case 41:
	  key_exists(unique_values[3], matrix[row][i], uv_at_index);
	  break;
	}
      }
    }

    ++row;
  }

  csv_file.close();

  return matrix;
}

void
key_exists(std::map<std::string, int>& unique_values,
           const std::string key, int& index) {

  // 1 if the key exists
  if (unique_values.count(key) == 0) {
    // mark as existent and assign an index that works as an id.
    unique_values[key] = index;
    ++index;
  }
}

inline bool
file_exists(const std::string& file) {
  struct stat buffer;
  return (stat (file.c_str(), &buffer) == 0);
}
