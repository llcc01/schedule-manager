# 数据结构课程设计之学生日程管理系统

## 在线文档

<https://docs.qq.com/desktop/mydoc/folder/IDfMGYKJgDWC?u=9253feff13ee413bb3973259b2f8cc84>

## 后端

### 配置本地 c++ 环境参考 CEF

本工程基于官方CEF例程修改，初次配置需要使用CMake生成工程
<https://zhuanlan.zhihu.com/p/344306287>

## 前端

### 目录

- `schmgr/` 项目源码
  - `cefclient/` CEF客户端
  - `core/` 核心源码
    - `back/` 后端支持
      - `api/`
        - `alarm` 闹钟API
        - `clock` 系统时钟API
        - `course` 课程API
        - `nav` 导航API
        - `sch` 日程API
        - `user` 用户API
      - `database/` 迷你数据库
        - `db` 数据库核心
        - `model` 数据模型
        - `table` 表格模型
      - `nav/` 导航相关
      - `timer/` 时钟相关
        - `clock` 系统时钟模块
        - `timer` 定时器模块
      - `api_back` API总处理
      - `logger` 日志模块
      - `res` 浏览器静态资源服务模块
    - `mtl/` Mini Template Library 迷你模板库，提供数据结构支持
      - `HashTable`
      - `List`
      - `Map`
      - `mti`
      - `multiMap`
      - `RBTree`
      - `unorder_Map`
      - `Vector`
    - `api` 浏览器JS环境API绑定
    - `core` 浏览器代理模块
    - `utils` 通用小工具
  - `shared` CEF客户端共享
  - `utils` 通用小工具
    - `md5`
    - `string`
- `Resources/` 资源文件

### 应用目录说明

- `db\` 数据库文件
- `static\` 前端静态文件
- `console.log` 浏览器控制台日志
- `debug.log` 调试日志
- `syslog.log` 后端日志
