已知BUG：用户输入 &) 时显示的是 &amp和;)对应的笑脸。

可直接测试。
另：FxInputFace做了点修改。用了setFocusProxy函数，以及用了textCursor->insert来添加表情
我的fxInputFace.cpp 版本可能不对，不要直接覆盖。

FxEditor的使用见main.cpp。
在 ui 文件里提升输入框为fxgui::FxEditor即可 
还需要一次调用 setImagePath()
(注意，为了测试我的代码里没有添加 namespace fxgui，为一致性，最好添加了再提交到SVN上)