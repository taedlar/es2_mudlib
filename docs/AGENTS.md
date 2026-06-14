# ES2 mudlib Development Environment Setup

This document describes how to set up a local testing environment for ES2 MUDlib development.

## Overview

ES2 mudlib requires the Neolith LPMud driver to run. This guide covers:
- Building the Neolith LPMud driver (supports Linux, Windows, macOS)
- Configuring the local environment
- Setting up administrator privileges
- Running the MUD for testing

## Initial Setup

### Build Neolith executable

- The Neolith driver is included as a git submodule [neolith](neolith)
- If you cloned ES2 mudlib from the git repository, you may need run:
  ```bash
  git submodule update --init --recursive
  ```
- Follow the instructions in [INSTALL.md](neolith/docs/INSTALL.md) to build the `neolith` executable

### Configuration

Create the log directory (see `LogDir` in [neolith.conf](../neolith.conf)) if it doesn't exist:
```powershell
mkdir mudlib/log
```

## Setting Up Administrator Account

### 1. Create Wizlist File

Create `mudlib/adm/etc/wizlist` with your administrator username:

```
# ES2 Wizard List
# Format: username (privilege_level)
gandalf (admin)
```

Privilege levels (from lowest to highest):
- `(player)` - Regular player
- `(immortal)` - Immortal player  
- `(apprentice)` - Apprentice wizard
- `(wizard)` - Regular wizard
- `(arch)` - Arch wizard
- `(admin)` - Administrator (full privileges)

### 2. How It Works

When you create a character with username `gandalf`, the security daemon (`adm/daemons/securityd.c`) reads the wizlist file and automatically grants that user `(admin)` status.

The wizlist is read during MUD startup, so you must restart the driver after creating or modifying the file.

## Running the MUD

### Console Mode

Console mode runs the MUD with stdin/stdout for a single local user:

```powershell
# Example: neolith was built with MSVC on Windows
.\neolith\out\build\vs16-x64\src\RelWithDebInfo\neolith.exe -f neolith.local.conf -c
```

### Network Mode

Network mode allows multiple connections via telnet/TCP on port 4000:

```powershell
# Example: neolith was built with MSVC on Windows
.\neolith\out\build\vs16-x64\src\RelWithDebInfo\neolith.exe -f neolith.local.conf
```

Connect using `telnet` or a MUD client to `localhost:4000`.

## Development Workflow

### Making Code Changes

1. Edit LPC files in `mudlib/`
   - All `.c` files are LPC code, not C

2. Reload objects in-game:
   - Use `update` command for single objects
   - Restart MUD for daemon or system-wide changes

3. Check logs:
   - `mudlib/log/debug.log` - Driver debug messages
   - `mudlib/log/` - Various LPC-level logs

### Binary Files

LPC objects can be saved as binary (`.b` files) in `mudlib/bin/`:
- Auto-generated when `#pragma save_binary` is used
- Safe to delete - will be regenerated
- Should be in `.gitignore`

### Testing Changes

1. Make changes to LPC files
2. Reload affected objects in-game or restart MUD
3. Test functionality
4. Check debug log for errors

## File Structure Reference

```
es2_mudlib/
├── neolith/                    # Neolith driver source (submodule)
│   ├── src/                    # Driver C/C++ source
│   ├── docs/                   # Driver documentation
│   └── out/build/*             # Build output
│       └── src/RelWithDebInfo/
│           └── neolith.exe     # Driver executable
├── mudlib/                     # MUDlib LPC source
│   ├── adm/                    # Administration (root privileges)
│   │   ├── daemons/           # System daemons
│   │   │   └── securityd.c    # Security/wizlist daemon
│   │   └── etc/
│   │       └── wizlist        # Wizard privilege list
│   ├── cmds/                  # Player/wizard commands
│   ├── daemon/                # Game mechanic daemons
│   ├── d/                     # Game areas/content
│   ├── feature/               # Reusable object features
│   ├── include/               # Header files
│   ├── std/                   # Standard inheritable objects
│   ├── data/                  # Persistent data (gitignored)
│   ├── log/                   # Log files (gitignored)
│   └── bin/                   # Binary compiled objects (gitignored)
├── neolith.conf               # Template config (source controlled)
├── neolith.local.conf         # Local config (gitignored)
└── docs/                      # Documentation
    └── AGENTS.md              # This file
```

## Next Steps

After setting up your environment:

1. Read the LPC tutorials in `neolith/docs/manual`
2. Review the driver efun documentation in `neolith/docs/efuns/`
3. Explore existing areas in `mudlib/d/` for examples
4. Start building your own content!

## Additional Resources

- **Neolith LPMud driver**: https://github.com/taedlar/neolith
- **LPC Language**: See `neolith/docs/manual/lpc.md`
