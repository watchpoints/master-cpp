
写了愿望清单10年了为什么没执行？

（1） 整个公司99%人都没有使用的，甚至反对，自己根本看不到好的一方面
（2） 你选择 Vim or Emacs or neovim 那个好 犹豫等待10年，根本没开始 ，没有合适资料
（3） 根本没有时间

反驳：
1. 笔记本 4g ,8g，16g内 打开浏览器 ide 彻底根本不能用了
 宁愿忍受10年也不去解决这个问题
2. yout经典视频很多，希望高手数显教你，自己不看去。
3. 什么时候开始选择，什么时候离开都是需求的。你根本没有开始
等


# day1

VIM哲学

首先来说一下vim的哲学，vim这种操作方式为何一直存在，并且主流ide也均有vim的键位插件？

因为vim的目标就是让你更加专注于你的代码书写上，避免被外部的因素而扰乱，其中最大的因素就是来自鼠标移动的这一动作，如果你经常敲代码，会有明显的感觉，就是写代码时要时不时从键盘移到鼠标 ，再从鼠标移动到键盘，这个动作其实是比较占用时间的，尤其是当鼠标的移动空间不够大时，你就不得不要把鼠标频繁拿起放下来达到移动鼠标的效果。

vim旨在让用户（主流为程序员，并且多为运维、后端方向居多）避免频繁需要操纵鼠标，只需要一个键盘就可以轻松操纵工作区（成为一代键帝hhh）
~~~
NeoVim 和 Vim 的差异
NeoVim 只有终端版本. 没有 GUI 版本，但是Vim 有 GUI版本 （Gvim）.
NeoVim 目前的剪贴板功能（寄存器） 和 原生Vim实现不一 .
NeoVim 配置文件入口和 Vim 不同， 可以通过 : version 查看.
NeoVim 目前对外部语言的支持并不友好，目前他只对python支持比较完善，要支持Lua和其他外围语言相当麻烦， Vim则支持比较全面
NeoVim 添加了异步函数，当然， Vim 暂时没有，Vim 8.0 也会补上这个功能
NeoVim 解决了 Vim 在终端（Terminal ,Xterm等）下 alt 按键映射不响应的 bug，当然，Vim也有插件的解决方案。
~~~


## 入门 vim

https://github.com/nshen/learn-neovim-lua

只看这个一个文章 
-  http://blog.shiyinhong.cn/2022/05/14/NeoVim%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA/
- https://jimyag.cn/posts/20d50b9d/
- https://zhuanlan.zhihu.com/p/301008305 ，
- https://zhuanlan.zhihu.com/p/577413629
- https://martinlwx.github.io/zh-cn/config-neovim-from-scratch/

- https://www.zhihu.com/tardis/zm/art/434731678?source_id=1005

剩下就是实践实践




https://github.com/neovim/neovim/wiki/Installing-Neovim


CentOS 8 / RHEL 8
Neovim is available through EPEL (Extra Packages for Enterprise Linux)

yum install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-8.noarch.rpm
yum install -y neovim python3-neovim


5、创建初始化 neovim 配置文件。
mkdir ~/.config/nvim/
nvim ~/.config/nvim/init.vim

配置文件：

- 
" 令编辑器显示行号
set nu



## 奇迹提问

1. 青铜提问1 nvim  init.vim init.lua 文件区别？




