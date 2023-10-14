#include "logger.h"


// Logger class implementation
Logger::Logger(const std::string& filename_, const std::string& path_folder_, size_t max_entries_, size_t max_files_) {
    if (is_valid_filename(filename_) && is_valid_path_folder(path_folder_) && max_entries_ > 0) {
        filename = filename_;
        path_folder = path_folder_;
        max_entries = max_entries_;
        max_entries_counter = max_entries_;
        max_files = max_files_;

        try {
            create_folder();
        } catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }

        add_current_files();
        delete_all_files();
        open_file();
    } else {
        throw std::runtime_error("Invalid filename or path folder or max_entries. Filename and path folder must be "
                                 "a valid file name R\"([a-zA-Z0-9_]+\\.(txt|log))\") and max_entries must be greater "
                                 "than 0.");
    }
}

Logger::~Logger() {
    if (file_stream.is_open()) {
        file_stream.close();
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    if (file || console) {
        std::string timestamp = get_formatted_timestamp();
        std::string level_string = get_log_level_string(level);
        std::string formatted_message = log_format;

        formatted_message = replace_placeholder(formatted_message, "%timestamp%", timestamp);
        formatted_message = replace_placeholder(formatted_message, "%level%", level_string);
        formatted_message = replace_placeholder(formatted_message, "%message%", message);

        if (file && level >= log_level_file) write_logs_file(formatted_message);
        if (console && level >= log_level_console) write_logs_to_console(formatted_message);

    } else {
        std::cout << "No one log input use" << std::endl;
    }
}

void Logger::debug(const std::string& debug_message) {
    log(LogLevel::DEBUG, debug_message);
}

void Logger::info(const std::string& info_message) {
    log(LogLevel::INFO, info_message);
}

void Logger::warning(const std::string& warning_message) {
    log(LogLevel::WARNING, warning_message);
}

void Logger::error(const std::string& error_message) {
    log(LogLevel::ERROR, error_message);
}

void Logger::set_log_level(Logger::LogLevel level) {
    log_level_console = level;
    log_level_file = level;
}

void Logger::set_log_level_console(Logger::LogLevel level) {
    log_level_console = level;
}

void Logger::set_log_level_file(Logger::LogLevel level) {
    log_level_file = level;
}

void Logger::set_log_format(const std::string &format) {

    const std::string required_elements[] = {
            "%timestamp%",
            "%level%",
            "%message%"
    };

    bool is_valid_format = false;
    for (const std::string &element : required_elements) {
        if (format.find(element) != std::string::npos) {
            is_valid_format = true;
            break;
        }
    }

    if (is_valid_format) {
        log_format = format;
    } else {
        throw std::runtime_error("Invalid log format. The format must contain at least one of the following elements: "
                                 "%timestamp%, %level%, %message%");
    }
}

void Logger::set_use_console_log(bool console_) {
    console = console_;
}

void Logger::set_use_file_log(bool file_) {
    file = file_;
}

void Logger::set_clear_all(bool clear_all_) {
    clear_all = clear_all_;
}

void Logger::set_filename(const std::string& filename_, const std::string& path_folder_, size_t max_entries_) {

    if (is_valid_filename(filename_) && is_valid_path_folder(path_folder_) && max_entries_ > 0) {
        while (!log_queue_files.empty()) log_queue_files.pop();
        if (file_stream.is_open()) {
            file_stream.close();
        }

        log_file_number = 0;
        filename = filename_;
        max_entries = max_entries_;
        max_entries_counter = max_entries_;
        path_folder = path_folder_;

        try {
            create_folder();
        } catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }

        add_current_files();
        delete_all_files();
        open_file();
    } else {
        throw std::runtime_error("Invalid filename or path folder or max_entries. Filename and path folder must be "
                                 "a valid file name R\"([a-zA-Z0-9_]+\\.(txt|log))\") and max_entries must be greater "
                                 "than 0.");
    }
}

void Logger::set_max_entries(size_t max_entries_) {

    if (max_entries - max_entries_counter < max_entries_) {
        if (log_file_number + 1 == max_files) {
            log_file_number = 0;
        } else ++log_file_number;
    }

    max_entries = max_entries_;
    max_entries_counter = max_entries;
}

void Logger::set_max_files(size_t max_files_) {
    max_files = max_files_;
}

