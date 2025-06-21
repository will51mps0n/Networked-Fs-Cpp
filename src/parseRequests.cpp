#include "parseRequests.h"
#include "fs_param.h"
#include <sstream>
#include <cctype>

std::vector<std::string> parse_request(std::string request)
{
    std::vector<std::string> result;
    if (request.empty())
        return result;

    std::istringstream stream(request);
    std::string word;
    while (stream >> word)
        result.push_back(word);

    return result;
}

bool valid_num_spaces(const std::string &request, const std::vector<std::string> &tokens)
{
    if (request.empty())
        return false;

    std::string rebuilt;
    for (size_t i = 0; i < tokens.size(); ++i)
    {
        rebuilt += tokens[i];
        if (i < tokens.size() - 1)
            rebuilt += ' ';
    }

    return rebuilt == request;
}

bool is_valid_username(const std::string &username)
{
    if (username.empty() || username.length() > FS_MAXUSERNAME)
        return false;
    for (char c : username)
    {
        if (isspace(c))
            return false;
    }
    return true;
}

bool is_valid_path(const std::string &path)
{
    if (path.empty() || path[0] != '/' || (path.size() > 1 && path.back() == '/'))
        return false;

    if (path == "/")
        return false;

    std::istringstream stream(path);
    std::string segment;
    while (std::getline(stream, segment, '/'))
    {
        if (segment.empty())
            continue;
        if (segment.length() > FS_MAXFILENAME)
            return false;
        for (char c : segment)
        {
            if (isspace(c) || c == '/')
                return false;
        }
    }
    return true;
}

bool contains_whitespace(const std::string &s)
{
    return std::any_of(s.begin(), s.end(), [](char c)
                       { return std::isspace(static_cast<unsigned char>(c)); });
}

bool is_valid_request_type(const std::string &type)
{
    return type == "FS_CREATE" || type == "FS_DELETE" || type == "FS_READBLOCK" || type == "FS_WRITEBLOCK";
}

bool is_block_valid(const std::string &blockStr)
{
    if (blockStr.empty() || blockStr.find_first_not_of("0123456789") != std::string::npos)
        return false;
    if (blockStr.length() > 1 && blockStr[0] == '0')
        return false;
    if (!std::all_of(blockStr.begin(), blockStr.end(), ::isdigit))
        return false;

    try
    {
        uint32_t value = std::stoul(blockStr);
        return value < FS_MAXFILEBLOCKS;
    }
    catch (...)
    {
        return false;
    }
}

bool type_valid(const std::string &typeStr)
{
    return typeStr == "f" || typeStr == "d";
}

bool valid_request(const std::string &request, std::vector<std::string> &tokens)
{
    if (tokens.empty())
        return false;

    for (const auto &token : tokens)
    {
        if (contains_whitespace(token))
            return false;
    }

    const std::string &type = tokens[0];
    if (!is_valid_request_type(type))
        return false;

    int expected = (type == "FS_DELETE") ? 3 : 4;
    if (tokens.size() != static_cast<size_t>(expected))
        return false;
    if (!valid_num_spaces(request, tokens))
        return false;

    const std::string &username = tokens[1];
    const std::string &path = tokens[2];
    if (!is_valid_username(username) || !is_valid_path(path))
        return false;

    if (expected == 3)
        return true;

    const std::string &arg = tokens[3];
    char op = type[3];
    switch (op)
    {
    case 'C':
        return type_valid(arg);
    case 'R':
        return is_block_valid(arg);
    case 'W':
        return is_block_valid(arg);
    default:
        return false;
    }
}
