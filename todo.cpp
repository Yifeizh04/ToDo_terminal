#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <string>

/**
 * The UP_TO_LOW constant represents the difference between ASCII values of uppercase and lowercase letters.
 */
constexpr int UP_TO_LOW = 'a' - 'A';

/**
 * The COMMAND vector holds the available command names.
 */
const std::vector<std::string> COMMAND = {"ls", "man", "rm", "sort", "add", "do", "undo", "clear", "exit"};

/**
 * The OPTION_COMMAND vector holds the option descriptions for each command.
 */
const std::vector<std::string> OPTION_COMMAND = {
    "", "", "[id_task, name_task, -d, -r]", "[0, 1]", "[name task]", "[id, name_task]", "[id, name_task]", "", ""};

/**
 * The DESCRIPTION_COMMAND vector holds the descriptions of each command.
 */
const std::vector<std::string> DESCRIPTION_COMMAND = {
    "Print all tasks with [id] name_task (green = done, red = not done) and number of tasks completed and remaining.",
    "Print the tutorial.",
    "Remove a specific task [id / name_task] [-d remove done task] [-r remove all tasks].",
    "Sort the tasks ([0] (default) first undone tasks, [1] done tasks first).",
    "Add a new task with a specified name.",
    "Mark a task with [id / name_task] as done.",
    "Mark a task with [id / name_task] as undone.",
    "Clear the shell.",
    "Exit the program (or you can press CTRL+C)."};

/**
 * The NAME_FILE constant represents the name of the file where all data is saved.
 */
const std::string NAME_FILE = "task_file_db";

/**
 * The Task class represents a task with a name and a completion status.
 */
class Task {
private:
    std::string name_task = "";
    bool is_done = false;

public:
    /**
     * Default constructor for Task.
     */
    Task() {}

    /**
     * Constructor for Task with a specified name.
     * @param name_task The name of the task.
     */
    Task(std::string name_task) : name_task(name_task) {}

    /**
     * Constructor for Task with a specified name and completion status.
     * @param name_task The name of the task.
     * @param is_done   The completion status of the task.
     */
    Task(std::string name_task, bool is_done) : name_task(name_task), is_done(is_done) {}

    /**
     * Get the name of the task.
     * @return The name of the task.
     */
    std::string get_name() {
        return name_task;
    }

    /**
     * Check if the task is done.
     * @return True if the task is done, false otherwise.
     */
    bool isDone() {
        return is_done;
    }

    /**
     * Mark the task as done.
     * @return True if the task was successfully marked as done, false if it was already done.
     */
    bool done() {
        if (is_done) {
            return false;
        } else {
            is_done = true;
            return true;
        }
    }

    /**
     * Mark the task as undone.
     * @return True if the task was successfully marked as undone, false if it was already undone.
     */
    bool undo() {
        if (is_done) {
            is_done = false;
            return true;
        } else {
            return false;
        }
    }

    /**
     * Compare two tasks based on their completion status.
     * @param other The task to compare with.
     * @return True if this task is less than the other task, false otherwise.
     */
    bool operator<(const Task &other) const {
        return is_done < other.is_done;
    }

    /**
     * Convert the task to a string representation.
     * @return The string representation of the task.
     */
    std::string to_string() {
        std::string result = name_task;
        result += '\n';
        result += (is_done ? '1' : '0');
        return result;
    }
};

/**
 * The NUMBER_ATTRIBUTE_TASK constant represents the number of attributes in a Task object.
 */
constexpr int NUMBER_ATTRIBUTE_TASK = 2;

/**
 * The Color enum represents ANSI color codes for text.
 */
enum Color { black = 30, red = 31, green = 32, yellow = 33, blue = 34, magenta = 35, cyan = 36, white = 37 };

/**
 * The check_color array holds color codes for indicating whether a task is done or not.
 */
const Color check_color[] = {red, green}; // Colors used to indicate whether a task is done or not

/**
 * The Manage class is responsible for managing tasks and user interactions.
 */
class Manage {
private:
    std::vector<Task> tasks;
    std::string command = "";
    std::set<std::string> memo_name;
    bool is_end = false;

