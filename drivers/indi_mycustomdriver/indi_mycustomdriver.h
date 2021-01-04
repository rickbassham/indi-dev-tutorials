#pragma once

#include <libindi/defaultdevice.h>

namespace Connection
{
    class Serial;
}

class MyCustomDriver : public INDI::DefaultDevice
{
public:
    MyCustomDriver();
    virtual ~MyCustomDriver() = default;

    virtual const char *getDefaultName() override;

    virtual bool initProperties() override;
    virtual bool updateProperties() override;

    virtual void ISGetProperties(const char *dev);
    virtual bool ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[],
                             int n) override;
    virtual bool ISNewText(const char *dev, const char *name, char *texts[], char *names[],
                           int n) override;

    virtual void TimerHit() override;

protected:
    virtual bool saveConfigItems(FILE *fp) override;

private:
    ISwitch SayHelloS[1];
    ISwitchVectorProperty SayHelloSP;

    IText WhatToSayT[1];
    ITextVectorProperty WhatToSayTP;

private: // serial connection
    bool Handshake();
    bool sendCommand(const char *cmd);
    int PortFD{-1};

    Connection::Serial *serialConnection{nullptr};
};
