# Agent Instructions for ES2 MUDlib Project

## Project Overview

This is **ES2 (Eastern Stories II)**, a LPMud mudlib project originally started in 1994. ES2 is the first open source LPMud in Taiwan and the Chinese culture. The project uses LPC (Lars Pensjö C) programming language, not C language, despite files having `.c` extensions.

## Critical File Extension Information

**IMPORTANT**: All `.c` files in the `mudlib/` directory are **LPC source files**, NOT C language files.
- LPC is a scripting language specific to LPMud game servers
- LPC syntax is similar to C but has different semantics, built-in functions (efuns), and object-oriented features
- Do NOT apply C language conventions, linters, or analysis tools to LPC files
- Do NOT suggest C-specific optimizations or patterns for LPC code

## Driver Information

- **Driver**: Neolith (a MudOS fork, customized for ES2)
- Driver source and documentation: `neolith/` subdirectory
- Driver documentation available at: `neolith/docs/`
- Configuration file: `neolith.conf` (runtime configuration)

## Project Structure

### Key Directories

- **`mudlib/`** - Main mudlib source code (all LPC files)
  - `adm/` - System administration objects with root privileges (critical, modify with care)
  - `cmds/` - Player and wizard commands
  - `daemon/` - System daemons (game mechanics, skills, races, etc.)
  - `d/` - Game areas, zones, and content (rooms, NPCs, items)
  - `feature/` - Reusable object features/modules (movement, combat, equipment, etc.)
  - `include/` - Header files (.h) with macros, constants, and definitions
  - `std/` - Standard inheritable objects (character, room, item, etc.)
  - `obj/` - Miscellaneous objects (login, user, utility objects)
  - `doc/` - Help files and documentation
  - `data/` - Data files (user data, persistent storage)
  - `bin/` - Binary compiled objects (.b files, auto-generated)

- **`neolith/`** - Neolith driver source code (C/C++)
  - `src/` - Driver source code (actual C/C++ files)
  - `docs/` - Driver documentation, efuns, applies, internals
  - `examples/` - Example LPC code
  
- **`docs/`** - Project-level documentation

### Important Files

- `neolith.conf` - Driver runtime configuration
- `mudlib/include/globals.h` - Global definitions implicitly included in all LPC files
- `mudlib/adm/obj/master.c` - Master object (system control)
- `mudlib/adm/obj/simul_efun.c` - Simulated efuns (custom built-in functions)

## LPC Language Conventions

### Syntax Features
- C-like syntax with object-oriented features
- Functions: `type function_name(args) { ... }`
- Variables can be static, public, private, nomask, etc.
- Built-in functions (efuns) provided by the driver
- Apply functions (applies) - special callbacks from the driver
- Inherits: `inherit "path/to/file";`
- Includes: `#include <header.h>` or `#include "path/to/file.h"`
- Pragmas: `#pragma optimize`, `#pragma save_binary`, etc.

### Naming Conventions
- Functions typically use snake_case: `query_weight()`, `set_max_encumbrance()`
- Many query/set functions for object properties
- Prefixes like `F_` for features, `COMMAND_D` for daemons (see include files)

### File Organization
- Feature files: Reusable modules providing specific capabilities (movement, combat, etc.)
- Standard files: Base inheritable objects for common types
- Daemon files: Singleton services managing game systems
- Command files: Player/wizard command implementations

## Development Guidelines

### When Working with LPC Code:

1. **Always check driver documentation** in `neolith/docs/` for:
   - Available efuns (built-in functions): `neolith/docs/efuns/`
   - Apply functions: `neolith/docs/applies/`
   - Driver internals: `neolith/docs/internals/`

2. **Reference include files** for constants, macros, and daemon paths:
   - `mudlib/include/globals.h` - Always included
   - Other headers in `mudlib/include/` for specific features

3. **Follow ES2 coding patterns**:
   - Look at existing similar files as examples
   - Use feature inheritance when adding capabilities
   - Follow the existing copyright notice format
   - Use UTF-8 encoding (not Big-5)

4. **Security considerations**:
   - Files in `/adm/` have root privileges - modify with extreme care
   - Check access control when adding new commands or features
   - Use `nomask` for security-critical functions

### Testing and Running

- Build Neolith driver first (see `neolith/docs/INSTALL.md`)
- Edit `neolith.conf` with correct local paths
- Run: `neolith -f neolith.conf`
- Run as console mode for local testing: `neolith -f neolith.conf -c`
- In-game editing with `ed()` efun is supported for historical reasons; a git-based workflow (source control) is recommended for modern development activities.
  - A running MUD server may fetch updated LPC code from source control and reload objects as needed when the git operations are supported in the LPMud driver.

### Version Control

- Project uses git for source control
- Use git commit IDs instead of version numbers
- Supports LPC syntax highlighting in vim, see `docs/syntax_highlight.md` for vim editor setup.

## Language and Culture

- Mixed Chinese and English content (UTF-8 encoding)
- MUD content primarily in Traditional Chinese
- Code comments may be in Chinese or English
- Documentation in both languages

## License

- MIT License (since 2022)
- Earlier versions (pre-2022) had restrictions but now also available under MIT
- Neolith driver has different licensing (no commercial use due to original LPMud restrictions)

## Common Tasks

### Coding Style
- Use 4 spaces for indentation
- Use descriptive names for functions and variables
- Comment complex logic thoroughly
- Follow existing code structure and patterns
- Add vim modelines for consistent formatting and syntax highlighting, e.g.:
  ```c
  // vim: set ts=4 sw=4 syntax=lpc
  ```

### Adding a New Command
1. Create file in appropriate `mudlib/cmds/` subdirectory
2. Implement `main()` function for command logic
3. Register with COMMAND_D daemon if needed

### Creating a Room
1. Create file in `mudlib/d/` under appropriate area
2. Inherit from `/std/room`
3. Define room properties in `create()` function

### Adding a Feature
1. Create file in `mudlib/feature/`
2. Define functions for the capability
3. Have target objects inherit the feature

### Modifying Game Mechanics
1. Locate relevant daemon in `mudlib/daemon/`
2. Understand system interactions before modifying
3. Test thoroughly as daemons affect entire game

## Resources

- Neolith driver docs: `neolith/docs/`
- LPC language manual: `neolith/docs/manual/`
- ES2 specific docs: `mudlib/doc/` and `docs/`
- Example code: `mudlib/d/` and `neolith/examples/`

## Troubleshooting

- Check `mudlib/log/debug.log` for driver errors
- LPC compile errors show file and line number
- Runtime errors logged to debug log
- Use `#pragma optimize` for performance-critical files
- Binary files (`.b`) auto-generated, can be deleted to force recompile
