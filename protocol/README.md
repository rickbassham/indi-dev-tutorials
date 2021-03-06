# INDI Protocol

As stated elsewhere, the INDI commmunication protocol is XML over TCP. Most of
the info here is summarized from the [INDI Protocol](http://www.clearskyinstitute.com/INDI/INDI.pdf).
You can see the full DTD there. I'm not going to cover ALL messages here, specifically
around handling of BLOBs. For that, please see the above documentation.

Because INDI at its heart is just XML over TCP, you don't actually NEED to use
the C++ indi library to write your own driver or client, but `libindi` does handle
a lot of the heavy lifting for you.

## Custom Types

Here are some custom types used by the INDI Protocol.

### `propertyState`

| Name  | XML Value | C++ Value   | Notes |
| ----- | --------- | ----------- | ----- |
| Idle  | `Idle`    | `IPS_IDLE`  |       |
| Ok    | `Ok`      | `IPS_OK`    |       |
| Busy  | `Busy`    | `IPS_BUSY`  |       |
| Alert | `Alert`   | `IPS_ALERT` |       |

### `switchState`

| Name | XML Value | C++ Value | Notes |
| ---- | --------- | --------- | ----- |
| Off  | `Off`     | `ISS_OFF` |       |
| On   | `On`      | `ISS_ON`  |       |

### `switchRule`

| Name      | XML Value   | C++ Value     | Notes |
| --------- | ----------- | ------------- | ----- |
| OneOfMany | `OneOfMany` | `ISR_1OFMANY` |       |
| AtMostOne | `AtMostOne` | `ISR_ATMOST1` |       |
| AnyOfMany | `AnyOfMany` | `ISR_NOFMANY` |       |

### `propertyPerm`

| Name | XML Value | C++ Value | Notes                                                                              |
| ---- | --------- | --------- | ---------------------------------------------------------------------------------- |
| ro   | `ro`      | `IP_RO`   | only the driver can update the value of this property, a client cannot update it   |
| wo   | `wo`      | `IP_WO`   | the driver will not update the value of this property, only a client can update it |
| rw   | `rw`      | `IP_RW`   | either a client or the driver can update the value of this property                |

### `BLOBenable`

| Name  | XML Value | C++ Value | Notes |
| ----- | --------- | --------- | ----- |
| Never | `Never`   | `B_NEVER` |       |
| Also  | `Also`    | `B_ALSO`  |       |
| Only  | `Only`    | `B_ONLY`  |       |

## XML Commands

### Device -> Client

These XML messages are sent from the device to clients (which may be other devices).
C++ functions that send these have the prefix `ID`.

#### `getProperties`

When sent from a device to a client (in this case it will be another device) this
tells INDI that we want to "snoop" on the given device for updates to its properties.

This message has the following attributes:

| Name      | Required | Notes |
| --------- | -------- | ----- |
| `version` | Y        |       |
| `device`  | Y        |       |
| `name`    | N        |       |

When sent from a device, you must at least specify the target device, and it is
recommended that you specify the property you want to snoop on as well.

The C++ function that sends this message in `libindi` is `IDSnoopDevice`.

Example XML:

```xml
<getProperties version="1.7" device="Telescope Simulator" name="EQUATORIAL_COORD" />
```

#### `defTextVector`

A device sends this message to define a new text vector property.

This message has the following attributes:

| Name        | Required | Notes                                                         |
| ----------- | -------- | ------------------------------------------------------------- |
| `device`    | Y        | name of the device                                            |
| `name`      | Y        | name of the property                                          |
| `label`     | N        | friendly name of the property                                 |
| `group`     | N        | property group membership (tab in the UI)                     |
| `state`     | Y        | current state of the property (one of `propertyState` values) |
| `perm`      | Y        | property permissions (one of the `propertyPerm` values)       |
| `timeout`   | N        | delay before the client assumes an update went wrong          |
| `timestamp` | N        | moment the data was valid                                     |
| `message`   | N        | commentary to send                                            |

And contains one or more `defText` elements, which have the following attributes:

| Name    | Required | Notes                      |
| ------- | -------- | -------------------------- |
| `name`  | Y        | name of the value          |
| `label` | N        | friendly name of the value |

The C++ function that sends this message in `libindi` is `IDDefText`, but it is
recommended to use the `defineProperty` method that wraps it.

Example XML:

```xml
<defTextVector device="CCD Simulator" name="CCD_CFA" label="Bayer Info"
    group="Image Info" state="Idle" perm="rw" timeout="60">
    <defText name="CFA_OFFSET_X" label="X Offset" />
    <defText name="CFA_OFFSET_Y" label="Y Offset" />
    <defText name="CFA_TYPE" label="Filter" />
</defTextVector>
```

#### `defNumberVector`

A device sends this message to define a new number vector property.

This message has the following attributes:

| Name        | Required | Notes                                                         |
| ----------- | -------- | ------------------------------------------------------------- |
| `device`    | Y        | name of the device                                            |
| `name`      | Y        | name of the property                                          |
| `label`     | N        | friendly name of the property                                 |
| `group`     | N        | property group membership (tab in the UI)                     |
| `state`     | Y        | current state of the property (one of `propertyState` values) |
| `perm`      | Y        | property permissions (one of the `propertyPerm` values)       |
| `timeout`   | N        | delay before the client assumes an update went wrong          |
| `timestamp` | N        | moment the data was valid                                     |
| `message`   | N        | commentary to send                                            |

And contains one or more `defNumber` elements, which have the following attributes:

| Name     | Required | Notes                                                    |
| -------- | -------- | -------------------------------------------------------- |
| `name`   | Y        | name of the value                                        |
| `label`  | N        | friendly name of the value                               |
| `format` | Y        | printf style format; must be suitable for a double value |
| `min`    | Y        | minimum value                                            |
| `max`    | Y        | maximum value                                            |
| `step`   | Y        | increment value; ignored if 0                            |

The values for `format` can be any C printf specifier that is valid for a double.
See [printf](http://www.cplusplus.com/reference/cstdio/printf/) for more info.
In addition to the normal double formats, INDI also supports formatting a number
as sexagesimal format, using the `%m` specifier. You can use this specifier like
so: `%[w].[f]m` where:

* [w] is the total field width
* [f] is the width of the fraction
  * valid values for `f` are
    * 9 -> ":mm:ss.ss"
    * 8 -> ":mm:ss.s"
    * 6 -> ":mm:ss"
    * 5 -> ":mm.m"
    * 3 -> ":mm"

So, if you want to get a formatted number like `"-123:45"` use `"%7.3m"` or
`"  0:01:02"` use `"%9.6m"`.

The C++ function that sends this message in `libindi` is `IDDefNumber`, but it is
recommended to use the `defineProperty` method that wraps it.

Example XML:

```xml
<defNumberVector device="Telescope Simulator" name="TELESCOPE_TIMED_GUIDE_NS"
    label="Guide N/S" group="Guider" state="Idle" perm="rw" timeout="60">
    <defNumber name="TIMED_GUIDE_N" label="North (ms)" format="%.f" min="0" max="60000" step="100" />
    <defNumber name="TIMED_GUIDE_S" label="South (ms)" format="%.f" min="0" max="60000" step="100" />
</defNumberVector>
```

#### `defSwitchVector`

A device sends this message to define a new switch vector property.

This message has the following attributes:

| Name        | Required | Notes                                                         |
| ----------- | -------- | ------------------------------------------------------------- |
| `device`    | Y        | name of the device                                            |
| `name`      | Y        | name of the property                                          |
| `label`     | N        | friendly name of the property                                 |
| `group`     | N        | property group membership (tab in the UI)                     |
| `state`     | Y        | current state of the property (one of `propertyState` values) |
| `perm`      | Y        | property permissions (one of the `propertyPerm` values)       |
| `rule`      | Y        | UI hint (one of the `switchRule` values)                      |
| `timeout`   | N        | delay before the client assumes an update went wrong          |
| `timestamp` | N        | moment the data was valid                                     |
| `message`   | N        | commentary to send                                            |

And contains one or more `defSwitch` elements, which have the following attributes:

| Name    | Required | Notes                      |
| ------- | -------- | -------------------------- |
| `name`  | Y        | name of the value          |
| `label` | N        | friendly name of the value |

The C++ function that sends this message in `libindi` is `IDDefSwitch`, but it is
recommended to use the `defineProperty` method that wraps it.

Example XML:

```xml
<defSwitchVector device="Telescope Simulator" name="TELESCOPE_PARK"
    label="Parking" group="Main" state="Idle" perm="rw" timeout="60" rule="OneOfMany">
    <defSwitch name="PARK" label="Park(ed)" />
    <defSwitch name="UNPARK" label="UnPark(ed)" />
</defSwitchVector>
```

#### `defLightVector`

A device sends this message to define a new light vector property.

This message has the following attributes:

| Name        | Required | Notes                                                         |
| ----------- | -------- | ------------------------------------------------------------- |
| `device`    | Y        | name of the device                                            |
| `name`      | Y        | name of the property                                          |
| `label`     | N        | friendly name of the property                                 |
| `group`     | N        | property group membership (tab in the UI)                     |
| `state`     | Y        | current state of the property (one of `propertyState` values) |
| `timestamp` | N        | moment the data was valid                                     |
| `message`   | N        | commentary to send                                            |

And contains one or more `defLight` elements, which have the following attributes:

| Name    | Required | Notes                      |
| ------- | -------- | -------------------------- |
| `name`  | Y        | name of the value          |
| `label` | N        | friendly name of the value |

The C++ function that sends this message in `libindi` is `IDDefLight`, but it is
recommended to use the `defineProperty` method that wraps it.

Example XML:

```xml
<defLightVector device="Weather Simulator" name="WEATHER_STATUS"
    label="Status" group="Main" state="Idle">
    <defLight name="WEATHER_FORECAST" label="Weather" />
    <defLight name="WEATHER_TEMPERATURE" label="Temperature (C)" />
</defLightVector>
```

#### `defBLOBVector`

A device sends this message to define a new BLOB vector property.

This message has the following attributes:

| Name        | Required | Notes                                                         |
| ----------- | -------- | ------------------------------------------------------------- |
| `device`    | Y        | name of the device                                            |
| `name`      | Y        | name of the property                                          |
| `label`     | N        | friendly name of the property                                 |
| `group`     | N        | property group membership (tab in the UI)                     |
| `state`     | Y        | current state of the property (one of `propertyState` values) |
| `perm`      | Y        | property permissions (one of the `propertyPerm` values)       |
| `timeout`   | N        | delay before the client assumes an update went wrong          |
| `timestamp` | N        | moment the data was valid                                     |
| `message`   | N        | commentary to send                                            |

And contains one or more `defBLOB` elements, which have the following attributes:

| Name    | Required | Notes                      |
| ------- | -------- | -------------------------- |
| `name`  | Y        | name of the value          |
| `label` | N        | friendly name of the value |

The C++ function that sends this message in `libindi` is `IDDefBLOB`, but it is
recommended to use the `defineProperty` method that wraps it.

Example XML:

```xml
<defBLOBVector device="CCD Simulator" name="CCD1"
    label="Image Data" group="Main" state="Idle" perm="ro" timeout="60">
    <defBLOB name="CCD1" label="Image" />
</defBLOBVector>
```

#### `setTextVector`

A device sends this message to inform clients about an update to an existing
text vector property.

The C++ function that sends this message in `libindi` is `IDSetText`.

#### `setNumberVector`

A device sends this message to inform clients about an update to an existing
number vector property.

The C++ function that sends this message in `libindi` is `IDSetNumber`.

#### `setSwitchVector`

A device sends this message to inform clients about an update to an existing
switch vector property.

The C++ function that sends this message in `libindi` is `IDSetSwitch`.

#### `setLightVector`

A device sends this message to inform clients about an update to an existing
light vector property.

The C++ function that sends this message in `libindi` is `IDSetLight`.

#### `setBLOBVector`

A device sends this message to inform clients about an update to an existing
BLOB vector property.

The C++ function that sends this message in `libindi` is `IDSetBLOB`.

#### `message`

A device sends this message to send a logging message to clients.

The C++ function that sends this message in `libindi` is `IDMessage`.

#### `delProperty`

A device sends this message to inform clients that a property has been deleted.

The C++ function that sends this message in `libindi` is `IDDelete`, but it is
recommended to use the `deleteProperty` method that wraps it.

### Client -> Device

These XML messages are sent from the device to clients (which may be other devices).

#### `getProperties`

When sent from a client to a device this tells the device to send define all of
the current properties on the device.

This message has the following attributes:

| Name      | Required | Notes |
| --------- | -------- | ----- |
| `version` | Y        |       |
| `device`  | N        |       |
| `name`    | N        |       |

The client does NOT need to specify the device, but if it specifies the property
name, it must also specify the device.

The C++ method on a driver that handles this message is `ISGetProperties`.

Example XML:

```xml
<getProperties version="1.7" device="" name="" />
```

#### `enableBLOB`

This message can be sent AFTER sending a `getProperties` message to enable or
disable transfer of BLOBs. BLOBs are properties that can be quite large, typically
full FITS files, so if you want to snoop on them, you have to explicitly enable it.

This message has the following attributes:

| Name     | Required | Notes |
| -------- | -------- | ----- |
| `device` | Y        |       |
| `name`   | N        |       |

The C++ function that sends this message in `libindi` is `IDSnoopBLOBs`.

#### `newTextVector`

A client sends this message to inform a device about a request to change the value
of a text vector property.

The C++ method on a driver that handles this message is `ISNewText`.

#### `newNumberVector`

A client sends this message to inform a device about a request to change the value
of a number vector property.

The C++ method on a driver that handles this message is `ISNewNumber`.

#### `newSwitchVector`

A client sends this message to inform a device about a request to change the value
of a switch vector property.

The C++ method on a driver that handles this message is `ISNewSwitch`.

#### `newBLOBVector`

A client sends this message to inform a device about a request to change the value
of a BLOB vector property.

The C++ method on a driver that handles this message is `ISNewBLOB`.
