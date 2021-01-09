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

This tutorial assumes you have setup your project structure to match [project-setup](00-project-setup.md).

## A Simple Driver

Here is an example of a very simple driver. It doesn't do anything at all, but shows the bare minimum
that must be written to get a driver to run.

### indi_mycustomdriver.h

```cpp
#pragma once

#include <libindi/defaultdevice.h>

class MyCustomDriver : public INDI::DefaultDevice
{
public:
    MyCustomDriver();
    virtual ~MyCustomDriver() = default;

    // You must override this method in your class.
    virtual const char *getDefaultName() override;
};
```

```cpp
#include "config.h"
#include "indi_mycustomdriver.h"

// We declare an auto pointer to MyCustomDriver.
static std::unique_ptr<MyCustomDriver> mydriver(new MyCustomDriver());

// libindidriver will try to link to these functions, so they MUST exist.
// Here we pass off handling of them to our driver's class.

void ISGetProperties(const char *dev)
{
    mydriver->ISGetProperties(dev);
}

void ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n)
{
    mydriver->ISNewSwitch(dev, name, states, names, n);
}

void ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n)
{
    mydriver->ISNewText(dev, name, texts, names, n);
}

void ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
{
    mydriver->ISNewNumber(dev, name, values, names, n);
}

void ISNewBLOB(const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[],
               char *formats[], char *names[], int n)
{
    mydriver->ISNewBLOB(dev, name, sizes, blobsizes, blobs, formats, names, n);
}

void ISSnoopDevice(XMLEle *root)
{
    mydriver->ISSnoopDevice(root);
}

MyCustomDriver::MyCustomDriver()
{
    setVersion(CDRIVER_VERSION_MAJOR, CDRIVER_VERSION_MINOR);
}

const char *MyCustomDriver::getDefaultName()
{
    return "My Custom Driver";
}
```

## Compiling

First create a `build` folder in your project's folder, move into it and configure the project with cmake.

```bash
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ../
```

If you look in the build folder, you should now see the `config.h` and `indi_mycustomdriver.xml` files
with the version info set properly.

Now we can build our executable.

```bash
make
```

And install it.

```bash
sudo make install
```

If you want to see your driver in KStars, be sure to restart it after installing your driver (only needed once).

So let's add it to a profile and start our driver.

![Ekos Profile Editor](01-profile_editor.png)

When we do, we can see it in the INDI Control Panel, with the `Main Control`, `General Info`, and `Options` tabs available.

![INDI Control Panel](01-driver.png)

## Moving On

Now we have a driver running, but it doesn't do anything. So let's move on to [properties](02-properties.md).