    // Method to fill the tasks
    void init() {
        clear_terminal();
        std::ifstream in(NAME_FILE);
        std::string line;

        int cnt = 0;
        std::string name_task = "";
        bool flag;
        while (getline(in, line)) {
            if (cnt == 0) {
                name_task = line;
            } else if (cnt == 1) {
                flag = (line == "1");
            }

            cnt = (cnt + 1) % NUMBER_ATTRIBUTE_TASK;
            if (cnt == 0) {
                int prv_size = memo_name.size();
                memo_name.insert(name_task);
                if (memo_name.size() != prv_size) {
                    tasks.emplace_back(name_task, flag);
                }
            }
        }
    }

    // Method to clear the terminal
    void clear_terminal() {
        std::cout << "\033[H\033[2J\033[3J";
    }

    // Print text with color (text can be bold or underlined)
    void print_colored(Color color, bool bold_text, bool under_line, std::string message) {
        std::string pref = "\033[";
        if (bold_text) {
            pref += "1;";
        }
        if (under_line) {
            pref += "4;";
        }

        pref += std::to_string(color);
        std::cout << pref << "m" << message << "\033[0m" << std::endl;
    }

    // Method to print an error message with red color and underline
    void print_error(std::string message) {
        Color color = red;
        print_colored(red, false, true, message);
    }

    // Method to print the tutorial
    void print_tutorial() {
        Color color = blue;
        std::cout << std::endl << "\033[1;32mTutorial\033[0m" << std::endl;
        for (int i = 0; i < COMMAND.size(); ++i) {
            std::cout << std::to_string(i + 1) << ") \033[1;" << std::to_string(color) << "m" << COMMAND[i] << " "
                      << OPTION_COMMAND[i] << "\033[0m" << "\033[4m" << '\t' << DESCRIPTION_COMMAND[i] << "\033[0m"
                      << std::endl
                      << std::endl;
        }
    }

    // Sort the tasks (based on the option)
    void sortTask(std::string option) {
        bool need_reverse = false;
        if (option != "") {
            int z = parse_int(option);
            if (z > -1 && z < 2) {
                need_reverse = z == 1;
            } else {
                print_error("Invalid sort option [0 - 1]");
                return;
            }
        }

        sort(tasks.begin(), tasks.end());

        if (need_reverse) {
            reverse(tasks.begin(), tasks.end());
        }
        update_file();
    }

    // Convert all letters of the command to lowercase
    void to_lowercase() {
        for (char &x : command) {
            if (x >= 'A' && x <= 'Z') {
                x += UP_TO_LOW;
            }
        }
    }

    // Split the string by space
    std::vector<std::string> split() {
        std::vector<std::string> arr;
        char prv = ' ';
        for (auto &x : command) {
            if (x != ' ') {
                if (prv == ' ') {
                    arr.push_back("");
                }
                arr.back() += x;
            }
            prv = x;
        }
        return arr;
    }

    // Parse an integer from a string
    int parse_int(std::string x) {
        try {
            int num = std::stoi(x);
            return num;
        } catch (std::invalid_argument const &ex) {
            return -1;
        } catch (std::out_of_range const &ex) {
            return -1;
        }
    }

    // Rewrite the file (inefficient way)
    void update_file() {
        std::ofstream ofs(NAME_FILE, std::ofstream::trunc);

        for (Task &x : tasks) {
            ofs << x.to_string() << std::endl;
        }

        ofs.close();
    }

    // Add a task
    void add_task(std::string new_task) {
        if (memo_name.count(new_task)) {
            print_error("Task already exists");
            return;
        }

        memo_name.insert(new_task);
        tasks.emplace_back(new_task);
        update_file();
    }

    // Print tasks
    void print_task() {
        int cnt_done = 0;

        for (int i = 0; i < tasks.size(); ++i) {
            Task task = tasks[i];
            bool is_done = task.isDone();
            std::cout << i << " " << "\033[1;" << std::to_string(check_color[is_done]) << "m" << task.get_name()
                      << "\033[0m" << std::endl;
            cnt_done += is_done;
        }
        if (cnt_done == tasks.size()) {
            std::cout << "\033[1;32mAll tasks done :)\033[0m" << std::endl;
        } else {
            std::cout << "Stats:\t" << "\033[1;" << std::to_string(check_color[0]) << "mundone:\t"
                      << std::to_string(tasks.size() - cnt_done) << "\033[0m";
            std::cout << "\033[1;" << std::to_string(check_color[1]) << "m done:" << cnt_done << "\033[0m" << std::endl;
        }
    }

