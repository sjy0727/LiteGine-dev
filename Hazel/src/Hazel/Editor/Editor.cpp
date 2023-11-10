#include "Editor.h"
#include "ImGui/L2DFileDialog.h"

namespace Hazel
{
    Editor* Editor::s_Instance = nullptr;

    Editor::Editor()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = this;
        }
    }

    void Editor::ShowFPS()
    {
        static bool show_fps_window = true;
        static bool no_titlebar     = true;
        static bool no_resize       = true;

        ImGuiWindowFlags window_flags = 0;
        if (no_titlebar)
            window_flags |= ImGuiWindowFlags_NoTitleBar;
        if (no_resize)
            window_flags |= ImGuiWindowFlags_NoResize;

        // 窗口自适应大小
        window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

        if (!ImGui::Begin("FPS", &show_fps_window, window_flags))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }

        ImGui::Text("CPU: APPLE M1");
        ImGui::Text("GPU: APPLE M1");
        ImGui::Separator();
        // 显示分辨率
        ImGui::Text("Resolution: 1280 x 780");
        ImGui::Separator();

        // 显示帧率
        ImGui::Text(
            "Average %-6.3f ms/frame (%-5.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::End();
    }

    void Editor::ShowFPSOverLay(bool& open)
    {
        // static bool open = true;
        static int location = 1;

        ImGuiIO&         io           = ImGui::GetIO();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                                        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        if (location >= 0)
        {
            const float          PAD       = 10.0f;
            const ImGuiViewport* viewport  = ImGui::GetMainViewport();
            ImVec2               work_pos  = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2               work_size = viewport->WorkSize;
            ImVec2               window_pos, window_pos_pivot;
            window_pos.x       = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y       = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowViewport(viewport->ID);
            window_flags |= ImGuiWindowFlags_NoMove;
        }
        else if (location == -2)
        {
            // Center window
            ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            window_flags |= ImGuiWindowFlags_NoMove;
        }

        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

        if (ImGui::Begin("Example: Simple overlay", &open, window_flags))
        {
            ImGui::Text("CPU: APPLE M1");
            ImGui::Text("GPU: APPLE M1");
            ImGui::Separator();
            // 显示分辨率
            ImGui::Text("Resolution: 1280 x 780");
            ImGui::Separator();

            // 显示帧率
            ImGui::Text(
                "Average %-6.3f ms/frame (%-5.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::Separator();
            ImGui::Text("Simple overlay\n"
                        "(right-click to change position)");
            ImGui::Separator();
            if (ImGui::IsMousePosValid())
                ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
            else
                ImGui::Text("Mouse Position: <invalid>");
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Custom", nullptr, location == -1))
                    location = -1;
                if (ImGui::MenuItem("Center", nullptr, location == -2))
                    location = -2;
                if (ImGui::MenuItem("Top-left", nullptr, location == 0))
                    location = 0;
                if (ImGui::MenuItem("Top-right", nullptr, location == 1))
                    location = 1;
                if (ImGui::MenuItem("Bottom-left", nullptr, location == 2))
                    location = 2;
                if (ImGui::MenuItem("Bottom-right", nullptr, location == 3))
                    location = 3;
                if (open && ImGui::MenuItem("Close"))
                    open = false;
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    static ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

    static int   lastUsing = 0;
    static float cameraProjection[16];

    static float cameraView[16] = {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};

    // Camera projection
    static bool  isPerspective = true;
    static float fov           = 45.f;
    static float viewWidth     = 10.f; // for orthographic
    static float camYAngle     = 165.f / 180.f * 3.14159f;
    static float camXAngle     = 32.f / 180.f * 3.14159f;

    static bool firstFrame = true;

    static bool  useWindow   = false;
    static int   gizmoCount  = 1;
    static float camDistance = 6.f;

    static float objectMatrix[4][16] = {
        {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f},

        {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 2.f, 0.f, 0.f, 1.f},

        {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 2.f, 0.f, 2.f, 1.f},

        {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 2.f, 1.f}};

    static float identityMatrix[16] = {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};

    void Editor::ShowImGuizmo()
    {
        // BUG
        ImGuiIO& io = ImGui::GetIO();
        if (isPerspective)
        {
            Perspective(fov, io.DisplaySize.x / io.DisplaySize.y, 0.1f, 100.f, cameraProjection);
        }
        else
        {
            float viewHeight = viewWidth * io.DisplaySize.y / io.DisplaySize.x;
            OrthoGraphic(-viewWidth, viewWidth, -viewHeight, viewHeight, 1000.f, -1000.f, cameraProjection);
        }
        ImGuizmo::SetOrthographic(!isPerspective);
        ImGuizmo::BeginFrame();

        //        ImGui::SetNextWindowPos(ImVec2(1024, 100), ImGuiCond_Appearing);
        //        ImGui::SetNextWindowSize(ImVec2(256, 256), ImGuiCond_Appearing);

        // create a window and insert the inspector
        //        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Appearing);
        //        ImGui::SetNextWindowSize(ImVec2(320, 340), ImGuiCond_Appearing);
        ImGui::Begin("Editor");
        if (ImGui::RadioButton("Full view", !useWindow))
            useWindow = false;
        ImGui::SameLine();
        if (ImGui::RadioButton("Window", useWindow))
            useWindow = true;

        ImGui::Text("Camera");
        bool viewDirty = false;
        if (ImGui::RadioButton("Perspective", isPerspective))
            isPerspective = true;
        ImGui::SameLine();
        if (ImGui::RadioButton("Orthographic", !isPerspective))
            isPerspective = false;
        if (isPerspective)
        {
            ImGui::SliderFloat("Fov", &fov, 20.f, 110.f);
        }
        else
        {
            ImGui::SliderFloat("Ortho width", &viewWidth, 1, 20);
        }
        viewDirty |= ImGui::SliderFloat("Distance", &camDistance, 1.f, 10.f);
        ImGui::SliderInt("Gizmo count", &gizmoCount, 1, 4);

        if (viewDirty || firstFrame)
        {
            float eye[] = {cosf(camYAngle) * cosf(camXAngle) * camDistance,
                           sinf(camXAngle) * camDistance,
                           sinf(camYAngle) * cosf(camXAngle) * camDistance};
            float at[]  = {0.f, 0.f, 0.f};
            float up[]  = {0.f, 1.f, 0.f};
            LookAt(eye, at, up, cameraView);
            firstFrame = false;
        }

        ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
        if (ImGuizmo::IsUsing())
        {
            ImGui::Text("Using gizmo");
        }
        else
        {
            ImGui::Text(ImGuizmo::IsOver() ? "Over gizmo" : "");
            ImGui::SameLine();
            ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");
            ImGui::SameLine();
            ImGui::Text(ImGuizmo::IsOver(ImGuizmo::ROTATE) ? "Over rotate gizmo" : "");
            ImGui::SameLine();
            ImGui::Text(ImGuizmo::IsOver(ImGuizmo::SCALE) ? "Over scale gizmo" : "");
        }
        ImGui::Separator();
        for (int matId = 0; matId < gizmoCount; matId++)
        {
            ImGuizmo::SetID(matId);

            EditTransform(cameraView, cameraProjection, objectMatrix[matId], lastUsing == matId);
            if (ImGuizmo::IsUsing())
            {
                lastUsing = matId;
            }
        }

        ImGui::End();
    }

    void
    Editor::EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
    {
        static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
        static bool           useSnap         = false;
        static float          snap[3]         = {1.f, 1.f, 1.f};
        static float          bounds[]        = {-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f};
        static float          boundsSnap[]    = {0.1f, 0.1f, 0.1f};
        static bool           boundSizing     = false;
        static bool           boundSizingSnap = false;

        if (editTransformDecomposition)
        {
            if (ImGui::IsKeyDown(ImGuiKey_T))
                mCurrentGizmoOperation = ImGuizmo::TRANSLATE;

            if (ImGui::IsKeyDown(ImGuiKey_E))
                mCurrentGizmoOperation = ImGuizmo::ROTATE;

            if (ImGui::IsKeyDown(ImGuiKey_R)) // r Key
                mCurrentGizmoOperation = ImGuizmo::SCALE;

            if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
                mCurrentGizmoOperation = ImGuizmo::TRANSLATE;

            ImGui::SameLine();

            if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
                mCurrentGizmoOperation = ImGuizmo::ROTATE;

            ImGui::SameLine();

            if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
                mCurrentGizmoOperation = ImGuizmo::SCALE;

            if (ImGui::RadioButton("Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
                mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;

            float matrixTranslation[3], matrixRotation[3], matrixScale[3];
            ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);

            ImGui::InputFloat3("Tr", matrixTranslation);
            ImGui::InputFloat3("Rt", matrixRotation);
            ImGui::InputFloat3("Sc", matrixScale);

            ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

            if (mCurrentGizmoOperation != ImGuizmo::SCALE)
            {
                if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
                    mCurrentGizmoMode = ImGuizmo::LOCAL;

                ImGui::SameLine();

                if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
                    mCurrentGizmoMode = ImGuizmo::WORLD;
            }

            if (ImGui::IsKeyDown(ImGuiKey_S))
                useSnap = !useSnap;

            ImGui::Checkbox("##UseSnap", &useSnap);
            ImGui::SameLine();

            switch (mCurrentGizmoOperation)
            {
                case ImGuizmo::TRANSLATE:
                    ImGui::InputFloat3("Snap", &snap[0]);
                    break;
                case ImGuizmo::ROTATE:
                    ImGui::InputFloat("Angle Snap", &snap[0]);
                    break;
                case ImGuizmo::SCALE:
                    ImGui::InputFloat("Scale Snap", &snap[0]);
                    break;
            }

            ImGui::Checkbox("Bound Sizing", &boundSizing);

            if (boundSizing)
            {
                ImGui::PushID(3);
                ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
                ImGui::SameLine();
                ImGui::InputFloat3("Snap", boundsSnap);
                ImGui::PopID();
            }
        }

        ImGuiIO&                io                  = ImGui::GetIO();
        float                   viewManipulateRight = io.DisplaySize.x;
        float                   viewManipulateTop   = 0;
        static ImGuiWindowFlags gizmoWindowFlags    = 0;

        if (useWindow)
        {
            //            ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Appearing);
            //            ImGui::SetNextWindowPos(ImVec2(400, 20), ImGuiCond_Appearing);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
            ImGui::Begin("Gizmo", nullptr, gizmoWindowFlags);
            ImGuizmo::SetDrawlist();
            float windowWidth  = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
            viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
            viewManipulateTop   = ImGui::GetWindowPos().y;
            ImGuiWindow* window = ImGui::GetCurrentWindow();
            gizmoWindowFlags =
                ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ?
                    ImGuiWindowFlags_NoMove :
                    0;
        }
        else
        {
            auto window = glfwGetCurrentContext();
            int  x, y;
            glfwGetWindowPos(window, &x, &y);
            ImGuizmo::SetRect((float)x, (float)y, io.DisplaySize.x, io.DisplaySize.y);
            //            float windowWidth  = (float)ImGui::GetWindowWidth();
            //            float windowHeight = (float)ImGui::GetWindowHeight();
            //            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth,
            //            windowHeight);
        }

        ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
        ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount);
        ImGuizmo::Manipulate(cameraView,
                             cameraProjection,
                             mCurrentGizmoOperation,
                             mCurrentGizmoMode,
                             matrix,
                             nullptr,
                             useSnap ? &snap[0] : nullptr,
                             boundSizing ? bounds : nullptr,
                             boundSizingSnap ? boundsSnap : nullptr);

        if (useWindow)
        {
            ImGuizmo::ViewManipulate(cameraView,
                                     camDistance,
                                     ImVec2(viewManipulateRight - 196, viewManipulateTop),
                                     ImVec2(196, 196),
                                     0x10101010);
        }
        else
        {
            auto window = glfwGetCurrentContext();
            int  x, y;
            glfwGetWindowPos(window, &x, &y);
            ImGuizmo::ViewManipulate(cameraView,
                                     camDistance,
                                     ImVec2((float)x + io.DisplaySize.x - 196, (float)y),
                                     ImVec2(196, 196),
                                     0x10101010);
        }

        if (useWindow)
        {
            ImGui::End();
            ImGui::PopStyleColor(1);
        }
    }

    void Editor::Perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float* m16)
    {
        float ymax, xmax;
        ymax = znear * tanf(fovyInDegrees * 3.141592f / 180.0f);
        xmax = ymax * aspectRatio;
        Frustum(-xmax, xmax, -ymax, ymax, znear, zfar, m16);
    }

    void Editor::OrthoGraphic(const float l, float r, float b, const float t, float zn, const float zf, float* m16)
    {
        m16[0]  = 2 / (r - l);
        m16[1]  = 0.0f;
        m16[2]  = 0.0f;
        m16[3]  = 0.0f;
        m16[4]  = 0.0f;
        m16[5]  = 2 / (t - b);
        m16[6]  = 0.0f;
        m16[7]  = 0.0f;
        m16[8]  = 0.0f;
        m16[9]  = 0.0f;
        m16[10] = 1.0f / (zf - zn);
        m16[11] = 0.0f;
        m16[12] = (l + r) / (l - r);
        m16[13] = (t + b) / (b - t);
        m16[14] = zn / (zn - zf);
        m16[15] = 1.0f;
    }

    void Editor::LookAt(const float* eye, const float* at, const float* up, float* m16)
    {
        float X[3], Y[3], Z[3], tmp[3];

        tmp[0] = eye[0] - at[0];
        tmp[1] = eye[1] - at[1];
        tmp[2] = eye[2] - at[2];
        Normalize(tmp, Z);
        Normalize(up, Y);

        Cross(Y, Z, tmp);
        Normalize(tmp, X);

        Cross(Z, X, tmp);
        Normalize(tmp, Y);

        m16[0]  = X[0];
        m16[1]  = Y[0];
        m16[2]  = Z[0];
        m16[3]  = 0.0f;
        m16[4]  = X[1];
        m16[5]  = Y[1];
        m16[6]  = Z[1];
        m16[7]  = 0.0f;
        m16[8]  = X[2];
        m16[9]  = Y[2];
        m16[10] = Z[2];
        m16[11] = 0.0f;
        m16[12] = -Dot(X, eye);
        m16[13] = -Dot(Y, eye);
        m16[14] = -Dot(Z, eye);
        m16[15] = 1.0f;
    }

    void Editor::Frustum(float left, float right, float bottom, float top, float znear, float zfar, float* m16)
    {
        float temp, temp2, temp3, temp4;
        temp    = 2.0f * znear;
        temp2   = right - left;
        temp3   = top - bottom;
        temp4   = zfar - znear;
        m16[0]  = temp / temp2;
        m16[1]  = 0.0;
        m16[2]  = 0.0;
        m16[3]  = 0.0;
        m16[4]  = 0.0;
        m16[5]  = temp / temp3;
        m16[6]  = 0.0;
        m16[7]  = 0.0;
        m16[8]  = (right + left) / temp2;
        m16[9]  = (top + bottom) / temp3;
        m16[10] = (-zfar - znear) / temp4;
        m16[11] = -1.0f;
        m16[12] = 0.0;
        m16[13] = 0.0;
        m16[14] = (-temp * zfar) / temp4;
        m16[15] = 0.0;
    }

    void Editor::Cross(const float* a, const float* b, float* r)
    {
        r[0] = a[1] * b[2] - a[2] * b[1];
        r[1] = a[2] * b[0] - a[0] * b[2];
        r[2] = a[0] * b[1] - a[1] * b[0];
    }

    float Editor::Dot(const float* a, const float* b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }

    void Editor::Normalize(const float* a, float* r)
    {
        float il = 1.f / (sqrtf(Dot(a, a)) + FLT_EPSILON);
        r[0]     = a[0] * il;
        r[1]     = a[1] * il;
        r[2]     = a[2] * il;
    }

    char* Editor::ShowFileDialog()
    {
        if (ImGui::Begin("File dialog"))
        {
            ImGui::TextUnformatted("Path: ");
            ImGui::InputText("##path", path, sizeof(path));
            ImGui::SameLine();
            if (ImGui::Button("Browse##path"))
            {
                file_dialog_buffer           = path;
                FileDialog::file_dialog_open = true;
                //            FileDialog::file_dialog_open_type = FileDialog::FileDialogType::SelectFolder;
                FileDialog::file_dialog_open_type = FileDialog::FileDialogType::OpenFile;
            }

            if (FileDialog::file_dialog_open)
            {
                FileDialog::ShowFileDialog(&FileDialog::file_dialog_open,
                                           file_dialog_buffer,
                                           sizeof(file_dialog_buffer),
                                           FileDialog::file_dialog_open_type);
            }
            ImGui::End();
        }
        return file_dialog_buffer;
    }
} // namespace Hazel