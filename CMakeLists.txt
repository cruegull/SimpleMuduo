# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.0)

# 项目信息
project (server)
# 项目目录
aux_source_directory(./src LIST1)
# 指定生成目标
add_executable(server ./test/server.cpp ${LIST1})

# 项目信息
project (client)
# 项目目录
aux_source_directory(./src LIST2)
# 指定生成目标
add_executable(client ./test/client.cpp ${LIST2})