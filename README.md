# Hardware Wallet

## Setup

Install an arm toolchain, such as the `arm-none-eabi-{gcc,binutils,newlib,gdb}` packages from your distro.
This should also be available for manual install from https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads.

Download the `nRF5x-Command-Line-Tools` from your distro or https://www.nordicsemi.com/eng/Products/nRF52840#Downloads.

Download the `J-Link Software and Documentation Pack` from your distro or https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack.

All of the above tools should be in your `PATH` (edit ~/.bashrc if needed).

Download the Nordic nRF5 SDK from https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF5-SDK
and set the `NORDIK_SDK` environment variable (export NORDIK_SDK=... inside ~/.bashrc).


### Creating a build directory

Note: any time you change a cmake file you must run `cmake ..` again.

```
mkdir build
cd build
cmake ..
```

### Installing libraries

```
./setup-mpack.sh
```

### Flash the softdevice

This will supply the bluetooth firmware stack used by this project. This should only need to be done once.

```
cd build
make flash_softdevice
```

### Ctags

Ctags allow you to quicky jump to a symbol's definition. This is especially useful for large projects like nordic's SDK.

For vim the keybinds are as follows:


| Jump to definition | Ctrl-] |
| Jump to file | gf |
| Jump back | Ctrl-T |


```
cd build
make ctags
```

## Compiling

```
cd build
make
```

## Running

```
cd build
make upload
```

## CLI & Log output

Notes: debug-server will also work as a backend, so if you are already using gdb you do not need to run jlink.

For seemingly unknown reasons when closing and reopening the rtt it will not connect correctly.
To fix this close restart your jlink/debug-server session.
You should see a `Process: XXX` line after 'Connected.' if things are working right.

You may not see any output until a new message is emitted, or you type a command.
Consider resetting thed device with `make reset` to see all output.

```
cd build
make jlink
make rtt
```

Current log levels can be seen with `$ log status` at the prompt.

Log levels can be changed for a module with `$ log enable <level> <module ...>`

## Debugging

Note: GDB is useful to inspect variables at breakpoints after exceptions;
however, manually stoping execution or stepping through code will break the
sensitive timing of the soft device and lead to issues.

```
cd build
make debug-server
make debug
```
