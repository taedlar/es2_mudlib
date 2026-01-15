# ES2 MUDlib Development Environment Setup

This document describes how to set up a local testing environment for ES2 MUDlib development on Windows.

## Overview

ES2 MUDlib requires the Neolith driver (a MudOS fork) to run. This guide covers:
- Building the Neolith driver on Windows
- Configuring the local environment
- Setting up administrator privileges
- Running the MUD for testing

## Prerequisites

### Required Software

1. **Visual Studio 2019 or later** with C++ development tools
   - Community Edition is sufficient
   - Ensure "Desktop development with C++" workload is installed

2. **CMake 3.20 or later**
   - Used for building the Neolith driver

3. **GnuWin32 Bison**
   - Required for LPC parser generation
   - Install to `C:\GnuWin32` or update path in CMakePresets.json

4. **Git**
   - For cloning the repository and managing the Neolith submodule

### Optional Software

- **Python 3.x** - For running automation scripts
- **MUD Client** (TinTin++, Mudlet, or similar) - For interactive testing
- **Vim** with LPC syntax highlighting (see [syntax_highlight.md](syntax_highlight.md))

## Initial Setup

### 1. Clone the Repository

```bash
git clone https://github.com/taedlar/es2_mudlib.git
cd es2_mudlib
```

### 2. Initialize Neolith Submodule

The Neolith driver is included as a git submodule:

```bash
git submodule update --init --recursive
```

This will clone the Neolith driver source into the `neolith/` directory.

## Building the Neolith Driver

### Configure the Build

From the repository root:

```powershell
cd neolith
cmake --preset vs16-x64
```

This creates a Visual Studio 2019 x64 build configuration in `neolith/out/build/vs16-x64/`.

### Build the Driver

```powershell
cmake --build --preset ci-vs16-x64
```

Build presets:
- `dev-vs16-x64` - Debug build (incremental)
- `pr-vs16-x64` - Release with debug info (incremental)
- `ci-vs16-x64` - Clean rebuild (recommended for first build)

### Verify the Build

The executable will be located at:
```
neolith\out\build\vs16-x64\src\RelWithDebInfo\neolith.exe
```

Expected output during build:
- Numerous C4244 warnings (type conversion) - these are expected from legacy code
- No errors

## Configuration

### Create Local Configuration File

**Important:** Do not modify the source-controlled `neolith.conf` file.

1. Copy the template:
```powershell
cd ..  # Return to repository root
cp neolith.conf neolith.local.conf
```

2. Edit `neolith.local.conf` with your local paths:
```properties
MudlibDir	H:/github/es2_mudlib/mudlib
LogDir		H:/github/es2_mudlib/mudlib/log
```

3. The local config is already in `.gitignore` and won't be committed.

### Create Required Directories

Create the log directory if it doesn't exist:
```powershell
mkdir mudlib\log -Force
```

## Setting Up Administrator Account

### 1. Create Wizlist File

Create `mudlib/adm/etc/wizlist` with your admin username:

```
# ES2 Wizard List
# Format: username (privilege_level)
admin (admin)
```

Privilege levels (from lowest to highest):
- `(player)` - Regular player
- `(immortal)` - Immortal player  
- `(apprentice)` - Apprentice wizard
- `(wizard)` - Regular wizard
- `(arch)` - Arch wizard
- `(admin)` - Administrator (full privileges)

### 2. How It Works

When you create a character with username `admin`, the security daemon (`adm/daemons/securityd.c`) reads the wizlist file and automatically grants that user `(admin)` status.

The wizlist is read during MUD startup, so you must restart the driver after creating or modifying the file.

## Running the MUD

### Console Mode

Console mode runs the MUD with stdin/stdout for a single local user:

```powershell
.\neolith\out\build\vs16-x64\src\RelWithDebInfo\neolith.exe -f neolith.local.conf -c
```

**Limitations on Windows:**
- Console mode has issues with piped I/O from PowerShell or Python subprocess
- Direct keyboard input in console mode works but is not recommended for testing
- Best used for quick smoke tests only

### Network Mode (Recommended)

Network mode allows multiple connections via telnet/TCP on port 4000:

```powershell
.\neolith\out\build\vs16-x64\src\RelWithDebInfo\neolith.exe -f neolith.local.conf
```

Or run in background:
```powershell
Start-Process -FilePath ".\neolith\out\build\vs16-x64\src\RelWithDebInfo\neolith.exe" -ArgumentList "-f","neolith.local.conf" -WindowStyle Hidden
```

Connect using a MUD client to `localhost:4000`.

### Stopping the MUD

```powershell
Get-Process neolith | Stop-Process -Force
```

## Creating Your First Admin Character

### Via Network Mode

1. Start the MUD in network mode:
```powershell
.\neolith\out\build\vs16-x64\src\RelWithDebInfo\neolith.exe -f neolith.local.conf
```

2. Connect with a MUD client to `localhost:4000`

3. At the login prompt, enter the username from wizlist (e.g., `admin`)

4. For a new user, the MUD will prompt for:
   - Password (enter twice to confirm)
   - Confirmation (y/n)
   - Character display name
   - Gender (m/f)
   - Age

5. After character creation, you will have `(admin)` status automatically

### Verify Admin Status

Once logged in, you can use wizard commands like:
- `promote <username> <status>` - Grant wizard privileges to other users
- Access to `/adm` directory for system administration
- Full read/write access to mudlib files

## Development Workflow

### Making Code Changes

1. Edit LPC files in `mudlib/`
   - Syntax highlight with vim (see [syntax_highlight.md](syntax_highlight.md))
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

## Troubleshooting

### Build Issues

**Problem:** CMake can't find bison
- **Solution:** Install GnuWin32 bison to `C:\GnuWin32` or update `BISON_ROOT` in CMakePresets.json

**Problem:** Visual Studio 2019 not found
- **Solution:** Install VS 2019 or modify CMakePresets.json to use a different generator

### Runtime Issues

**Problem:** "error opening log file"
- **Solution:** Create the log directory: `mkdir mudlib\log -Force`

**Problem:** "Failed to get console mode for stdin"
- **Solution:** This is expected when using piped I/O. Use network mode instead.

**Problem:** "fopen(): [log/log] No such file or directory"
- **Solution:** Non-critical - MUD will still run. Create missing directories if needed.

### Admin Privileges Not Working

**Problem:** Created user doesn't have admin status
- **Solution:** 
  1. Verify `mudlib/adm/etc/wizlist` contains your username
  2. Restart the MUD (wizlist is read at startup)
  3. Check username spelling matches exactly

## File Structure Reference

```
es2_mudlib/
├── neolith/                    # Neolith driver source (submodule)
│   ├── src/                    # Driver C/C++ source
│   ├── docs/                   # Driver documentation
│   └── out/build/vs16-x64/     # Build output
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

1. Read the LPC tutorials in `mudlib/doc/lpc/`
2. Review the driver efun documentation in `neolith/docs/efuns/`
3. Explore existing areas in `mudlib/d/` for examples
4. Start building your own content!

## Additional Resources

- **ES2 MUDlib GitHub**: https://github.com/taedlar/es2_mudlib
- **Neolith Driver GitHub**: https://github.com/taedlar/neolith
- **LPC Language**: See `neolith/docs/manual/lpc.md`
- **Mudlib Documentation**: See `mudlib/doc/`
- **Syntax Highlighting**: See [syntax_highlight.md](syntax_highlight.md)

## Version History

- **2026-01-15**: Initial version covering Windows build and setup process
