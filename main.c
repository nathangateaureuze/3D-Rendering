#include <windows.h>
#include <stdlib.h>
#include <math.h>

static int quit = 0;


struct imageFrame
{
    int width;
    int height;
    uint32_t *pixels;
};


LRESULT CALLBACK WindowProcessMessage(HWND, UINT, WPARAM, LPARAM);

static struct imageFrame frame = {0};
static BITMAPINFO frame_bitmap_info;
static HBITMAP frame_bitmap = 0;
static HDC frame_device_context = 0;


struct Shape
{
  int (*points)[3];
  int pointslength;

  int (*relations)[2];
  int relationslength;
};


struct Shape shapeconstruct(int *p, int pl, int *r, int rl)
{
    struct Shape *s = malloc(sizeof(struct Shape));

    (*s).pointslength = pl;
    (*s).relationslength = rl;

    (*s).points = malloc(sizeof(int[3]) * (*s).pointslength);
    (*s).relations = malloc(sizeof(int[2]) * (*s).relationslength);

    for(int i = 0; i < (*s).pointslength; i++)
    {   
        (*s).points[i][0] = *(p + 3 * i + 0);
        (*s).points[i][1] = *(p + 3 * i + 1);
        (*s).points[i][2] = *(p + 3 * i + 2);
    }
    for(int i = 0; i < (*s).relationslength; i++)
    {
        (*s).relations[i][0] = *(r + 2 * i + 0);
        (*s).relations[i][1] = *(r + 2 * i + 1);
    }
    
    return *s;
}


void line(struct imageFrame *image, int x1, int y1, int x2, int y2) 
{
    int centerX = (*image).width / 2;
    int centerY = (*image).height / 2;

    x1 += centerX;
    y1 += centerY;
    x2 += centerX;
    y2 += centerY;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int error = dx - dy;
    int error2;

    if (x1 >= 0 && x1 < (*image).width && y1 >= 0 && y1 < (*image).height) 
    {
        (*image).pixels[y1 * (*image).width + x1] = 0x5FA816;
    }

    while (x1 != x2 || y1 != y2) 
    {
        if (x1 >= 0 && x1 < (*image).width && y1 >= 0 && y1 < (*image).height) 
        {
            (*image).pixels[y1 * (*image).width + x1] = 0x5FA816;
        }
        error2 = 2 * error;
        
        if (error2 > -dy) 
        {
            error -= dy;
            x1 += sx;
        }

        if (error2 < dx) 
        {
            error += dx;
            y1 += sy;
        }
    }
}


void rotation(struct Shape s, double rotationx, double rotationy, double rotationz)
{
    for (int i = 0; i < s.pointslength; ++i)
    {
        int x = s.points[i][0];
        int y = s.points[i][1];
        int z = s.points[i][2];
        
        //Rotation X axis
        s.points[i][0] = (int)(x);
        s.points[i][1] = (int)(y * cos(rotationx) + z * -sin(rotationx));
        s.points[i][2] = (int)(y * sin(rotationx) + z * cos(rotationx));
        
        x = s.points[i][0];
        y = s.points[i][1];
        z = s.points[i][2];
        
        //Rotation Y axis
        s.points[i][0] = (int)(x * cos(rotationy) + z * sin(rotationy));
        s.points[i][1] = (int)(y);
        s.points[i][2] = (int)(x * sin(rotationy) + z * cos(rotationy));
        
        x = s.points[i][0];
        y = s.points[i][1];
        z = s.points[i][2];
        
        //Rotation Z axis
        s.points[i][0] = (int)(x * cos(rotationz) + y * sin(rotationz));
        s.points[i][1] = (int)(x * -sin(rotationz) + y * cos(rotationz));
        s.points[i][2] = (int)(z);
    }
}


void shape(struct imageFrame *image, int points[8][3], int relations[12][2], int relationssize)
{
    for (int i = 0; i < relationssize; ++i)
    {
        line(image, points[relations[i][0]][0], points[relations[i][0]][1], points[relations[i][1]][0], points[relations[i][1]][1]);
    }
}


    ////////////////    3D    /////////////////

