## 环境配置
- 使用Visual Studio 2022编译,与2025年4月的VS2022的17.13.6版本兼容
- 第三方库都安装在Dependencies文件夹中,主要来源于learning opengl,glew和glfw更新为2017的版本 
- 使用`SDL2`作为窗口管理,使用GLM作为数学库
- vs项目的参数应该已配置完毕,**请使用x86编译**,如有问题可参照[ChernoOpenGL](https://github.com/speauty/ChernoOpenGL) 

## 目录结构
- `Dependencies` : 第三方库

## PS
- 看着大佬们各种NB的写法,总是控制不住自己的手
- 原本想包装一下shared_ptr,但是发现包装了之后易读性反而下降了,还是在类内部包装吧
- 2025.4.14:再也不用宏魔法了,一直出问题,写对象池写了一天