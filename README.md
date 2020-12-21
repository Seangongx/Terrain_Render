# Terrain_Render
基于OpenGL地形渲染小项目

配置方法：
配置属性->常规
	平台：活动（win32）
	Windows SDK版本：10.0.17134.0
	平台工具集Visual Studio 2017 (v141)
	字符集：使用多字节字符集
配置属性->VC++目录
	包含目录：...\TerrainRender\libinclude\include
	库目录：...\TerrainRender\libinclude\lib
配置属性->C/C++->优化
	//
配置属性->C/C++->语言->符合模式
	//是
配置属性->C/C++->语言->预编译头
	不使用预编译头
配置属性->C/C++->语言->所有选项->SDL检查
	//
配置属性->链接器->输入->附加依赖项
	glfw3.lib, assimpd.lib opengl32.lib legacy_stdio_definitions.lib
配置属性->链接器->命令行->其他选项
	/FORCE:MULTIPLE 
	


