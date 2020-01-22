# Specification for the Remote Procedure Calls (RPC) of libaoscdk

This document specifies the structure of RPC messages running between `libaoscdk` and its front-end for message exchange.

The RPC message packets are based on the [JSON-RPC 2.0][jrpc-2] specification. This document specifies the possible request methods and responses from and to `libaoscdk`.

Document version: 0.1

## Overview

- dk
  - **dk.play**
  - **dk.stop**
  - _dk.error_
  - dk.ir
    - **dk.ir.parse**
  - dk.step
    - _dk.step.current_
    - _dk.step.percent_
    - **dk.step.max**

Legends:

- foo: namespaces
- **foo**: requests from front-end to back-end
- _foo_: notifications from back-end to front-end

## Requests and Responses

RPC requests should be sent by the font-end and received by the back-end.

### dk.play

The `dk.play` request tells `libaoscdk` to invoke the installation process according to the given [DKIR][dkir].

#### Request

Note that the `params` field is omitted.

```json
{
  "jsonrpc": "2.0",
  "method": "dk.play",
  "id": 1
}
```

#### Response

On successful execution of the installation process, `result` will be **boolean** `true`.

Note that a successful response to `dk.play` does not mean a successful installation. The later installation process is informed from `libaoscdk` to the front-end through `dk.step.current` _notifications_. See also [`dk.step.current`](#dk.step.current).

```json
{
  "jsonrpc": "2.0",
  "result": true,
  "id": 1
}
```

### dk.stop

The `dk.stop` request tells `libaoscdk` to stop the current installation process.

#### Request

```json
{
  "jsonrpc": "2.0",
  "method": "dk.stop",
  "id": 1
}
```

#### Response

On successful termination of the installation process, `result` will be **boolean** `true`. Otherwise the `error` will be set to a **string** describing the error.

```json
{
  "jsonrpc": "2.0",
  "result": true,
  "id": 1
}
```

```json
{
  "jsonrpc": "2.0",
  "error": "cannot stop the installation process",
  "id": 1
}
```

### dk.ir.parse

The `dk.ir.parse` request tells `libaoscdk` to parse the given [DKIR][dkir] and be ready for any other operations.

#### Request

```json
{
  "jsonrpc": "2.0",
  "method": "dk.ir.parse",
  "params": {},
  "id": 1
}
```

- `params` is a [DKIR][dkir] JSON object.

#### Response

On success, the `result` will be **boolean** `true`. Otherwise the `error` will be set to a **string** describing the error.

```json
{
  "jsonrpc": "2.0",
  "result": true,
  "id": 1
}
```

```json
{
  "jsonrpc": "2.0",
  "error": "cannot parse the given DKIR",
  "id": 1
}
```

### dk.step.max

The `dk.step.max` request tells `libaoscdk` to give the total number of steps in the installation process for the front-end's display.

#### Request

```json
{
  "jsonrpc": "2.0",
  "method": "dk.step.max",
  "id": 1
}
```

#### Response

The `result` will be an **integer** of the number of total steps.

```json
{
  "jsonrpc": "2.0",
  "result": 8,
  "id": 1
}
```

## Notifications

Notifications are sent from `libaoscdk` to the front-end to notify the front-end to update its prompts so that a user can be aware of what is happening.

### dk.step.percent

The `dk.step.percent` notification tells the front-end that how many percents of the **current step** has `libaoscdk` done. The front-end should display it on its user interface.

The maximum number of `percent` is 100.

```json
{
  "jsonrpc": "2.0",
  "method": "dk.step.percent",
  "params": 42
}
```

Besides a number, `null` is also possible, indicating that the percent of progress cannot be measured. In this case the front-end should display a pulsing progress bar (or its equivalent).

### dk.step.current

The `dk.step.current` notification tells the front-end that `libaoscdk` has been in which step. The front-end should display it on its user interface.

The total number of steps can be retrieved through the `dk.step.max` request from the front-end to `libaoscdk`. See also [`dk.step.max`](#dk.step.max).

```json
{
  "jsonrpc": "2.0",
  "method": "dk.step.current",
  "params": {
    "step": 1,
    "msg": "the quick brown fox jumps over the lazy dog."
  }
}
```

### dk.error

The `dk.error` notification tells the front-end that `libaoscdk` has encountered a fatal error and it is going to die. The front-end should display the message on its user interface, and gracefully quit.

```json
{
  "jsonrpc": "2.0",
  "method": "dk.error",
  "params": "received SIGBUS"
}
```

## Illegal Inputs

All illegal inputs should be processed as is described in the [JSON-RPC 2.0 specification][jrpc-2].

[dkir]: dkir-specs.md
[jrpc-2]: https://www.jsonrpc.org/specification
