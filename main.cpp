#include <string>
#include <iostream>
#include <fmt/core.h>

#include <glad/gl.h>
#include <SDL.h>
#include <SDL_syswm.h>

#define SOKOL_IMPL
#define SOKOL_GLCORE33
#define SOKOL_EXTERNAL_GL_LOADER 
#include <sokol_gfx.h>

int main(int, char **)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
    return -1;
  }

  if (SDL_GL_LoadLibrary(nullptr) < 0)
  {
    printf("SDL_GL_LoadLibrary failed. SDL_Error: %s\n", SDL_GetError());
    return -1;
  }

  // Request an OpenGL 4.5 context (should be core)
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
  // Also request a depth buffer
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

  uint32_t flags_ = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;

  auto sdl_window = SDL_CreateWindow("Hello Sokol", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, flags_);

  if (sdl_window == nullptr)
  {
    std::cout << fmt::format("SDL Window could not be created: {}\n", SDL_GetError());
    return -1;
  }

  /*SDL_version version;
  SDL_VERSION(&version);
  SDL_SysWMinfo wmi;

  if (!SDL_GetWindowWMInfo(sdl_window, &wmi, SDL_SYSWM_CURRENT_VERSION))
  {
    std::cout << fmt::format("SDL_SysWMinfo could not be retrieved: {}\n", SDL_GetError());
    return -1;
  }*/

  SDL_GLContext gl_ctx;
  gl_ctx = SDL_GL_CreateContext(sdl_window);

  SDL_GL_MakeCurrent(sdl_window, gl_ctx);

  GLADloadfunc gl_proc_resolver = (GLADloadfunc)SDL_GL_GetProcAddress;
  int version = gladLoadGL(gl_proc_resolver);
  std::cout << fmt::format("OpenGL {}.{} loaded\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

  /* setup sokol_gfx */
  sg_desc sgDesc{0};
  sg_setup(&sgDesc);

  /* a vertex buffer */
  const float vertices[] = {
      // positions            // colors
      0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
      0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f};

  sg_buffer_desc sgBufferDesc{
      .data = SG_RANGE(vertices)};

  sg_buffer vbuf = sg_make_buffer(&sgBufferDesc);

  sg_shader_desc sgShaderDesc{
      .vs = {.source =
                 "#version 330\n"
                 "layout(location=0) in vec4 position;\n"
                 "layout(location=1) in vec4 color0;\n"
                 "out vec4 color;\n"
                 "void main() {\n"
                 "  gl_Position = position;\n"
                 "  color = color0;\n"
                 "}\n"},
      .fs = {.source =
                 "#version 330\n"
                 "in vec4 color;\n"
                 "out vec4 frag_color;\n"
                 "void main() {\n"
                 "  frag_color = color;\n"
                 "}\n"}};

  /* a shader */
  sg_shader shd = sg_make_shader(&sgShaderDesc);

  /* a pipeline state object (default render states are fine for triangle) */
  sg_pipeline_desc sgPipelineDesc{.shader = shd};

  sgPipelineDesc.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
  sgPipelineDesc.layout.attrs[1].format = SG_VERTEXFORMAT_FLOAT4;

  sg_pipeline pip = sg_make_pipeline(&sgPipelineDesc);

  /* resource bindings */
  sg_bindings bind{0};
  bind.vertex_buffers[0] = vbuf;

  /* default pass action (clear to grey) */
  sg_pass_action pass_action = {0};

  /* draw loop */
  SDL_Event sdl_event;
  bool running = true;

  const int FPS = 60;
  const int frameDelay = 1000 / FPS;
  Uint32 frameStart;
  int frameTime;

  while (running)
  {
    frameStart = SDL_GetTicks();
    while (SDL_PollEvent(&sdl_event) != 0)
    {
      if (sdl_event.type == SDL_QUIT)
        running = false;
    }

    int cur_width, cur_height;
    SDL_GL_GetDrawableSize(sdl_window, &cur_width, &cur_height);
    sg_begin_default_pass(&pass_action, cur_width, cur_height);
    sg_apply_pipeline(pip);
    sg_apply_bindings(&bind);
    sg_draw(0, 3, 1);
    sg_end_pass();
    sg_commit();
    SDL_GL_SwapWindow(sdl_window);

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime)
    {
      SDL_Delay(frameDelay - frameTime);
    }
  }
  /* cleanup */
  sg_shutdown();
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();

  return 0;
}
