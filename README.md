# NetworkResolver

## Overview
NetworkResolver is a command-line tool designed to facilitate network diagnostics and information retrieval. It provides functionalities for resolving hostnames to IP addresses, performing reverse DNS lookups, and displaying service ports in various formats. This tool supports both IPv4 and IPv6 addresses and allows users to specify the transport protocol (TCP/UDP), the byte order for port numbers, and output formatting options.

## Features
- **Hostname Resolution**: Resolve hostnames to IP addresses.
- **Reverse DNS Lookup**: Perform reverse lookups to find hostnames associated with IP addresses.
- **Protocol Specification**: Choose between TCP and UDP protocols for service lookup.
- **Output Formatting**: Display port numbers in decimal or hexadecimal format.
- **Byte Order Selection**: Choose between host and network byte order for displaying port numbers.
- **IPv4 and IPv6 Support**: Support for both IPv4 and IPv6 address formats.

## Usage

prog [-r] [-t|-u] [-x] [-h|-n] [-46] {hostname | IP_address} {servicename | port}


- `-t` TCP (default if neither `-t` nor `-u` is specified)
- `-u` UDP
- `-x` Hexadecimal output for port numbers
- `-h` Host byte order for port numbers (default)
- `-n` Network byte order for port numbers
- `-4` Use IPv4 (default if neither `-4` nor `-6` is specified)
- `-6` Use IPv6
- `-r` Perform a reverse DNS lookup

