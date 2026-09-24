# Agent Instructions for ES2 Mudlib Development Workflows

This file covers environment setup, configuration, and troubleshooting for the ES2 + Neolith development environment.

> For LPC coding conventions, file structure, and development guidelines, see [`mudlib/AGENTS.md`](mudlib/AGENTS.md) as the single source of truth.

## Setup

### 1. Clone

Always clone with submodules to include both mudlib and the Neolith driver source:

```bash
git clone --recurse-submodules https://github.com/taedlar/es2_mudlib.git
```
Or, if already cloned:
```bash
git submodule update --init --recursive
```

### 2. Build Neolith

Follow [`neolith/docs/INSTALL.md`](neolith/docs/INSTALL.md) to build the `neolith` executable.

Neolith uses CMake presets. Build output is always placed under `neolith/out/build/<presetName>/`.
Use the preset matching your platform (use `pr-<presetName>` for `--build` if not otherwise specified):

For example, on Linux:
```bash
cd neolith
cmake --preset linux-gcc
cmake --build --preset pr-linux-gcc
```

The resulting executable for `linux-gcc` configuration is `neolith/out/build/linux-gcc/src/RelWithDebInfo/neolith`. For a debug build, substitute `pr-<preset>` (e.g. `pr-linux`) with `dev-<preset>` to get a `Debug/neolith` binary.

### 3. Configuration

Neolith resolves mudlib paths relative to the config file, so the source-controlled [`neolith.conf`](neolith.conf) works for development as-is.
Creating a local copy is optional and only needed if you want to override settings without touching the tracked file:

```bash
cp neolith.conf neolith.local.conf
# Edit neolith.local.conf as needed, then pass it with -f
```

Create the log directory if it does not exist (see `LogDir` in the config):

```bash
mkdir -p mudlib/log
```

### 4. Administrator Account

Create `mudlib/adm/etc/wizlist` with your username to grant admin access:

```
# ES2 Wizard List
# Format: username (privilege_level)
gandalf (admin)
```

Privilege levels (lowest to highest): `(player)`, `(immortal)`, `(apprentice)`, `(wizard)`, `(arch)`, `(admin)`.

The security daemon (`adm/daemons/securityd.c`) reads this file at startup and grants privileges automatically. Restart the driver after any changes.

## Running the MUD

**Console mode** — single local user, no client needed (recommended for development):

```bash
neolith -f neolith.conf -c
```

**Network mode** — multi-user via telnet/TCP on port 4000:

```bash
neolith -f neolith.conf
```

Connect with `telnet localhost 4000` or any MUD client.

## Development

### Reloading Code

ES2 development uses a Git-first workflow. After editing LPC files:

- Reload a single object in-game: use the `update` command.
- Restart the MUD for daemon or system-wide changes.

### Log Files

| File | Written by | Contents |
|---|---|---|
| `mudlib/log/debug.log` | Neolith driver | LPC runtime messages, driver-level errors and diagnostics |
| `mudlib/log/log` | ES2 mudlib (`log_error` in `master.c`) | LPC compile-time errors and mudlib-level write denials |

When diagnosing a problem, check `debug.log` first for driver output, then `mudlib/log/log` for compile errors reported by the mudlib.

## Troubleshooting

| Symptom | Check |
|---|---|
| Driver won't start | Paths in `neolith.conf`; `mudlib/log/` directory exists |
| LPC compile error | `mudlib/log/log` (written by `master.c`:`log_error`) |
| LPC runtime error | `mudlib/log/debug.log` (written by Neolith driver) |
| Privilege not granted | `mudlib/adm/etc/wizlist` exists and driver was restarted |
| Stale binary objects | Delete `.b` files in `mudlib/bin/`; they are auto-regenerated |
