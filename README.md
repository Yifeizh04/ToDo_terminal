# ToDo

**ToDo_terminal** is a command-line task management program written in C++. While it doesn't have a graphical user interface (GUI), it offers a simple and efficient way to manage tasks directly from your terminal. The commands used in this program are reminiscent of Linux commands, making it easy to get started and use for anyone familiar with the terminal. It saves every task in a file called task_file_db.
## Getting Started

To compile and run **ToDo**, you have two options:

1. **Using g++:**

   Compile the C++ code using the `g++` compiler. Navigate to the project directory and run the following commands:

   ```bash
   g++ -o run todo.cpp
   ./todo
   ```
2. ***Run run_todo.sh***
    It automates the compilation and execution process. Before running the script, make sure it has execute permissions:
    ```bash
    chmod +x run_todo.sh
    ```
    Then, execute the script:
    ```bash
    ./run_todo.sh
    ```
## Usage Examples

Here are some examples of commands you can use with **ToDo** (you can find all the command in the code):

#### Adding a Task

```bash
$ add study math 
```

#### Remove a Task

```bash
$ rm study math
```

#### Marking a Task as Done

```bash
$ do study math
```

#### Listing Tasks

```bash
$ ls
```


#### Clear terminal

```bash
$ clear
```
