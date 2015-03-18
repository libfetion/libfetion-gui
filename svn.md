# Introduction #

提交代码到svn注意事项


# Details #

在往svn提交代码前，最好是能给其它人review下，这样有助由代码质量的提升。

当然如果上网不方便，或者找不到其它人来review你提交的代码，这样的话如果不提交的话，会影响开发速度。

我们可以：
1：提交你觉得稳定的代码。（稳定的代码可以指功能没有完成，但在以后的开发中将很少会改动该部分代码,”改动该部分代码“描述得也不是很好，大家意会下吧）

2：如果提交过的代码有BUG，我们怎么修复：
我举个例子，
如在 [r78](https://code.google.com/p/libfetion-gui/source/detail?r=78) svn版本号中提交的代码有问题,需要修复

我们可以在提交修复代码时候 往 svn添加如下格式的注释：
fix [r78](https://code.google.com/p/libfetion-gui/source/detail?r=78) + the reason of correct it.

这样我们浏览时就很方便的知道原因了，在svn也不会乱套。