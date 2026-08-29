# Minishell

*This project has been created as part of the 42 curriculum by anasimi.*

## Description

Minishell is a simplified Unix shell written in C.
The goal of this project is to understand processes, file descriptors, environment variables, pipes, redirections, and command execution.

## Features

* Interactive prompt
* Command history
* Built-in commands:

  * echo
  * cd
  * pwd
  * export
  * unset
  * env
  * exit
* Environment variables
* Pipes (`|`)
* Redirections (`<`, `>`, `<<`, `>>`)
* Signal handling

## Instructions

### Compilation

```bash
make
```

### Run

```bash
./minishell
```

### Clean

```bash
make clean
make fclean
make re
```

## Resources

### References

* [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — the behaviour of quoting, expansion, redirections and exit codes was taken from here, and from `bash` itself used as a live reference.
* [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/readline.html) — the prompt, the history and the redisplay done from the signal handler.
* Linux man pages: `execve(2)`, `fork(2)`, `pipe(2)`, `dup2(2)`, `wait(2)`, `signal(7)`, `stat(2)`, `access(2)`.
* [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html) — the tokenisation rules and the order in which a command line is processed.
* The 42 Minishell subject.

### How AI was used

AI was used as a reviewer and as a testing aid, not as a source of
finished code. Concretely:

* **Comparing against bash.** Building test batteries that run the same
  input through `minishell` and through `bash` and diff the output, the
  exit code and the files written. This is what exposed most of the
  edge cases: word splitting, empty expansions, ambiguous redirects and
  the exit codes 126/127.
* **Reviewing the here-document handling.** Discussing why reading a
  here-document inside the forked child breaks `cmd << A | cmd << B`,
  which led to reading every here-document up front, in the parent.
* **Signal behaviour.** Reproducing `ctrl-C`, `ctrl-D` and `ctrl-\` in a
  pseudo-terminal and comparing the result with bash, which is how the
  handling during a running child and during a here-document was
  settled.
* **Norm and memory checks.** Running `norminette` and `valgrind` and
  discussing how to split functions that were over the line limit.

Every change was read, understood and tested before being kept.
