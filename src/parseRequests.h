#pragma once
#include "fs_param.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <assert.h>
#include <algorithm>
#include "fs_param.h"

// Ensure requests is valid
std::vector<std::string> parse_request(std::string request);
// Validate num spaces
bool valid_num_spaces(const std::string &request, const std::vector<std::string> &tokens);
// Check for valid path
bool is_valid_path(const std::string &path);
// Make sure block is a valid number
bool is_block_valid(const std::string &blockStr);
/*
 * Req type is for fs request type: read, write, create, delete
 */
bool type_valid(const std::string &typeStr);
bool valid_request(const std::string &request, std::vector<std::string> &tokens);
bool contains_whitespace(const std::string &s);
bool is_valid_request_type(const std::string &requestType);
bool is_valid_username(const std::string &username);