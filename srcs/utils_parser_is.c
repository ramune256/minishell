#include "minishell.h"

bool	is_empty_cmd(t_cmd *cmd)
{
	if (cmd->type == NODE_REDIR)
		return (false);
	if (cmd->type == NODE_EXEC && cmd->argv == NULL)
		return (true);
	return (false);
}

bool	is_end_cmd(t_token *tokens)
{
	if (!tokens)
		return (true);
	if (tokens->type == TOKEN_PIPE)
		return (true);
	if (tokens->type == TOKEN_EOF)
		return (true);
	return (false);
}
