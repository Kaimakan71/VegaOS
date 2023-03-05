# VegaOS
VegaOS is a microkernel-based operating system designed to be secure and usable on both AARCH64 and x86_64 platforms. 
We aim to provide a user-friendly interface and support for common user-level utilities and tools, while also being powerful enough for advanced users. 

Note: VegaOS is in an early state.

## Getting started
These instructions will guide you through the process of building VegaOS from source and installing it on your machine.

### Prerequisites
- gcc (latest stable release)
- QEMU

### Building VegaOS for x86_64

```bash
./configure
make cross_x86_64
make
```

### Building VegaOS for AARCH64
```bash
./configure --enable-aarch64
make cross_aarch64
make
```

This will generate the ISO image, which can be used to install VegaOS on your machine.

#### QEMU
To install VegaOS on QEMU, run the following command:
```bash
make run
```

## License
VegaOS is licensed under [The 3-Clause BSD License](LICENSE).
