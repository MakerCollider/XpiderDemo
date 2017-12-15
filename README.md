# XpiderDemo

This is a Xpider app demo built by QT5.
It shows how to control Xpider and you can also use this demo to control your Xpider on your PC.

## Class Description

### XpiderInfo
This class maintaines the data received from xpider, or will be sent to the xpider.

**XpiderInfo::custom\_data\_** contains the data from xpider registers, they are name, hardware version, uuid, etc.

### XpiderProtocol

This class will be used to encode/decode the data between app and xpider

1. **XpiderProtocol::GetMessage()** can be used to get info from raw data, and the data will be stored into `XpiderProtocol::xpider_info_`.
2. **XpiderProtocol::GetBuffer()** can be used to generate raw data from command, such as move, rotate, etc.

### XpiderComm

This class maintaines the tcp connections between app and xpider, Usually it's on a seperated thread. And most of the function are QT slots.

### XpiderCenter

This class is a demo to show how to control xpider in QT.

1. **XpiderCenter::connectXpider()** This function will emmit a signal to call `XpiderComm::Connect()` slot.
2. **XpiderCenter::NewFrameHandler()** This slot will be called when XpiderComm receive a new frame from xpider. The function will call `XpiderProtocol::GetMessage()` to get sensor info from xpider mainboard.
3. **XpiderCenter::setMove()** This function will make xpider move with given command value(move and rotate value). The function will call `XpiderProtocol::GetBuffer()` to generate raw data, and then send it to xpider through `XpiderComm::SendFrame()` function.