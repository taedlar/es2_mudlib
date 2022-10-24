# README
[ES2](https://zh.wikipedia.org/zh-tw/%E6%9D%B1%E6%96%B9%E6%95%85%E4%BA%8B2_%E5%A4%A9%E6%9C%9D%E5%B8%9D%E5%9C%8B) is a LPMud started by
[Annihilator](https://github.com/taedlar) since 1994. It is the first open source LPMud in Taiwan (and in the whole Chinese culture).
There are many Chinese MUDs thereafter started base on its code base and developed their own contents.

## Usage

ES2 Mudlib is compatible to the following LPMud drivers:
- [Neolith](https://github.com/taedlar/neolith) (*recommended*)
- MudOS v22 (probably dead)
> It might be possible to run ES2 mudlib on popular [FluffOS](https://github.com/fluffos/fluffos), with some efforts.

To run ES2 Mudlib,
1. Build Neolith
2. `git clone` a copy of ES2 Mudlib from this repository to a local directory.
3. Make a copy of [neolith.conf](neolith.conf) and edit the path settings to fit your local directory.
4. Run  
```
neolith -f neolith.conf
```
5. Then you shall be able to connect to the mud by `telnet localhost 4000`. Enjoy.

### Client Software
- ES2 Mudlib is compatible with classical telnet client and vt100 terminal, with ANSI coloring.
- You can also use a MUD client such as [TinTin++](https://github.com/scandum/tintin), or [Mudlet](https://github.com/Mudlet/Mudlet).

### Differences from v1.3
- Changed to UTF-8. No longer using Big-5 encoding.
- Tested with Neolith only.
- Favors `git` based source control than in-game programming. May remove certain wizard commands in the future.
- Favors `vim` editor for its ability to [syntax highlight](docs/syntax_highlight.md) LPC programs.
- No longer release as specific version. If you plan to stick on particular version of ES2 Mudlib, use git commit id instead.

## License

The original release of ES2 mudlib is licensed under a roughly written notice that forbids commercial-use and requires displaying the name
of "ES2 mudlib". Since 2022, it is released on github by Annihilator and placed under **MIT license**, so those old restrictions are no
longer required. Please note the initial commit in github repository is based on the ES2 mudlib v1.3b release. Annihilator also grant you
the choice to use any earlier released versions of ES2 mudlib under MIT license for less restrictions.

With the change of license, please note:

### Commercial Use

**Yes**, the ES2 mudlib under MIT licnese allows commercial-use.  

However, a LPMud mudlib requires a compatible LPMud driver to run on. Currently ES2 mudlib is known compatible with MudOS and
[Neolith](https://github.com/taedlar/neolith) (a MudOS fork, specially customized for ES2 mudlib), while both of them do not allow commercial-use
due to the restrictions of original LPMud source from Lars.

### Displaying "ES2 mudlib"

You are **no longer required** to display the name of ES2 mudlib if you start a mud with the ES2 mudlib. Annihilator still hold the copyright
of the name "ES2 mudlib". Since the original license requires (and grant user the rights) to display the name, changing to MIT license should
not affect how old users use the name. Annihilator grant you the PERMISSION TO DISPLAY "ES2 mudlib" in you MUD's login screen with the
disclaimer that ES2 mudlib is PROVIDED AS-IS and SHALL NOT BE LIABLE TO ANY DAMAGE possibly caused by the software to your assets or users.

### Open Source

You are **not required** to open source your MUD game built on ES2 mudlib. This is technically the same in old license and in MIT license,
and this is why MIT license is choosed rather than GPL.
