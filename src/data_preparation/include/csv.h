#pragma once

#include <string>
#include <map>

/**
 * Represents a csv file.
 */
typedef struct {
  std::string file; // Filename.
  bool has_header = false; // Has header?, that is, column names.
  std::string header;
  int num_rows;
  int num_columns;
  float split;
  bool encode = false; // Indicates if the dataset have to be encoded.
} csv_properties;

/**
 * Writes the content of 'matrix' into a csv file.
 */
void write_csv(std::string** matrix,
               int num_rows, int num_columns,
               std::string output_file,
               std::string header = "");

/**
 * Reads a csv file and returns a matrix containing the csv file contents.
 */
std::string** read_csv(csv_properties& csv_properties,
	      std::map<std::string, int>* unique_values);

/**
 * Checks if a key is already in the hashmap 'unique values'.
 * If it is not already there, assign to it an index.
 */
void key_exists(std::map<std::string, int>& unique_values,
                const std::string key, int& index);

/**
 * Checks if a file exists.
 */
inline bool file_exists(const std::string& file);
