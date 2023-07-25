# 0x16. C - Simple Shell

## Description
The Shell Project is a simple command-line shell implemented in the C programming language. A shell is a user interface that allows users to interact with the operating system by executing commands. The Shell Project aims to provide basic functionalities of a shell, such as executing commands, handling input/output, and managing processes.

## Functions Involved
The Shell Project consists of several C functions that facilitate different aspects of the shell's functionality. Below are the main functions involved in the project:

### 1. `main()`
The `main()` function is the entry point of the program. It sets up the shell environment, displays the prompt, and enters into the main loop where it waits for user input. It reads the user's input, parses the command, and then executes it. Additionally, it handles the termination of the shell when the user decides to exit.

### 2. `read_line()`
The `read_line()` function is responsible for reading the user's input from the command-line. It reads characters from `stdin` until it encounters a newline character, indicating the end of the user's input. The function dynamically allocates memory to store the input, and it returns a pointer to the input line.

### 3. `parse_line()`
The `parse_line()` function takes the user's input as a string and parses it into individual tokens or arguments. It breaks the input into separate words or arguments using spaces as delimiters. The function returns an array of pointers to these tokens, which will be used for command execution.

### 4. `execute_command()`
The `execute_command()` function is responsible for executing the parsed command obtained from `parse_line()`. It first checks if the command is a built-in shell command (e.g., cd, exit). If it's a built-in command, the shell handles it internally. If it's an external command, the function forks a new process and uses `execvp()` to execute the external command in the child process.

### 5. `builtin_cd()`
The `builtin_cd()` function is a built-in shell command for changing the current working directory of the shell. It takes care of changing the directory as per the user's input.

### 6. `builtin_exit()`
The `builtin_exit()` function is another built-in shell command that allows the user to exit the shell gracefully. It handles the termination of the main loop and the cleanup of any allocated resources before the shell exits.

### 7. `wait_for_child()`
The `wait_for_child()` function is used to wait for the completion of a child process. When the shell executes an external command, it creates a child process to execute the command. The parent process (shell) uses `waitpid()` or a similar function to wait for the child process to complete its execution.
