#pragma once

#include <string>
#include <map>

/**
 * Represents a csv file.
 */
typedef struct {
  std::string file; // Filename.
  bool header = false; // Has header?, that is, column names.
  int num_lines;
  int num_columns;
} csv_properties;

/**
 * Reads a csv file and returns a matrix containing the csv file contents.
 */
std::string** read_csv(csv_properties& csv_properties,
	      std::map<std::string, int>* unique_values);

/**
 * Checks if a key is already in the hashmap 'unique values'.
 * If it is not already there, assign to it an index.
 */
void exists(std::map<std::string, int>& unique_values,
	    const std::string key, int& index);

/**
 * Frees the matrix that represents the csv
 */
void free_csv(const csv_properties& csv, std::string **columns);
