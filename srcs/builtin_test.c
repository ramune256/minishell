#include "minishell.h"

int	c_echo(char **argv)
{
	(void)argv;
	return (0);
}

int	c_cd(char **argv)
{
	(void)argv;
	return (0);
}

int	c_pwd(char **argv)
{
	(void)argv;
	printf("%s\n", getenv("PWD"));
	return (0);
}

int	c_export(char **argv)
{
	(void)argv;
	return (0);
}

int	c_unset(char **argv)
{
	(void)argv;
	return (0);
}

int	c_env(char **argv)
{
	(void)argv;
	return (0);
}

int	c_exit(char **argv)
{
	(void)argv;
	return (0);
}
