#include <iostream>
#include <map>
#include <vector>
#include <cassert>
#include <regex>

using std::cin, std::cout, std::string, std::unordered_map, std::vector, std::pair, std::stoi, std::regex, std::regex_match;

enum defined_commands
{
    quit,
    show_all,
    flush_all,
    insert,
    get,
    update,
    flush,
};

unordered_map<string, defined_commands> map_input_to_command =
    {
        {"quit", defined_commands::quit},
        {"show_all", defined_commands::show_all},
        {"flush_all", defined_commands::flush_all},
        {"insert", defined_commands::insert},
        {"get", defined_commands::get},
        {"update", defined_commands::update},
        {"flush", defined_commands::flush},
};

vector<string> parse_string_by_delimiter(string input, string delimiter)
{
    // important to use size_t instead of int as size_t is platfrom dependent so, it might save memory
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = input.find(delimiter, pos_start)) != string::npos)
    {
        token = input.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(input.substr(pos_start));
    return res;
}

unordered_map<int, int> data;

int main()
{
    bool running = true;
    while (running)
    {
        cout << "aquiem_db> ";
        string input;
        getline(cin, input);

        // exception handling for bad-commands
        try
        {
            // parsing and throwing exceptions
            vector<string> parsed_input = parse_string_by_delimiter(input, " ");
            string input_command = parsed_input[0];

            if (map_input_to_command.count(input_command) == 0)
            {
                string error_message = "invalid command";
                throw error_message;
            }

            regex numeric_regex("^[0-9]{1,10000}$");
            vector<string> input_parameters;
            for (int i = 1; i < parsed_input.size(); i++)
            {
                if (!regex_match(parsed_input[i], numeric_regex))
                {
                    string error_message = "invald parameter found, keys and values can only be integers below 10000";
                    throw error_message;
                }
                input_parameters.push_back(parsed_input[i]);
            }

            // executing the commands
            defined_commands command = map_input_to_command.at(input_command);

            int key, value, new_value;
            switch (command)
            {
            case defined_commands::quit:
                cout << "Bye!\n";
                running = false;
                break;

            case defined_commands::show_all:

                if (data.size() == 0)
                {
                    string error_message = "no entries present in key-value store";
                    throw error_message;
                }

                for (pair<int, int> key_value_pair : data)
                {
                    cout << key_value_pair.first << ' ' << key_value_pair.second << '\n';
                }
                break;

            case defined_commands::flush_all:
                data.clear();
                cout << "flushed all keys\n";
                break;

            case defined_commands::insert:
                if (input_parameters.size() != 2)
                {
                    string error_message = "invalid number of parameters in 'insert' command expected 2 got ";
                    error_message += input_parameters.size();
                    throw error_message;
                }

                key = stoi(input_parameters[0]);
                value = stoi(input_parameters[1]);

                if (data[key])
                {
                    string error_message = "given key is already present in the key-value store, to update the existing key use 'update' command";
                    throw error_message;
                }

                data[key] = value;
                cout << "inserted\n";
                break;

            case defined_commands::get:
                if (input_parameters.size() != 1)
                {
                    string error_message = "invalid number of parameters in 'get' command expected 1 got ";
                    error_message += input_parameters.size();
                    throw error_message;
                }

                key = stoi(input_parameters[0]);

                if (data.count(key) == 0)
                {
                    string error_message = "given key is not present in the key-value store";
                    throw error_message;
                }

                value = data.at(key);
                cout << value << '\n';
                break;

            case defined_commands::update:

                if (input_parameters.size() != 2)
                {
                    string error_message = "invalid number of parameters in 'update' command expected 2 got ";
                    error_message += input_parameters.size();
                    throw error_message;
                }

                key = stoi(input_parameters[0]);
                new_value = stoi(input_parameters[1]);

                if (data.count(key) == 0)
                {
                    string error_message = "given key is not present in the key-value store";
                    throw error_message;
                }

                data[key] = new_value;
                cout << "updated\n";
                break;

            case defined_commands::flush:
                if (input_parameters.size() != 1)
                {
                    string error_message = "invalid number of parameters in 'flush' command expected 1 got ";
                    error_message += input_parameters.size();
                    throw error_message;
                }

                key = stoi(input_parameters[0]);

                if (data.count(key) == 0)
                {
                    string error_message = "given key is not present in the key-value store";
                    throw error_message;
                }

                data.erase(key);
                cout << "flushed\n";
                break;
            }
        }
        catch (string error_message)
        {
            cout << error_message << "\n";

            if (error_message == "invalid command")
            {
                cout << "list of availble commands :\n";
                int command_number = 1;
                for (auto i : map_input_to_command)
                {
                    cout << command_number << ") " << i.first << "\n";
                    command_number++;
                }
            }
        }

        cout << '\n';
    }

    return 0;
}