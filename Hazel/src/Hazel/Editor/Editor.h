#pragma once
#include "ImGui/ImGuizmo.h"
#include "ImGui/TextEditor.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "hzpch.h"

#include "GLFW/glfw3.h"

namespace Hazel
{
#define IM_CLAMP(V, MN, MX) ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))
    
    class Editor
    {
    public:
        Editor();
        ~Editor() = default;
        void                  ShowFPS();
        void                  ShowFPSOverLay(bool& open);
        inline static Editor* Get() { return new Editor(); }
        void                  ShowImGuizmo();
        char*                 ShowFileDialog();

        // ImGuizmo 相关函数
        void  EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition);
        void  Perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float* m16);
        void  OrthoGraphic(const float l, float r, float b, const float t, float zn, const float zf, float* m16);
        void  LookAt(const float* eye, const float* at, const float* up, float* m16);
        void  Frustum(float left, float right, float bottom, float top, float znear, float zfar, float* m16);
        void  Cross(const float* a, const float* b, float* r);
        float Dot(const float* a, const float* b);
        void  Normalize(const float* a, float* r);

    private:
        static Editor* s_Instance;

        // FileDialog相关函数
        inline static char* file_dialog_buffer = nullptr;
        inline static char  path[500]          = "";
    };
} // namespace Hazel