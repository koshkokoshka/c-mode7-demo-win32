#include <Windows.h>
#include <math.h>

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

const static BITMAPINFO bmi = {
    .bmiHeader = {
        .biSize = sizeof(BITMAPINFOHEADER),
        .biWidth = SCREEN_WIDTH,
        .biHeight = SCREEN_HEIGHT,
        .biPlanes = 1,
        .biBitCount = 32,
        .biCompression = BI_RGB,
        .biSizeImage = 0,
        .biXPelsPerMeter = 0,
        .biYPelsPerMeter = 0,
        .biClrUsed = 0,
        .biClrImportant = 0
    },
    .bmiColors = {
        0
    }
};

void WinMainCRTStartup() {

    // Create "EDIT" control and get its DC context
    HDC hdc = GetDC(
        CreateWindow(
            "edit",
            0,
            WS_POPUP | WS_VISIBLE,
            0,
            0,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            NULL,
            NULL,
            NULL,
            NULL
        )
    );

    // Message loop
    static MSG message;
    while (1) {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            if (GetAsyncKeyState(VK_ESCAPE)) {
                return; // lister for ESC key press for quit
            }
            DispatchMessage(&message);
        }

        static int time;
        time += 1;

        static BYTE pixels[SCREEN_WIDTH * SCREEN_HEIGHT * 4];

        for (int y = 0; y < SCREEN_HEIGHT; ++y) {
            double z = -0.5 + ((double)y / SCREEN_HEIGHT);

            double v = (double)y;
            v /= z;
            if (z < 0) {
                v -= time;
            } else {
                v += time;
            }

            for (int x = 0; x < SCREEN_WIDTH; ++x) {

                double u = -SCREEN_WIDTH/2.0 + (double)x;
                u /= z;

                BYTE color = ((BYTE)u) ^ ((BYTE)v);
                if (z < 0) {
                    z = -z;
                }
                pixels[((y * SCREEN_WIDTH) + x) * 4 + 1] = (BYTE)((double)color * z);
            }
        }

        StretchDIBits(
            hdc,
            0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
            0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
            &pixels,
            &bmi,
            DIB_RGB_COLORS,
            SRCCOPY
        );
    }
}