void Logger::open_file() {
    while(true) {
        if (!log_queue_files.empty()) {
            while (max_files < log_queue_files.size() + 1 && max_files != 0) delete_first_file();
        }

        if (max_entries > get_count_of_lines() || max_entries == 0) {
            break;
        }

        log_queue_files.push('#' + std::to_string(log_file_number) + filename);

        if (log_file_number + 1 == max_files) {
            log_file_number = 0;
        } else ++log_file_number;
    }

    if (path_folder.empty()) {
        file_stream.open('#' + std::to_string(log_file_number) + filename, std::ios::app);
    } else {
        file_stream.open(path_folder + "\\\\" + '#' + std::to_string(log_file_number) + filename, std::ios::app);
    }
    log_queue_files.push('#' + std::to_string(log_file_number) + filename);
}

void Logger::delete_first_file() {
    if (path_folder.empty()) {
        std::filesystem::path file_path(log_queue_files.front());

        try {
            std::filesystem::remove(file_path);
            log_queue_files.pop();
        } catch (const std::filesystem::filesystem_error& e) {
            std::cout << "Error deleting file: " << e.what() << std::endl;
        }
    } else {
        std::filesystem::path file_path(path_folder + "\\\\" + log_queue_files.front());

        try {
            std::filesystem::remove(file_path);
            log_queue_files.pop();
        } catch (const std::filesystem::filesystem_error& e) {
            std::cout << "Error deleting file: " << e.what() << std::endl;
        }
    }
}

void Logger::delete_all_files() {
    if (clear_all) {
        while(!log_queue_files.empty()) {
            delete_first_file();
        }
    }
}

void Logger::write_logs_to_file(const std::string& formatted_message) {

    if (!file_stream.is_open()) {
        open_file();
    }

    file_stream << formatted_message << std::endl;
}

void Logger::write_logs_file(const std::string& formatted_message) {
    if (max_entries_counter > 0 || max_entries == 0) {
        max_entries_counter -= 1;
        write_logs_to_file(formatted_message);
    } else {
        max_entries_counter = max_entries;
        if (file_stream.is_open()) {
            file_stream.close();
        }

        if (log_file_number + 1 == max_files) {
            log_file_number = 0;
        } else ++log_file_number;

        open_file();
        write_logs_to_file(formatted_message);
    }
}

void Logger::write_logs_to_console(const std::string& formatted_message) {
    std::cout << formatted_message << std::endl;
}

void Logger::add_current_files() {
    if (path_folder.empty()) {
        for (const auto &entry:
                std::filesystem::directory_iterator(std::filesystem::current_path())) {
            std::string temp = entry.path().filename().string();
            if (entry.is_regular_file() && temp.find(filename) != std::string::npos) {
                log_queue_files.push(temp);
            }
        }
    } else {
        for (const auto &entry:
                std::filesystem::directory_iterator(std::filesystem::path(path_folder))) {
            std::string temp = entry.path().filename().string();
            if (entry.is_regular_file() && temp.find(filename) != std::string::npos) {
                log_queue_files.push(temp);
            }
        }
    }
}

void Logger::create_folder() {
    if (!path_folder.empty()) {
        if (!std::filesystem::exists(path_folder)) {
            if (!std::filesystem::create_directory(path_folder)) {
                throw std::runtime_error("failed to create folder");
            }
        }
    }
}

size_t Logger::get_count_of_lines() {

    std::ifstream ifile('#' + std::to_string(log_file_number) + filename);

    if (ifile.is_open()) {
        size_t line_count = std::count(std::istreambuf_iterator<char>(ifile),
                std::istreambuf_iterator<char>(), '\n');
        ifile.close();

        return line_count;
    } else {
        return 0;
    }
}

bool Logger::is_valid_filename(const std::string& filename) {
    std::regex pattern(R"([a-zA-Z0-9_]+\.(txt|log))");
    return std::regex_match(filename, pattern);
}

bool Logger::is_valid_path_folder(const std::string& path_folder) {
    if (path_folder.empty()) return true;
    std::regex pattern(R"(.+[a-zA-Z0-9_])");
    return std::regex_match(path_folder, pattern);
}

std::string Logger::get_formatted_timestamp() {
    std::time_t now = std::time(nullptr);
    char timestamp[20];
    std::strftime(timestamp, sizeof(timestamp), "%d-%m-%Y %H:%M:%S", std::localtime(&now));
    return {timestamp};
}

std::string Logger::get_log_level_string(Logger::LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        default:
            return "";
    }
}

std::string Logger::replace_placeholder(const std::string& format, const std::string& placeholder,
                                        const std::string& value) {

    std::string result = format;
    size_t pos = result.find(placeholder);

    while (pos != std::string::npos) {
        result.replace(pos, placeholder.length(), value);
        pos = result.find(placeholder, pos + value.length());
    }

    return result;
}
