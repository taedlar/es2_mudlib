ES2 TODO items
=====

## Rewrite in-game help documents
Documents in `/doc` are out-of-date and needs re-write for current state.

### Import LPC, apply, efuns documentations from Neolith
Neolith repository has the up-to-date LPC, apply, and efuns documentations.
We should use it as the source-of-truth rather than maintain our own LPC documentations.

### Provide markdown reader
Original ES2 mudlib maintains man page style document files which is bad for version control.
We'll migrate to program-friendly markdown format document file and render it (using ANSI escape codes) in the MUD.

## Write AGENTS.md for LPC coding agent
We need to teach AI coding agent about LPC programming and ES2 mudlib contexts.

### Optimize instructions for RAG and alignment with Neolith LPC
- LLM lacks training data for LPC programming and may suffer from hallucinations (C language, MudOS and other legacy LPMuds).
- ES2 mudlib now stick with Neolith to avoid ambiguities in LPC programing language (there is no public standards for LPC).
- We need to document LPC coding conventions and mudlib architecture, mudlib subsystems and common pitfalls.

## Add testing framework for ES2 mudlib
Neolith supports console mode that allows automated LPC testing.
We'll develop ES2's testing framework using Neolith console mode.

### LPC compiler
It will be handy to have a LPC compiler CLI to let AI coding agent validate the LPC code it writes.
This can be done via Neolith's MUD application that loads LPC objects within ES2 mudlib environment.

### ES2 testbots
For mudlib functionalities, use Python testbots (as demonstrated in Neolith examples) to validate code changes.
The goal is to create basic pull-request qualification process that validates code changes.
