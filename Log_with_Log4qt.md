# Introduction #
Warning: log4qt debug support was disabled in PR build. As a developer, you need append 'debug' profile in libfetion.pro
```
# development configure
CONFIG += debug
```
To use the logger function, you have to include `fxdebug.h` or `log4qt/logger.h` in your class header.
```
#include "fxdebug.h"
```
Then, add LOG4QT\_DECLARE\_QCLASS\_LOGGER in your class declaration
```
FxTestClass:public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    ...
}
```
**Note, only classes derived from QObject were supported with Log4qt logger**

Libfetion-gui uses Log4qt::Logger::rootLogger for debug information logging. Access rootLogger with
```
#define FX_LOGGER   logger()
```
Five categories log level DEBUG, INFO, WARN, ERROR and FATAL were supported in our application. You can use different log level with our built-in macros.
```
#define FX_DEBUG(x) do{logger()->debug(x);}while(0);
#define FX_INFO(x) do{logger()->info(x);}while(0);
#define FX_WARN(x) do{logger()->warn(x);}while(0);
#define FX_ERROR(x) do{logger()->error(x);}while(0);
#define FX_FATAL(x) do{logger()->fatal(x);}while(0);
```
We suggested newly created functions have FX\_FUNCTION inside body
```
#define FX_FUNCTION FX_DEBUG(__PRETTY_FUNCTION__)
```
```
FxMain::FxMain()
{
    FX_FUNCTION
    ...
}
```