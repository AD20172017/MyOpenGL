## 环境配置
- 使用Visual Studio 2022编译,与2025年4月的VS2022的17.13.6版本兼容
- 第三方库都安装在Dependencies文件夹中,主要来源于learning opengl,glew和glfw更新为2017的版本 
- 使用`SDL2`作为窗口管理,使用GLM作为数学库
- vs项目的参数应该已配置完毕,**请使用x86编译**,如有问题可参照[ChernoOpenGL](https://github.com/speauty/ChernoOpenGL) 

## 目录结构
### `Dependencies` : 第三方库
主要可能会用到的第三方库
- learning opengl
- glew
- glfw
- SDL2
- SDL2_image
- stb_image
## 说明
- 对大佬的拙劣模仿吧了,希望有一天能够将这些融汇贯通,写出属于自己的代码
- 使用ACQUIRE_FUNC的类无法使用RAII的特性,Object及其子类的创建都是在Object的管理下,使用宏定义写死了创建的逻辑
- 至于对于想要使用RAII来保证安全的类,即对资源复用性要求不高的类,可以使用`std::shared_ptr`,或者不使用ACQUIRE_FUNC
- 使用模板定义一个RAII_ACQUIRE_FUNC的宏也不是不行_
## PS
- 看着大佬们各种NB的写法,总是控制不住自己的手
- 原本想包装一下shared_ptr,但是发现包装了之后易读性反而下降了,还是在类内部包装吧
- 2025.4.14:再也不用宏魔法了,一直出问题,写对象池写了一天
- 智能指针只有std::shared_ptr<>才可以自定义删除器, std::make_shared<>不可以