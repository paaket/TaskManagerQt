cmake_minimum_required(VERSION 3.20)

project(TaskManagerQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_AUTOMOC ON)

add_executable(TaskManagerQt main.cpp
    TaskManagerQt.cpp
    LoginWindow.cpp
    RegisterWindow.cpp
    DatabaseManager.cpp
    TaskModel.cpp
    FolderModel.cpp
    TaskDelegator.cpp
    TaskSortProxyModel.cpp
    CreateTaskWindow.cpp
    EditTaskWindow.cpp
    CreateFolderWindow.cpp
    EditFolderWindow.cpp
    EditUserWindow.cpp
    DeleteUserWindow.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Sql Widgets)
find_package(libpqxx CONFIG REQUIRED)

target_link_libraries(TaskManagerQt PRIVATE Qt6::Sql Qt6::Widgets pqxx)