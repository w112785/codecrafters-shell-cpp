[![progress-banner](https://backend.codecrafters.io/progress/shell/43dc6bc0-59eb-45ca-9011-e542cdffb1ef)](https://app.codecrafters.io/users/codecrafters-bot?r=2qF)

This is a starting point for C++ solutions to the
["Build Your Own Shell" Challenge](https://app.codecrafters.io/courses/shell/overview).

In this challenge, you'll build your own POSIX compliant shell that's capable of
interpreting shell commands, running external programs and builtin commands like
cd, pwd, echo and more. Along the way, you'll learn about shell command parsing,
REPLs, builtin commands, and more.

**Note**: If you're viewing this repo on GitHub, head over to
[codecrafters.io](https://codecrafters.io) to try the challenge.

# Why?
Why not? When I saw this was a free challenge for now, I jumped on it. It is fun and I am more or less creating a interpreter of sorts for windows.

# IMPORTANT
Well I am coding this on windows and running it on windows. CodeCrafters will build and run it on linux.
Leading me to need to work around some differences in the systems. Namely the difference in the way Windows and Linux handle the PATH variable. 

# POSIX Shell Development To-Do List
## Copied this todo list from ["Sploosh"](https://github.com/adamadair/sploosh)
## Some will be implemented as part of the CodeCrafters Challenge

## Core Features
- [ ] Implement command parsing and execution
- [ ] Support shell built-ins (`cd`, `exit`, `echo`, etc.)
- [ ] Handle background execution with `&`
- [ ] Implement pipelines using `|`
- [ ] Implement I/O redirection (`>`, `<`, `>>`, `2>`, `2>>`)

## Control Structures
- [ ] `if` / `else` / `elif` support
- [ ] `for` loops
- [ ] `while` / `until` loops
- [ ] `case` statements
- [ ] Logical operators (`&&`, `||`, `!`)

## Expansions
- [ ] Variable expansion (`$VAR`, `${VAR}`)
- [ ] Command substitution (`` `cmd` `` and `$(cmd)`)
- [ ] Arithmetic expansion (`$((...))`)
- [ ] Filename expansion/globbing (`*`, `?`, `[...]`)
- [ ] Quoting rules (`"`, `'`, `\`)

## Job Control & Signals
- [ ] Implement background processes (`&`)
- [ ] `fg`, `bg`, and `jobs` support
- [ ] Signal handling (e.g., `SIGINT`, `SIGCHLD`)

## Environment Management
- [ ] Variable assignment and export (`VAR=value`, `export VAR`)
- [ ] Accessing and inheriting environment variables

## Script Execution
- [ ] Read and execute commands from a script file
- [ ] Support `#!` (shebang) handling

## Optional Enhancements
- [ ] Command line history
- [ ] Tab completion
- [ ] Command line editing (e.g., readline-like behavior)
