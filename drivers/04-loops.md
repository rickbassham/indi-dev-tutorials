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

## Driver Loop

You'll probably want to do more than just respond to user input in your driver.
If you need to periodically check things, the main driver loop is where to do it.

At it's most basic, you really just need to override `TimerHit`.

```cpp
void MyCustomDriver::TimerHit()
{
    if (!isConnected())
        return;

    LOG_INFO("timer hit");

    // If you don't call SetTimer, we'll never get called again, until we disconnect
    // and reconnect.
    SetTimer(POLLMS);
}
```

After `Handshake` is called when connecting, `DefaultDevice` will call `TimerHit` after `POLLMS` elapses. We have to make sure to set the timer again at the end, otherwise it'll never get called
again (until we reconnect that is).
