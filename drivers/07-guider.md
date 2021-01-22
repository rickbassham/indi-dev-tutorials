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
* [Coming From ASCOM](09-coming-from-ascom.md)

## Guider

The best and simplest example of a working `GuiderInterface` is the `ArduinoST4` driver.
This class is clean and minimal and adding an explanation here would be redundant.

The only thing implemented in this class not covered by the tutorials so far is the
timer functions `IEAddTimer` and `IERmTimer`, but they should be pretty straight forward
to understand. They are just adding/removing delayed callbacks to functions.

If you want to know how to implement a guider driver, look at `arduino_st4.h` and
`arduino_st4.cpp` in the [main INDI library](https://github.com/indilib/indi).
