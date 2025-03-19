
## 1. 安装cpp-jsonlib库
默认已经创建vcpkg.json，下面两行不需要。
```shell
# vcpkg new --application
# vcpkg add port cpp-jsonlib
```

请执行下面的命令，安装cpp-jsonlib库。
```shell
vcpkg install --overlay-ports=./custom-overlay/cpp-jsonlib
```
