#include "engine.h"
#include "renderer.h"
#include "entity.h"
#include "shader.h"
#include "Texture.h"

bool active = true;
float my_color[4]{0.5, 0.5, 0};
void imgui_sample1()
{
    // Create a window called "My First Tool", with a menu bar.
    if (!active)
        return;
    ImGui::Begin("My First Tool", &active, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O"))
            { /* Do stuff */
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            { /* Do stuff */
            }
            if (ImGui::MenuItem("Close", "Ctrl+W"))
            {
                active = false;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Edit a color stored as 4 floats
    ImGui::ColorEdit4("Color", my_color);

    // Generate samples and plot them
    float samples[100];
    for (int n = 0; n < 100; n++)
        samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
    ImGui::PlotLines("Samples", samples, 100);

    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
    ImGui::BeginChild("Scrolling");
    for (int n = 0; n < 50; n++)
        ImGui::Text("%04d: Some text", n);
    ImGui::EndChild();
    ImGui::End();
}

void imgui_sample2()
{
    // Create a simple ImGui window
    ImGui::Begin("Hello, ImGui!");

    // Add UI elements
    ImGui::Text("This is a simple ImGui window.");
    if (ImGui::Button("Click me!"))
        // Do something when the button is clicked
        std::cout << "CLICK!\n";
    // End the ImGui window
    ImGui::End();
}

namespace engine
{
    Engine::Engine(std::string appName)
        : m_width{800},
          m_height{600},
          m_window{nullptr},
          m_renderer{nullptr},
          m_scene{},
          m_physicsScene{},
          m_lastFrameTime{}
    {
        // Initialize GLFW
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }
        // Set GLFW window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL 4.6
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use core profile

        // Create a GLFW window
        m_window = glfwCreateWindow(m_width, m_height, appName.c_str(), NULL, NULL);
        if (!m_window)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(m_window);

        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsDark();

        // Initialize ImGui for GLFW and OpenGL
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        // Initialize GLEW
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return;
        }

        // glEnable(GL_CULL_FACE); //todo: FIX quads are drawn backwards.............

        initFreeType();

        m_renderer = std::make_unique<Renderer>();
    }

    Engine::~Engine()
    {
    }

    // ################ PUBLIC ################
    void Engine::start()
    {
        m_lastFrameTime = std::chrono::high_resolution_clock::now();

        for (auto e : m_scene)
        {
            if (e->drawEnabled)
            {
                auto shader = e->getShader();
                shader->Bind();

                // Create VBO and VAO
                GLuint VBO;
                glGenVertexArrays(1, &(e->vao));
                glGenBuffers(1, &VBO);

                glBindVertexArray(e->vao);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, e->vertices.size() * sizeof(float), e->vertices.data(), GL_STATIC_DRAW);

                // Specify vertex attributes
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
                glEnableVertexAttribArray(1);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);

                shader->Unbind();
            }

            if (e->physicsEnabled)
                m_physicsScene.push_back(e);
            e->init();
        }

        while (!glfwWindowShouldClose(m_window))
        {
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> frameTime = endTime - m_lastFrameTime;
            m_lastFrameTime = endTime;

            glClear(GL_COLOR_BUFFER_BIT);

            //? Physics Loop
            for (auto e : m_physicsScene)
            {
                for (auto e2 : m_physicsScene)
                {
                    if (e != e2)
                        e->checkCollision(*e2);
                }
            }

            //? Update + Draw Loop
            for (auto e : m_scene)
            {
                e->update(frameTime.count());
                if (e->drawEnabled)
                    m_renderer->render(e->getShader(), e->getVAO(), e->verticesCount, e->getTexture());
            }

            //? Late Update Loop
            for (auto &e : m_scene)
            {
                e->lateUpdate(frameTime.count());
            }

            renderGUI();

            glfwPollEvents();
            glfwSwapBuffers(m_window);
        }

        glfwTerminate();
    }

    // # INTERFACE METHODS
    void Engine::closeApp()
    {
        glfwSetWindowShouldClose(m_window, true);
    }

    int Engine::getKey(int key)
    {
        return glfwGetKey(m_window, key);
    }

    std::shared_ptr<Entity> Engine::findEntityByName(const std::string &name) const
    {
        for (auto e : m_scene)
        {
            if (name == e->getName())

                return e;
        }
        return nullptr;
    }

    void Engine::renderText(const std::string text, const glm::vec3 position, const float scale, const Color color)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const float realScale = scale * 48.0f / 256.0f;
        glm::vec3 cachePosition{position.x, position.y, position.z};

        float copyX = cachePosition.x;
        m_textShader->Bind();
        m_textShader->SetUniform("textColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_fontTextures);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        int workingIndex = 0;
        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = m_characters[*c];

            if (*c == '\n')
            {
                cachePosition.y -= ((ch.Size.y)) * 1.3 * scale;
                cachePosition.x = copyX;
            }
            else if (*c == ' ')
            {
                cachePosition.x += (ch.Advance >> 6) * scale;
            }
            else
            {
                float xpos = cachePosition.x + ch.Bearing.x * scale;
                float ypos = cachePosition.y - (256 - ch.Bearing.y) * scale;

                m_transforms[workingIndex] = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(256 * scale, 256 * scale, 0));
                m_letterMap[workingIndex] = ch.TextureID;

                // render quad
                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                cachePosition.x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
                workingIndex++;
                if (workingIndex == ARRAY_LIMIT)
                {
                    // todo: add a SetUniform variant that allows to set workingIndex
                    glUniformMatrix4fv(glGetUniformLocation(m_textShader->ID, "transforms"), workingIndex, GL_FALSE, &m_transforms[0][0][0]);
                    glUniform1iv(glGetUniformLocation(m_textShader->ID, "letterMap"), workingIndex, &m_letterMap[0]);
                    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, workingIndex);
                    workingIndex = 0;
                }
            }
        }
        glUniformMatrix4fv(glGetUniformLocation(m_textShader->ID, "transforms"), workingIndex, GL_FALSE, &m_transforms[0][0][0]);
        glUniform1iv(glGetUniformLocation(m_textShader->ID, "letterMap"), workingIndex, &m_letterMap[0]);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, workingIndex);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        glDisable(GL_BLEND);
    }

    void Engine::setBackgroundColor(const Color color)
    {
        glClearColor(color.r, color.g, color.b, 1.f);
    }
    // ################ PRIVATE ################

    void Engine::initFreeType()
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return;
        }

        if (m_fontName.empty())
        {
            std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
            return;
        }

        // load font as face
        FT_Face face;
        if (FT_New_Face(ft, m_fontName.c_str(), 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            return;
        }

        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 256, 256);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures(1, &m_fontTextures);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_fontTextures);
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, 256, 256, 128, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph: " << c << std::endl;
                continue;
            }

            glTexSubImage3D(
                GL_TEXTURE_2D_ARRAY,
                0, 0, 0, int(c),
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows, 1,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer);

            // set texture options
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // now store character for later use
            Character character{
                static_cast<int>(c),
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)};

            m_characters.insert(std::pair<char, Character>(c, character));
        }

        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        // re-enable default byte alignment value
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        // todo: check if this works better
        // m_letterMap.reserve(ARRAY_LIMIT);
        // m_transforms.reserve(ARRAY_LIMIT);
        for (int i = 0; i < ARRAY_LIMIT; i++)
        {
            m_letterMap.push_back(0);
            m_transforms.push_back(glm::mat4(1.0f));
        }

        // Texture coords == Vertex position (texcoord.y inverted in the shader!)
        GLfloat vertex_data[] = {
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            1.0f,
            0.0f,
        };

        // configure VAO/VBO for texture quads
        // -----------------------------------
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // todo: occhio al sizeof se cambiamo location di vertex_data!
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_textShader = std::make_shared<Shader>("../shaders/text_yt.vs", "../shaders/text_yt.fs");

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_width), 0.0f, static_cast<float>(m_height));
        m_textShader->Bind();
        m_textShader->SetUniform("projection", projection);

        m_textShader->Unbind();
    }

    void Engine::renderGUI() const
    {
        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        imgui_sample2(); // todo: maybe here we want some kind of list of GUIs
        imgui_sample1();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
