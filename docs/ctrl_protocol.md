# Ctrl protocol

## Introduction

The Ctrl protocol is a minimalistic bidirectional communication protocol between the controller and the web app, using USB (WebUSB). For the purpose of exchanging data such as logging, configuration settings and profile mappings.

The exchange is performed with discrete packages of at most 64 bytes.

Both sides of the communication (USB host and USB device) are equally-levelled actors from the point of view of the protocol, both sides can initiate the communication (send a message) at any time.

There is no acknowledge when receiving messages at protocol level, though some messages could trigger a response message from the other side. In such cases the response message is asynchronous, and can happen at any time later or not happen at all. Is up to the "receiver" to determine adequate timeout values.

Verification and error correction is provided by the USB protocol.

Padding zeros to complete the 64 bytes may be used. Therefore the protocol avoids using zero as indexes for any of its enums.

## Package general structure

| Byte 0 | 1 | 2 | 3 | 4~63 |
| - | - | - | - | - |
| Protocol version | Device Id | Message type | Payload size | Payload

### Protocol version

| Key | Index |
| - | - |
| VERSION 1 | 1 |

### Device id

| Key | Index |
| - | - |
| ALPAKKA | 1
| KAPYBARA | 2

### Message type

| Key | Index |
| - | - |
LOG | 1
PROC | 2
CONFIG_GET | 3
CONFIG_SET | 4
CONFIG_SHARE | 5
PROFILE_GET | 10
PROFILE_SET | 11
PROFILE_SHARE | 12

## Log message
Message output by the firmware, as strings of arbitrary size.

Direction: `Controller` -> `App`

| Byte 0 | 1 | 2 | 3 | 4~63 |
| - | - | - | - | - |
| Protocol version | Device Id | Message type | Payload size | Payload
|                  |           | LOG          | 1-60         | Log message

## Proc message
Trigger a procedure (eg: calibration) on the controller.

Direction: `Controller` <- `App`

| Byte 0 | 1 | 2 | 3 | 4 |
| - | - | - | - | - |
| Version | Device Id | Message type | Payload size | Payload
|         |           | PROC         | 1            | PROC INDEX

### Procedure index
Procedure index as defined in [hid.h](/src/headers/hid.h#L179).

## Config GET message
Request the current value of some specific configuration parameter.

Direction: `Controller` <- `App`

| Byte 0 | 1 | 2 | 3 | 4 |
| - | - | - | - | - |
| Version | Device Id | Message type | Payload size | Payload
|         |           | CONFIG_GET   | 1            | CONFIG INDEX

## Config SET message
Change the value of some specific configuration parameter.

Direction: `Controller` <- `App`

| Byte 0 | 1 | 2 | 3 | 4 | 5 | 6~10
| - | - | - | - | - | - | - |
| Version | Device Id | Message type | Payload size | Payload      | Payload       | Payload
|         |           | CONFIG_SET   | 7            | CONFIG INDEX | PRESET INDEX  | PRESETS VALUE

## Config SHARE message
Notify the current value of some specific configuration parameter.

Direction: `Controller` -> `App`

| Byte 0 | 1 | 2 | 3 | 4 | 5 | 6~10 |
| - | - | - | - | - | - | - |
| Version | Device Id | Message type | Payload size | Payload      | Payload       | Payload
|         |           | CONFIG_SHARE | 7            | CONFIG INDEX | PRESET INDEX  | PRESETS VALUE

### Config index

| Key | Index |
| - | - |
| PROTOCOL | 1
| SENS_TOUCH | 2
| SENS_MOUSE | 3
| DEADZONE | 4

## Profile messages
TODO

## Example of config interchange

(Imagine this is a sequence diagram)
| App |  | Controller
| - | - | - |
| App is about to display current mouse sens
| App request the current mouse sens value | -- CONFIG_GET --> | OK
| OK | <-- CONFIG_SHARE -- | Controller share the current value
| App display the current mouse sensitivity
| User changes mouse sensitivity in-app
| App request to change the current value | -- CONFIG_SET --> | OK
| OK | <-- CONFIG_SHARE -- | Controller share the new current value
| App display the new mouse sensitivity
