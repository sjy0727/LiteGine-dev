# LiteGine 进展
1. BUG
- ImGui无法完成事件，而GLFW可以完成事件。
- 按照imgui官方教程example_glfw_opengl3中一样写 就可以解决高dpi不准的问题，但是又会存在不能同时处理事件,键盘按三个键鼠标同时移动会报错。
```shell
Assertion failed: ((io.KeysDown[n] == false || IsKeyDown((ImGuiKey)n)) && "Backend needs to either only use io.AddKeyEvent(), either only fill legacy io.KeysDown[] + io.KeyMap[]. Not both!"), function UpdateKeyboardInputs, file imgui.cpp, line 9163.
```

- 使用io.KeyMap 会报错 

```shell
Assertion failed: (KeyMap[n] == -1 && "Backend needs to either only use io.AddKeyEvent(), either only fill legacy io.KeysDown[] + io.KeyMap[]. Not both!"), function AddKeyAnalogEvent, file imgui.cpp, line 1498.
```