    // Find the index of a task by name
    int find_idx_task(std::string name_task) {
        int i = 0;
        while (i < tasks.size() && name_task != tasks[i].get_name()) {
            ++i;
        }

        return (i == tasks.size() ? -1 : i);
    }

    // Remove a task
    void remove_task(std::string second_part) {
        int id_task = parse_int(second_part);
        if (memo_name.count(second_part)) {
            id_task = find_idx_task(second_part);
        } else if (second_part == "-d" || second_part == "-r") {
            std::vector<Task> supp;
            if (second_part == "-d") {
                for (Task t : tasks) {
                    if (!t.isDone()) {
                        supp.push_back(t);
                    } else {
                        memo_name.erase(t.get_name());
                    }
                }
            } else {
                memo_name.clear();
            }
            swap(tasks, supp);
            update_file();
            return;
        } else if (id_task < 0 || id_task >= tasks.size()) {
            print_error("Invalid id_task or name_task or option");
            return;
        }

        memo_name.erase(tasks[id_task].get_name());
        tasks.erase(tasks.begin() + id_task);
        update_file();
    }

    // Mark a task as done
    void do_task(std::string second_part) {
        int id_task = 0;
        if (memo_name.count(second_part)) {
            id_task = find_idx_task(second_part);
        } else if (id_task < 0 || id_task >= tasks.size()) {
            print_error("Invalid id_task or name_task");
            return;
        }

        bool res = tasks[id_task].done();
        if (!res) {
            std::cout << "Da fixare";
        } else {
            update_file();
        }
    }

    // Mark a task as undone
    void undo_task(std::string second_part) {
        int id_task = 0;
        if (memo_name.count(second_part)) {
            id_task = find_idx_task(second_part);
        } else if (id_task < 0 || id_task >= tasks.size()) {
            print_error("Invalid id_task or name_task");
            return;
        }

        bool res = tasks[id_task].undo();
        if (!res) {
            std::cout << "Da fixare";
        } else {
            update_file();
        }
    }

    // Process the user's command
    void process_command() {
        to_lowercase();
        std::vector<std::string> part = split();
        if (find(COMMAND.begin(), COMMAND.end(), part[0]) == COMMAND.end()) {
            print_error("Not a valid command");
            return;
        } else {
            bool has_second = part.size() > 1;
            std::string second_part = "";
            if (has_second) {
                for (int i = 1; i < part.size(); ++i) {
                    second_part += part[i] + " ";
                }
                second_part.pop_back();
            }

            if (part[0] == "clear") {
                clear_terminal();
            } else if (part[0] == "ls") {
                print_task();
            } else if (part[0] == "exit") {
                is_end = true;
            } else if (part[0] == "sort") {
                sortTask(has_second ? part[1] : "");
            } else if (part[0] == "man") {
                print_tutorial();
            } else if (part[0] == "add") {
                if (!has_second) {
                    print_error("The command " + part[0] + " requires an option");
                } else {
                    add_task(second_part);
                }
            } else if (part[0] == "rm") {
                if (!has_second) {
                    print_error("The command " + part[0] + " requires an option");
                } else {
                    remove_task(second_part);
                }
            } else if (part[0] == "do") {
                if (!has_second) {
                    print_error("The command " + part[0] + " requires an option");
                } else {
                    do_task(second_part);
                }
            } else if (part[0] == "undo") {
                if (!has_second) {
                    print_error("The command " + part[0] + " requires an option");
                } else {
                    undo_task(second_part);
                }
            }
        }
    }

public:
    /**
     * Default constructor for Manage.
     * Initializes the task list, displays a welcome message, and prints the tutorial.
     */
    Manage() {
        init();
        std::cout << "\033[1;mToDo\033[0m" << std::endl;
        print_tutorial();
        run();
    }

    /**
     * Run the Manage application, accepting and processing user commands.
     */
    void run() {
        Color text_color = yellow;
        while (!is_end) {
            std::cout << "Insert command:\t";
            std::cout << "\033[1;" << std::to_string(text_color) << "m";
            do {
                std::getline(std::cin, command);
            } while (command == "");
            std::cout << "\033[0m";
            process_command();
        }
        clear_terminal();
    }
};

/**
 * The main function that starts the Manage application.
 */
int main() {
    Manage manage;
    manage.run();
    return 0;
}
