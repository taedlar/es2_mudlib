# Agent Instructions for ES2 Mudlib LPC Code

## Project Overview
This document provides specific instructions for working with LPC (Lars Pensjö C) code within the `mudlib/` directory of the ES2 (Eastern Stories II) project.

**IMPORTANT**: All `.c` files in the `mudlib/` directory are **LPC source files**, NOT C language files.
- LPC is a scripting language specific to LPMud game servers.
- LPC syntax is similar to C but has different semantics, built-in functions (efuns), and object-oriented features.
- Do NOT apply C language conventions, linters, or analysis tools to LPC files.
- Do NOT suggest C-specific optimizations or patterns for LPC code.

## LPC Language Conventions

### Syntax Features
- C-like syntax with object-oriented features.
- Functions: `type function_name(args) { ... }`
- Variables can be static, public, private, nomask, etc.
- Built-in functions (efuns) provided by the driver (Neolith).
- Apply functions (applies) - special callbacks from the driver.
- Inherits: `inherit "path/to/file";`
- Includes: `#include <header.h>` or `#include "path/to/file.h"`
- Pragmas: `#pragma optimize`, `#pragma save_binary`, etc.

### Naming Conventions
- Functions typically use `snake_case`: `query_weight()`, `set_max_encumbrance()`.
- Many `query_`/`set_` functions for object properties.
- Prefixes like `F_` for features, `COMMAND_D` for daemons (see include files).

### File Organization
- **`mudlib/adm/`**: System administration objects with root privileges (modify with care).
- **`mudlib/cmds/`**: Player and wizard commands.
- **`mudlib/daemon/`**: System daemons (game mechanics, skills, races, etc.).
- **`mudlib/d/`**: Game areas, zones, and content (rooms, NPCs, items).
- **`mudlib/feature/`**: Reusable object features/modules (movement, combat, equipment, etc.).
- **`mudlib/include/`**: Header files (.h) with macros, constants, and definitions.
- **`mudlib/std/`**: Standard inheritable objects (character, room, item, etc.).
- **`mudlib/obj/`**: Miscellaneous objects (login, user, utility objects).

## Development Guidelines

### When Working with LPC Code:

1.  **Always check driver documentation** in `neolith/docs/` for:
    - Available efuns (built-in functions): `neolith/docs/efuns/`
    - Apply functions: `neolith/docs/applies/`
    - Driver internals: `neolith/docs/internals/`

2.  **Reference include files** for constants, macros, and daemon paths:
    - `mudlib/include/globals.h` - Always included.
    - Other headers in `mudlib/include/` for specific features.

3.  **Follow ES2 coding patterns**:
    - Look at existing similar files as examples.
    - Use feature inheritance when adding capabilities.
    - Follow the existing copyright notice format.
    - Use UTF-8 encoding (not Big-5).

4.  **Security considerations**:
    - Files in `/adm/` have root privileges - modify with extreme care.
    - Check access control when adding new commands or features.
    - Use `nomask` for security-critical functions.

## Coding Style
- Use 4 spaces for indentation.
- Use descriptive names for functions and variables.
- Comment complex logic thoroughly.
- Follow existing code structure and patterns.

## Common Tasks

### Adding a New Command
1.  Create file in appropriate `mudlib/cmds/` subdirectory.
2.  Implement `main()` function for command logic.
3.  Register with `COMMAND_D` daemon if needed.

### Creating a Room
1.  Create file in `mudlib/d/` under appropriate area.
2.  Inherit from `/std/room`.
3.  Define room properties in `create()` function.

### Adding a Feature
1.  Create file in `mudlib/feature/`.
2.  Define functions for the capability.
3.  Have target objects inherit the feature.

### Modifying Game Mechanics
1.  Locate relevant daemon in `mudlib/daemon/`.
2.  Understand system interactions before modifying.
3.  Test thoroughly as daemons affect entire game.

## Resources

- Neolith driver docs: `neolith/docs/`
- LPC language manual: `neolith/docs/manual/`
- ES2 specific docs: `mudlib/doc/` and `docs/`
- Example code: `mudlib/d/` and `neolith/examples/`

## Troubleshooting

- Check `mudlib/log/debug.log` for driver errors.
- LPC compile errors show file and line number.
- Runtime errors logged to debug log.
- Use `#pragma optimize` for performance-critical files.
- Binary files (`.b`) auto-generated, can be deleted to force recompile.
