#include <igw.h>
#include <video/window.h>

int main() {
    Window window;
    window_init(&window, "Hello World", 1200, 600);

    f32 slider_value = 0.0f;

    while (!window_should_close(&window))
    {
        window_update_input(&window);
        ig_frame_new();

        window_clear(0.125f, 0.125f, 0.125f, 1.0f);
        {
            ig_begin("Sample Window", nullptr, 0);
            ig_text("This is useful, I guess..");
            ig_slider1f("Slider", &slider_value, 0.0f, 1.0f, "%.2f", 0);
            ig_end();
        }

        ig_render();
        window_swap_buffers(&window);
    }

    window_terminate(&window);
    return 0;
}
