# Introduction #
Similar to assert(stmt) in libc, we provide light weight ASSERT for libfetion-gui application. Developers are suggested to use these Macros instead of
  * pass-in parameter check
  * assert (unless you know what you do)
```
#define FX_RETURN_IF_FAILED(x) do{ if (!x) return; }while(0);
#define FX_RETURN_WITH_VALUE_IF_FAILED(x, v) do{ if (!x) return v; }while(0);
```