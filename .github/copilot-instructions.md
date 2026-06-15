# Agent Instructions for Github Copilot

## LPC Code Style
- Use K&R style braces for functions and control structures.
- Use 4 spaces for indentation.
- Use descriptive names for functions and variables.
- Comment complex logic thoroughly.
- Follow existing code structure and patterns.

# Neolith LPMud driver binary search order

When trying to run ES2 mudlib, search for the neolith binary in the following order:
1. If the environment variable `NEOLITH_BIN` is set, the binary is `$NEOLITH_BIN`.
2. If you built Neolith from source, the binary is located in `neolith/out/build/<presetName>/src/RelWithDebInfo/neolith` (or `neolith.exe` on Windows).
3. Try `which neolith` or `where neolith` to find the executable in your PATH.
4. If none of the above work, ask the user to specify the location of the binary or set environment variables accordingly.
