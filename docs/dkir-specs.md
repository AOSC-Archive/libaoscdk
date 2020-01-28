# Specification of the AOSC OS DeployKit Internal Representation (DKIR)

The AOSC OS DeployKit "Internal Representation" (DKIR in short) is for the DeployKit backend, `libaoscdk`, to read as an input so that it can perform the installation of AOSC OS according to the user choices.

DKIR is designed to be rendered by a front-end. However, as the representation is not that complex, an advanced user can still manually modify (or create) a DKIR and supply it to `libaoscdk` to (potentially) perform more complicated operations automatically.

This document describes the structure of DKIR.

Document version: 0.1 (Unstable)

## Example

Below is a full example of DKIR.

```json
{
  "version": 0,
  "partition": {
    "format": true,
    "filesystem": "ext4",
    "device": "/dev/sda1"
  },
  "extract": {
    "mountpoint": "/media/aosc-root",
    "tarball": "/root/aosc-os.tar.xz"
  },
  "extra_packages": [
      "nvidia-proprietary",
      "devel-base",
      "ccid"
  ],
  "configuration": {
    "root_password": "tqbfjotld",
    "hostname": "localhost",
    "timezone": "Asia/Shanghai",
    "users": [
      {
        "id": "foo",
        "password": "dltojfbqt",
        "name": "Foo Bar",
        "home": "/home/foo",
        "groups": [
          "wheels",
          "netdev",
          "cdrom",
          "audio",
          "video"
        ]
      }
    ],
    "overrides": [
      {
        "from": "/root/conf/fstab",
        "to": "/etc/fstab"
      },
      {
        "from": "/root/conf/tlp",
        "to": "/etc/default/tlp"
      }
    ]
  },
  "bootloader": {
    "install": true,
    "type": "grub2",
    "target": "x86_64-efi",
    "efi_directory": "/efi",
    "bootloader_id": "AOSC GRUB"
  }
}
```

## The Version Value

The version value indicates the specification version used in a DKIR. This is the major version number of specification; value `0` stands for an unstable version of documentation.

## The Partition Configuration Object

```json
{
  "format": true,
  "filesystem": "ext4",
  "device": "/dev/sda1"
}
```

- `format`: **boolean**, whether a format should be performed. If this is `true`, then `filesystem` and `device` must exist. If this is `false`, then `filesystem` and `device` should be ignored.
- `filesystem`: **string**, the type of file system the target disk device will use.
- `device`: **string**, the device file of the target device where the operating system is going to be installed.

## The Extraction Configuration Object

```json
{
  "mountpoint": "/media/aosc-root",
  "tarball": "/root/aosc-os.tar.xz"
}
```

- `mountpoint`: **string**, the path to a directory where the target root file system is going to be mounted.
- `tarball`: **string**, the path or the URL to a tarball to be extracted.
    - If this is a path, then the tarball file will be accessed directly.
    - If this is a URL, then `libaoscdk` will first try fetching it from the Internet.

## The Extra Package Installation Array

The array specifies a list of additional packages that are going to be installed in addition to the extracted operating system image. Dependencies of these packages will be handled by an advanced package manager frontend, e.g. Apt is used instead of Dpkg to install these packages.

## The System Configuration Object

```json
{
  "root_password": "tqbfjotld",
  "hostname": "localhost",
  "timezone": "Asia/Shanghai",
  "users": [
    {
      "id": "foo",
      "password": "dltojfbqt",
      "name": "Foo Bar",
      "home": "/home/foo",
      "groups": [
        "wheels",
        "netdev",
        "cdrom",
        "audio",
        "video"
      ]
    }
  ],
  "overrides": [
    {
      "from": "/root/conf/fstab",
      "to": "/etc/fstab"
    },
    {
      "from": "/root/conf/tlp",
      "to": "/etc/default/tlp"
    }
  ]
}
```

- `root_password`: **string**, password for the `root` account.
- `hostname`: **string**, hostname for the newly-installed operating system.
- `timezone`: **string**, time zone to be set on the newly-installed operating system.
- `users`: **User[]**, a list of user objects that specify new users to be added to the newly-installed operating system.
- `overrides`: **Override[]**, a list of override objects that specify what files in the newly-installed operating system should be replaced by what.

### The User Object

```json
{
  "id": "foo",
  "password": "dltojfbqt",
  "name": "Foo Bar",
  "home": "/home/foo",
  "groups": [
    "wheels",
    "netdev",
    "cdrom",
    "audio",
    "video"
  ]
}
```

- `id`: **string**, the username (ID) of the new user.
- `password`: **string**, the password of the new user.
- `name`: **string**, the display name of the new user.
- `home`: **string**, the path to the home directory of the new user.
- `groups`: **string[]**, a list of groups that the new user is going to be added into.

### The Override Object

```json
{
  "from": "/root/conf/fstab",
  "to": "/etc/fstab"
}
```

- `from`: **string**, path to the source file to be copied from.
- `to`: **string**, path to the target where the source file is going to be copied to.

## The Bootloader Configuration Object

```json
{
  "install": true,
  "type": "grub2",
  "target": "x86_64-efi",
  "efi_directory": "/efi",
  "bootloader_id": "AOSC GRUB"
}
```

- `install`: **boolean**, whether the boot loader should be installed or not. When this is `true`, then `type` is required. When this is `false`, all other values should be ignored.
- `type`: **string**, specifies what kind of boot loader should be installed.
- `target`: **string**, specifies image generation target for GRUB2. Possible values depend on `grub-install`.
- `efi_directory`: **string**, specifies the mounting point of the EFI System Partition (ESP). This field is required for EFI platforms for boot loader installers to install themselves.
- `bootloader_id`: **string**, specifies the name of boot loader. This field is only useful for EFI platforms, where boot loaders will write themselves to the EFI boot manager with their display names.
