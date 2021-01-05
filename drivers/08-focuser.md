# INDI Driver Development

* [Full Example](https://github.com/rickbassham/indi-dev-tutorials/tree/main/drivers/indi_mycustomdriver)
* [Project Setup](00-project-setup.md)
* [Simple Driver](01-simple.md)
* [Properties](02-properties.md)
* [Serial Connection](03-serialconnection.md)
* [Driver Loop](04-loops.md)
* [Helpful Functions](05-helpful-functions.md)
* [Driver Interface](06-driver-interface.md)
* [Guider](07-guider.md)
* [Focuser](08-focuser.md)

## Focuser

Now let's talk about the `Focuser` base class.

This class, along with several of the other base classes available, adds some
extra standard properties, like being able to choose between serial or TCP
communication.

You can tell the base class which type of connection your driver supports by
calling `setSupportedConnections` in your constructor.

You can also tell the base `Focuser` class what capabilities your focuser has
by calling `SetCapability` in your constructor. This is a common paradigm across
INDI base classes. The capabilities you set here will determine which methods you
will need to override.

```cpp
enum
{
    FOCUSER_CAN_ABS_MOVE       = 1 << 0, /*!< Can the focuser move by absolute position? */
    FOCUSER_CAN_REL_MOVE       = 1 << 1, /*!< Can the focuser move by relative position? */
    FOCUSER_CAN_ABORT          = 1 << 2, /*!< Is it possible to abort focuser motion? */
    FOCUSER_CAN_REVERSE        = 1 << 3, /*!< Is it possible to reverse focuser motion? */
    FOCUSER_CAN_SYNC           = 1 << 4, /*!< Can the focuser sync to a custom position */
    FOCUSER_HAS_VARIABLE_SPEED = 1 << 5, /*!< Can the focuser move in different configurable speeds? */
    FOCUSER_HAS_BACKLASH       = 1 << 6  /*!< Can the focuser compensate for backlash? */
} FocuserCapability;
```

See the `indi_dummy_focuser` in this project for a barebones example of a focuser.
