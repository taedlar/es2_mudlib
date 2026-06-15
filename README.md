# README
[ES2](https://zh.wikipedia.org/zh-tw/%E6%9D%B1%E6%96%B9%E6%95%85%E4%BA%8B2_%E5%A4%A9%E6%9C%9D%E5%B8%9D%E5%9C%8B) is a LPMud started by
[Annihilator](https://github.com/taedlar) in 1994. It was the first open-source LPMud in Taiwan and the Chinese-speaking community.
Many Chinese MUDs were later built on this codebase, each growing into its own world.

## Usage

Starting with v1.5, ES2 Mudlib uses [Neolith](https://github.com/taedlar/neolith) as a Git submodule dependency.
This keeps mudlib and driver source together in one checkout, giving modern coding agents a monorepo-like code context while preserving separate repositories.
Neolith also provides better cross-platform consistency on **Linux/Windows/macOS** and modern LPC features (such as dot-call syntax).

ES2 development is moving from in-game editing toward a Git-first workflow.
For that reason, we strongly recommend cloning with driver sources (`--recurse-submodules`) so your local workspace includes the full mudlib + driver context.

> [!NOTE]
> ES2 Mudlib v1.5 runs only on Neolith. This follows the original LPMud model: driver + mudlib as an integrated system.
>
> Neolith remains **backward compatible** with MudOS-level LPC, so older MudOS-based mudlibs can still run on it.
> ES2 Mudlib v1.5 depends on Neolith extensions and is no longer compatible with MudOS.

To run ES2 Mudlib:
1. `git clone --recurse-submodules` to fetch a copy of ES2 Mudlib and Neolith source files.
2. Build Neolith. See `/neolith/docs/INSTALL.md`.
3. Edit the configuration file [neolith.conf](neolith.conf) if necessary.
4. Start the MUD:
```bash
# If you want to run in console mode, use the `-c` option.
/path/to/neolith -f neolith.conf
```

### Client Software
- ES2 Mudlib supports classic telnet clients and VT100 terminals with ANSI colors.
- You can also use a MUD client such as [TinTin++](https://github.com/scandum/tintin), or [Mudlet](https://github.com/Mudlet/Mudlet).

### Neolith Console Mode
ES2 Mudlib supports Neolith **console mode**, which lets you connect directly through the host console.
This is useful for quick local testing and automation workflows.

Whether you are exploring old-school MUD development, preserving legacy systems, or building new gameplay ideas, this mode keeps iteration fast.
It is also a natural fit for "single-user MUD" play and development: one process, one terminal, no client setup required.
If you have seen forum discussions about single-user mudding, this is the workflow they usually mean.

To connect in console mode, add the `-c` option to neolith:
```bash
neolith -f neolith.conf -c
```
> [!CAUTION]
> External ports still work in console mode.
> Pressing `Ctrl-C` stops the MUD server process and disconnects all users immediately.
> If you desire a true single-user world, disable all `Port` settings in the configuration file.

## Versioning
- The `main` branch is for v1.5 onwards and requires the Neolith driver (not compatible with MudOS).
- The `v1.4` branch is the legacy version compatible with MudOS or FluffOS (with minor fixes).
- A GitHub workflow `release.yml` is added to automate ES2 mudlib main branch releases.

## Community

Contributions are welcome from both long-time MUD developers and newcomers.
If you are improving docs, fixing bugs, modernizing tooling, or sharing gameplay content, your help is appreciated.

Start with [CONTRIBUTING.md](CONTRIBUTING.md) and [CODE_OF_CONDUCT.md](docs/CODE_OF_CONDUCT.md).

## License Notes

Early ES2 Mudlib releases used a custom notice that prohibited commercial use and required displaying the name "ES2 mudlib".
Since 2022, Annihilator has released ES2 Mudlib on GitHub under the **MIT License**.
The initial GitHub commit is based on ES2 Mudlib v1.3b. Earlier ES2 releases may also be used under MIT terms.

With the MIT license, please note:
- **Commercial use**: ES2 Mudlib is MIT-licensed, but it needs a compatible driver. ES2 currently depends on [Neolith](https://github.com/taedlar/neolith), which follows **fair-code style** terms aligned with upstream LPMud restrictions and does **not** allow commercial use.
- **Alternative driver**: You may build your own compatible clean-room driver that does not reuse MudOS/LPMud code.
- **Displaying "ES2 mudlib"**: This is no longer required under MIT, though existing users may continue to do so.
- **Open source**: You are not required to open-source your MUD game when using ES2 Mudlib.
