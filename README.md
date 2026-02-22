*This project has been created as part of the 42 curriculum by shunwata, nmasuda.*

# minishell

## Description
A bash-compatible shell reimplemented in C from scratch

features:
- Execute commands
- Multiple Pipes and Redirections
- Heredoc
- Most of built-in commands(such as cd, pwd, and others)

It also supports non-interactive mode (input from fd other than stdin)

## Instructions
```
git clone (URL)
cd minishell && make
./minishell
```
type *exit* to exit
press Control-c to send SIGINT (Signal Interrupt)

## Resources
- https://www.slideshare.net/slideshow/ss-135407412/135407412
- https://www.coins.tsukuba.ac.jp/~syspro/2005/No3.html
- https://usatie.notion.site/minishell-29921d3ea13447ad897349acd5733d5e

The template was created by AI
And then the humans rewrote almost everything.


## Technical
### Tokenizer
Converts a string like "ls -l | wc" into a concatenated list of tokens like this:
```
[WORD:"ls"] -> [WORD:"-l"] -> [PIPE:"|"] -> [WORD:"wc"] -> [EOF:NULL]
```

### Parser
Recursively processes a linked list of tokens to convert them into an abstract syntax tree.

```
NODE_PIPE (pipe)
/ \
/ \
NODE_EXEC NODE_EXEC
("ls", "-l") ("wc")
```

- node_redir is constructed by wrapping an exec_node inside it.
- node_pipe has two pointers to node_exec. However, the right-hand node_exec may be a node_pipe, and the right-hand node_exec of that node_pipe may also be a node_pipe.

:::note
info
BTW this sentence looks like a logic puzzles
:::

- The left-hand side is never a node_pipe.
- The end is never a node_pipe.
- If the command does not have a pipe, the AST will consist of only one node_exec or one node_redir with a node_exec inside.
- If there is a syntax error, specify NULL for the AST to terminate. If the AST is NULL, execution of the executor is skipped.

### Executor
Reads the AST created by the parser from top to bottom and recursively executes pipes, commands, and redirects.


## Contributors
- shunwata(me)
- nmasuda
