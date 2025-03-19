Package: cpp-jsonlib:x64-windows@1.0.0

**Host Environment**

- Host: x64-windows
- Compiler: MSVC 19.29.30158.0
-    vcpkg-tool version: 2025-02-11-bec4296bf5289dc9ce83b4f5095943e44162f9c2
    vcpkg-scripts version: 0413db10ad 2025-02-18 (4 weeks ago)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
-- Note: cpp-jsonlib only supports static library linkage. Building static library.
Downloading https://github.com/Mysvac/cpp-jsonlib/archive/1.0.0.tar.gz -> Mysvac-cpp-jsonlib-1.0.0.tar.gz
Successfully downloaded Mysvac-cpp-jsonlib-1.0.0.tar.gz
error: failing download because the expected SHA512 was all zeros, please change the expected SHA512 to: 873a48cfe585c9d914f3e5f134113220d3dfc836ce7a5cad7b208c3c12059f4d5ed0fc661284ebd0ebdf00890a13422bc407a1b0bc87d2b50f0d3ea08d0584ad
CMake Error at scripts/cmake/vcpkg_download_distfile.cmake:124 (message):
  Download failed, halting portfile.
Call Stack (most recent call first):
  scripts/cmake/vcpkg_from_github.cmake:106 (vcpkg_download_distfile)
  E:/myCodes/others/JsonNativeTest/custom-overlay/cpp-jsonlib/portfile.cmake:3 (vcpkg_from_github)
  scripts/ports.cmake:196 (include)



```

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "dependencies": [
    "cpp-jsonlib"
  ]
}

```
</details>
