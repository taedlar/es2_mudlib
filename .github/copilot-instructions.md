# Agent Instructions for ES2 MUDlib Project

## Project Overview

This is **ES2 (Eastern Stories II)**, a LPMud mudlib project originally started in 1994. ES2 is the first open source LPMud in Taiwan and the Chinese culture. The project uses LPC (Lars Pensjö C) programming language, not C language, despite files having `.c` extensions.

## Driver Information

- **Driver**: Neolith (a MudOS fork, customized for ES2)
- Driver source and documentation: `neolith/` subdirectory
- Driver documentation available at: `neolith/docs/`
- Configuration file: `neolith.conf` (runtime configuration)

## Project Structure

### Key Directories

- **`mudlib/`** - Main mudlib source code (all LPC files)
- **`neolith/`** - Neolith driver source code (C/C++)
  - `src/` - Driver source code (actual C/C++ files)
  - `docs/` - Driver documentation, efuns, applies, internals
  - `examples/` - Example LPC code
  
- **`docs/`** - Project-level documentation

### Important Files

- `neolith.conf` - Driver runtime configuration
- `mudlib/include/globals.h` - Global definitions implicitly included in all LPC files
## Development Guidelines

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

## Resources

- Neolith driver docs: `neolith/docs/`
- LPC language manual: `neolith/docs/manual/`
- ES2 specific docs: `docs/`
- Example code: `neolith/examples/`
