# Slops: SLow OPerating System

This is the Slops operating system. It is a simple unix-like operating system, written in assembly and C.

Slops is designed to be simple and easy to use. It is based on the principle of least surprise. That is, it should do what you expect it to do, and no more.

Slops is released under the Apache Licence, version 2.

# How to use

## Build iso image

```bash
make
```
And if you want to enable debug:
```bash
make DEBUG=1
```

## Boot the system

```bash
make run
```

## Remove artifacts

```bash
make clean
```

or if you want to remove all generated files run:

```bash
make distclean
```
