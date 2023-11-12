# Alpakka Firmware

*Alpakka controller reference firmware (for Raspberry Pi Pico)*
## Project links
- [Alpakka Manual](https://inputlabs.io/devices/alpakka/manual).
- [Alpakka Firmware](https://github.com/inputlabs/alpakka_firmware). _(you are here)_
- [Alpakka PCB](https://github.com/inputlabs/alpakka_pcb).
- [Alpakka 3D-print](https://github.com/inputlabs/alpakka_case).
- [Input Labs Roadmap](https://github.com/orgs/inputlabs/projects/2/views/2).

## Supported developer operative systems
- GNU/Linux (and MacOS) - See [Development in Linux](https://inputlabs.io/devices/alpakka/manual/dev_unix).
- Windows - See [Development in Windows](https://inputlabs.io/devices/alpakka/manual/dev_windows).

## System dependencies
With `apt`, `rpm`, `pacman`, `brew`, or the equivalent package manager of your system, install:
- **gcc**
- **git**
- **cmake**

## Project dependencies
- `make install`: Download and configure dependencies automatically.

## Development commands
- `make`: Build compilation environment and build executables.
- `make rebuild`: Build executables again using cache (faster).
- `make load`: Load built .uf2 file into the Pico (requires bootsel mode or active session).
- `make reload`: Do both `rebuild` and `load` commands (for dev convenience).
- `make clean`: Delete previous build files.
- `make session`: Connect to UART serial stdio, and display controller log.

While having an active session:
- `make restart`: Restart the controller.
- `make bootsel`: Put the controller in bootsel mode.
- `make calibrate`: Calibrate thumbstick and IMUs.
- `make format`: Format NVM sector and reset to initial values.
- `make test`: Start a semi-manual testing procedure for the buttons and axis.

## Devkit button
- Single press: Restart the controller.
- Double press: Put the controller in bootsel mode.

Read the [Alpakka developer manual](https://inputlabs.io/devices/alpakka/manual/dev) for details about the **devkit** and more.

##Troubleshooting

After cloning this repository change folder name from alpakka_pf_firmware to alpakka_firmware

Then follow install instructions from https://inputlabs.io/alpakka/manual/dev_unix or https://inputlabs.io/alpakka/manual/dev_windows

If "make load" stops at "Waiting for Pico in Bootsel mode / RPI-RP2 drive" then it might be trying to install the uf2 file into /media/RPI-RP2 instead of /media/(whatever your user is)/RPI-RP2. What helped me is going to /alpakka_firmware/build in the file explorer and dragging the alpakka.uf2 file directly into the RPI-RP2 drive.

When the controller restarts enter fps_fusion profile to play Phantom Forces.
