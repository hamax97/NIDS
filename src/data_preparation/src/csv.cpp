#include "csv.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>

std::string**
read_csv(csv_properties& csv_properties,
	 std::map<std::string, int>* unique_values) {

  std::string** matrix;

  // Open csv file
  std::ifstream csv_file;
  csv_file.open(csv_properties.file, std::ios_base::in);

  if (!csv_file.is_open()) {
    std::cerr << "Could not open file: " << csv_properties.file << "\n";
    exit(EXIT_FAILURE);
  }

  // Read file
  std::string line;

  // Header
  if (csv_properties.header) {
    csv_properties.num_lines -= 1; // header line
    std::getline(csv_file, line); // read header
  }

  int uv_pt_index, uv_svc_index, uv_flag_index, uv_at_index;
  matrix = new std::string*[csv_properties.num_lines];
  uv_pt_index = uv_svc_index = uv_flag_index = uv_at_index = 0;
  int row = 0;
  while (std::getline(csv_file, line)) {

    matrix[row] = new std::string[csv_properties.num_columns];

    // Parse each line
    std::stringstream values(line);

    for (int i = 0; i < csv_properties.num_columns; ++i) {
      std::getline(values, matrix[row][i], ',');

      // Only four columns will be encoded:
      // - 'protocol_type' -> column 1
      // - 'service' -> column 2
      // - 'flag' -> column 3
      // - 'attack_type' -> column 41
      switch (i) {
      case 1:
	exists(unique_values[i], matrix[row][i], uv_pt_index);
	break;

      case 2:
	exists(unique_values[i], matrix[row][i], uv_svc_index);
	break;

      case 3:
	exists(unique_values[i], matrix[row][i], uv_flag_index);
	break;

      case 41:
	exists(unique_values[i], matrix[row][i], uv_at_index);
	break;

	//      default:
	//	;
      }
    }

    ++row;
  }

  std::cout << "here";

  csv_file.close();

  return matrix;
}

void
exists(std::map<std::string, int>& unique_values,
       const std::string key, int& index) {
  // 1 if the key exists
  if (!unique_values.count(key)) {
    // mark as existent and assign an index that works as an id.
    unique_values[key] = index;
    ++index;
  }
}

void
free_csv(const csv_properties& csv, std::string **matrix) {
  for (int i = 0; i < csv.num_lines; ++i)
    delete [] matrix[i];
  delete [] matrix;
}