int i;
int time = 0;
double rx = 0;
double ry = 0;
double rz = 0;

int points1[8][3];
int relations1[12][2] = {{0,1},{0,2},{1,3},{2,3},{0,4},{2,6},{3,7},{1,5},{7,5},{4,5},{6,7},{4,6}};

struct Shape cube;

    ////////////////    3D    /////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow) {
    const wchar_t window_class_name[] = L"My Window Class";
    static WNDCLASS window_class = { 0 };
    window_class.lpfnWndProc = WindowProcessMessage;
    window_class.hInstance = hInstance;
    window_class.lpszClassName = (PCSTR)window_class_name;
    RegisterClass(&window_class);

    frame_bitmap_info.bmiHeader.biSize = sizeof(frame_bitmap_info.bmiHeader);
    frame_bitmap_info.bmiHeader.biPlanes = 1;
    frame_bitmap_info.bmiHeader.biBitCount = 32;
    frame_bitmap_info.bmiHeader.biCompression = BI_RGB;
    frame_device_context = CreateCompatibleDC(0);

    static HWND window_handle;
    window_handle = CreateWindow((PCSTR)window_class_name, "Drawing Pixels", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                 640, 300, 640, 480, NULL, NULL, hInstance, NULL);
    if(window_handle == NULL) { return -1; }

    cube = shapeconstruct((int *)points1, 8, (int *)relations1, 12);

    while(!quit) 
    {

        ////////////////    3D    /////////////////

        int points1[8][3] = {{-100,-100,100},{100,-100,100},{-100,100,100},{100,100,100},{-100,-100,-100},{100,-100,-100},{-100,100,-100},{100,100,-100}};

        cube.points = points1;

        ////////////////    3D    /////////////////

        static MSG message = { 0 };
        while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) { DispatchMessage(&message); }

        InvalidateRect(window_handle, NULL, FALSE);
        UpdateWindow(window_handle);
    }

    free(cube.points);
    free(cube.relations);
    free(&cube);

    return 0;
}


LRESULT CALLBACK WindowProcessMessage(HWND window_handle, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(message) 
    {
        case WM_QUIT:
        case WM_DESTROY: 
            {
            quit = 1;
            } break;

        case WM_PAINT: 
            {
            static PAINTSTRUCT paint;
            static HDC device_context;
            device_context = BeginPaint(window_handle, &paint);
            BitBlt(device_context,
                   paint.rcPaint.left, paint.rcPaint.top,
                   paint.rcPaint.right - paint.rcPaint.left, paint.rcPaint.bottom - paint.rcPaint.top,
                   frame_device_context,
                   paint.rcPaint.left, paint.rcPaint.top,
                   SRCCOPY);
            EndPaint(window_handle, &paint);
            } break;

        case WM_SIZE: 
            {
                frame_bitmap_info.bmiHeader.biWidth  = LOWORD(lParam);
                frame_bitmap_info.bmiHeader.biHeight = HIWORD(lParam);

                if(frame_bitmap) DeleteObject(frame_bitmap);
                frame_bitmap = CreateDIBSection(NULL, &frame_bitmap_info, DIB_RGB_COLORS, &frame.pixels, 0, 0);
                SelectObject(frame_device_context, frame_bitmap);

                frame.width =  LOWORD(lParam);
                frame.height = HIWORD(lParam);
            } break;

        case WM_KEYDOWN:
            {
                if (GetAsyncKeyState(0x41)) //A
                {
                    for (int i = 0; i < frame.height * frame.width; ++i)
                    {
                        frame.pixels[i] = 0x262626;
                    }
                    ry += 0.10;
                    rx += 0.10;
                    rotation(cube, rx, ry, 0);
                    shape(&frame, cube.points, cube.relations, cube.relationslength);
                }
            } break;

        default: 
            {
            return DefWindowProc(window_handle, message, wParam, lParam);
            }
    }
    return 0;
}