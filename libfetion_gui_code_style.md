# Introduction #

libfetion-gui 代码规范


# Details #
1:  by libfetion, Feb 07, 2009

整理规范 libfetion-gui 代码

目前libfetion-gui 代码很多地方不是很规范，:
在下部分中，我们即将实现：
1： 整理代码模块
2： 函数命名统一规则


2: by iptton, Apr 16, 2009

警告方面：
建议尽量使用宏
> Q\_UNUSED(variablename)

from issues 41:
1，xxxEvent 函数的重写
做皮肤里处理了一个问题，背景在resize后没有自动扩展
发现问题出现在类里的这个函数里 void resizeEvent(...
这个函数没有调用 Parent::resizeEvent(
建议以后重写这类函数(xxxEvent)时都把父类的调用一下。
2，对于Flags类型变量的设置
> 除非确实需要重置所有flag，否则都用 与 或 非 这些操作符，例如：
> > 设置无边框flag
> > setWindowFlags(  windowFlags() | Qt::FramelessWindowHint );
> > 清除无边框flag
> > setWindowFlags(  windowFlags() & (!Qt::FramelessWindowHint) );
> > 取反无边框flag
> > setWindowFlags(  windowFlags() ^ Qt::FramelessWindowHint );
> > 而不是这样
> > setWindowFlags( Qt::FramelessWindowHint );

3: by iptton, Apr 17, 2009

关于QObject::deleteLater() 函数的使用

> QT是有事件循环的，在某对象的event handler里delete一个对象
> QObject会提示：
> > QObject : do not delete object, 'MsgEdit', during its event handler!

> 这样做的后果可能现在还没看到，项目里的fxmsgwindow.cpp  closeTabWin (162行)里就有
这样的代码：
> > if (accountTab)
> > > delete accountTab;

> 建议改成：
> > if (accountTab)
> > > accountTab->deleteLater();

PS:建议以后添加的代码命名用全名,如上面的 closeTabWin 用 closeTabWindow更好点，因为随时
可能忘记当时用的是全名还是缩写

4: by libfetion, Apr 17, 2009


> if (accountTab)
> > delete accountTab;
> > 建议改成：
> > > if (accountTab)
> > > > accountTab->deleteLater();

delete accountTab;
这个是C语言的风格，而且有一定的风险。

如果使用QObject来作为对象的基类的话（里面有对象引用计数），
在以后编程中是非常有好处的。这样可以避免在传递对象后使用一些已经被删除了的对象而导致
程序错误。

5:  by iptton, May 07, 2009

一点命名规则的建议：

文件名全小写

不缩写

类名格式: ClassNameWithOutUnderLine

成员名：
　　	publicMenber
　　	_privateMember
　　_protectedMember
　　	在可以见文知义的情况下可以直接使用其它作用来命名，在有可能引起歧义时加前缀表示类型。

> 特殊情况：qt 的connectSlotByName所用的函数。
尽可能使用QT系统提供的功能
> 如:
> > Q\_UNUSED(variableName);
> > Q\_ASSERT(condisionExpress);
> > Q\_ASSERT\_X( condisionExpress, "where", "what");
> > qobject\_cast<newType**>(poiter);  //不使用需要支持rtti编译器才能用的dynamic\_cast()**

优化编译依赖，头文件与实现文件分开。不使用QtGui这个大头文件，只使用所应用到的

6: by echo.xjtu, Jun 06, 2009

建议Libfetion协议库函数命名全部小写字母+下划线
GUI代码参照QT类成员函数命名规则：小写动词开始，大写字母分段

7: by alsor.zhou, Aug 18, 2009

有个问题～到目前为止([r315](https://code.google.com/p/libfetion-gui/source/detail?r=315)), 全局函数使用过多，虽然方便了调用,但不容易管理..



