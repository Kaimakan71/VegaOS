# VegaOS
VegaOS is a contemporary operating system designed for the x86_64 and arm64 (aka aarch64) architectures. It is written entirely in the Rust programming language and features a modern kernel, drivers, and various system tools, making it a fully-functional and secure operating system designed for future.

## Getting started
These instructions will guide you through the process of building VegaOS from source and installing it on your machine.

### Prerequirements
- Rust (latest stable release)
- NASM (latest stable release)
- gcc (latest stable release)
- QEMU or Virtualbox

### Building VegaOS
To build VegaOS, clone the source code from the Github repository:
```bash
git clone https://github.com/Vega-OS/src
cd src
```
Once the source code is downloaded, build VegaOS by running:
```bash
make iso
```
This will generate the ISO image, which can be used to install VegaOS on your machine.

### Installing & Running VegaOS
You can install VegaOS on your machine natively, or using install it on an virtual machine like QEMU or Virtualbox.

#### QEMU
To install VegaOS on QEMU, run the following command:
```bash
make run
```

#### Virtualbox
To install VegaOS on Virtualbox, build VegaOS (using command `make iso`) and use generated iso in new virtual machine in your Virtualbox.

## Contributing
We welcome contributions from everyone. If you would like to contribute to VegaOS, please read our [contributing guidelines](CONTRIBUTING.md).

## License
VegaOS is licensed under [The 3-Clause BSD License](LICENSE).
