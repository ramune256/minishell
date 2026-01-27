# Coding Guidelines

* **Function Names:**
    * Must be either a "Verb" or a "Verb + Object".
    * **Good Examples:** `clone_ev()`, `cleanup()`, `execute()`, `execute_pipe()`, `set_signal_parent()`, `get_next_line()`, `isalnum()`
    * **Bad Examples:** `prefix_add_after()`, `execute_core()`

* **Avoid Vague Words:**
    * Do not use non-specific words like `check`, `process`, or `change` in function names.

* **Abstraction:**
    * The purpose of splitting functions must be "abstraction."
    * Passing around variables like `int *` repeatedly is simply "splitting code without abstraction" and holds no essential value.
    * Functions should be designed to be as reusable as possible.

**Example of Bad Code**
```c
bool	main_create_cur(size_t *ev_len, size_t fir, char *ev_var, char **cur)
{
	size_t	before;
	size_t	after;
	char	*prefix;
	char	*tmp;

	before = *ev_len;
	after = ft_strlen(ev_var);
	prefix = NULL;
	tmp = NULL;
	prefix = ft_substr(*cur, 0, fir);
	if (!prefix)
		return (free(*cur), free(ev_var), false);
	tmp = ft_strjoin(prefix, ev_var);
	(free(prefix), free(ev_var));
	if (!tmp)
		return (free(*cur), false);
	if (prefix_add_after(fir + before, fir + after, tmp, cur) == false)
		return (false);
	return (true);
}

char	*create_cur(size_t *j, char *cur, t_alloc *heap)
{
	size_t	fir;
	size_t	ev_len;
	char	*ev_var;

	fir = *j;
	ev_len = 0;
	ev_var = NULL;
	(*j)++;
	ev_var = chenge_ev(cur, heap, j, &ev_len);
	if (ev_var)
	{
		if (main_create_cur(&ev_len, fir, ev_var, &cur) == false)
			return (NULL);
	}
	else
		*j = fir + 1;
	return (cur);
}
```

**Example of Good Code**
```c
	while (1)
	{
		get_input(&heap.line, "minishell> ");
		if (heap.line)
		{
			if (isatty(STDIN_FILENO))
				add_history(heap.line);
			tokenize(&heap);
			parse(&heap);
			expand(heap.ast, &heap);
			print_ast(heap.ast, 0);
			execute(heap.ast, &heap);
		}
	}

```
*The logic is properly abstracted.*

* **Const Correctness:**
    * Pointer arguments that are not modified within the function should be marked as `const` (or `static` depending on context intent), and those that are modified should not.
    * This clarifies the function's intent.
    * However, it is not necessary if it impairs readability.

* **Abbreviations:**
    * Do not abbreviate if the original word cannot be understood at a glance.
    * Examples: `ret`, `par`, `cur`, `cnt`.

* **Error Handling (Malloc):**
    * Failures in functions like `malloc` are critical errors; it is preferable to terminate immediately rather than propagating the error up to the caller.
    * You need a mechanism—such as a `cleanup()` function and a struct grouping dynamically allocated pointers—that allows you to free all memory and `exit()` immediately from anywhere in the code.
    * However, highly reusable functions like `ft_strdup()` do not need to terminate immediately.

---

# NORMINETTE C Coding Standard

## Formatting
1.  **Indentation:** Use "Horizontal Tabs" only (spaces are forbidden).
2.  **Line Length:** Max 80 characters per line. overly long lines should be broken with a backslash `\` and properly indented.
3.  **Spacing:** Separate functions with exactly one empty line.
4.  **Empty Lines:** Do not include spaces or tabs on empty lines.
5.  **Function Definition:** Use exactly one "Horizontal Tab" between the return type and the function name.

## Variables
6.  **Declaration Position:** Variables must be declared at the very beginning of the function.
7.  **Limit:** Maximum of 5 variables per function.
8.  **Alignment:** Variable names should be aligned within the scope using indentation.
9.  **One per Line:** Only one variable declaration per line.
10. **Initialization:** Do not initialize variables during declaration (e.g., `int a = 0;` is forbidden).

## Control Structures & Prohibitions
11. **Loops:** `for` loops are forbidden (use `while`).
12. **Ternary Operator:** `? :` is forbidden.
13. **Conditions:** Do not perform assignments inside `while` or `if` conditions.
14. **Braces:** Do not write `while` and `{` on the same line.

## Return
15. **Parentheses:** Return values must be enclosed in `()`. Put one space between `return` and `()` (e.g., `return (0);`).
16. **Void Functions:** Write `return ;` for void functions.

## Other Guidelines
17. **If Statements:** To prevent wasting lines on braces, the body of an `if` statement should ideally be written on a single line.
18. **One-Line Execution:** Executing multiple functions in one line, such as `(cleanup(&something), exit(1));`, is permissible.
19. **Comma Operator:** If you can reduce line count by putting a function inside the `return ()` parentheses (using the comma operator), you should do so.
20. **Function Length:** All functions should possess high readability through abstraction, and the body of each function should be within 25 lines.


# How to verify the code
* Build your code using make, make clean, make fclean and make re.
* To check for memory leaks, use valgrind.
