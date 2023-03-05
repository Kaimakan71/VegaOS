# VegaOS
VegaOS is a contemporary operating system designed for the x86_64 and arm64 (aka aarch64) architectures. It is written entirely in the Rust programming language and features a modern kernel, drivers, and various system tools, making it a fully-functional and secure operating system designed for future.

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
