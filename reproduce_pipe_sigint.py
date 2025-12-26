import subprocess
import time
import signal
import os
import sys

# Start minishell
p = subprocess.Popen(["./minishell"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, bufsize=0)

# Write a piped command that waits
p.stdin.write("cat | cat\n")
p.stdin.flush()
time.sleep(1)

# Send SIGINT to the process group (assuming proper group handling or just the pid if no new group)
# Since we didn't use setsid, it's in the same group as this script?
# No, we want to simulate Ctrl-C on the terminal.
# If we just kill(p.pid, SIGINT), only parent gets it?
# But parent ignores it.
# We need to assume children inherited the group or check how minishell handles groups.
# Minishell doesn't seem to set process groups in the code I saw.
# So they share the group.
# If I send SIGINT to p.pid, it goes to the process.
# But Ctrl-C sends to the process GROUP.
# So I should send to os.getpgid(p.pid)?
# But that might kill this script too if we are in the same group.

# Workaround:
# We can't easily simulate "Ctrl-C to foreground group" without a TTY.
# But we can assume that if we kill the children manually, we can test the status logic.
# Wait, the user's issue is likely about the `status` variable in execute_pipe not capturing the signal correctly.

# Let's try to just send SIGINT to the minishell PID.
# Code: signal(SIGINT, SIG_IGN) in parent.
# So parent ignores it.
# Children?
# set_signal_child sets SIGINT to SIG_DFL.
# So children should die.
# But wait, if I send SIGINT to parent PID, does it propagate to children?
# Only if sent to Process Group.
# So I need to act like a terminal.

print("Please run this manually in a terminal to verify.")
