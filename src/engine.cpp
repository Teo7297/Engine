#include "engine.h"
#include "renderer.h"
#include "entity.h"
#include "shader.h"
#include "Texture.h"

namespace engine
{
    Engine::Engine(std::string appName)
        : m_width{800},
          m_height{600},
          m_window{nullptr},
          m_renderer{nullptr},
          m_scene{},
          m_physicsScene{},
          m_lastFrameTime{},
          m_entitiesToDestroy{},
          m_nextEntityID{1}
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

        // Initialize GLEW
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return;
        }

        glfwGetFramebufferSize(m_window, &m_width, &m_height);
        glViewport(0, 0, m_width, m_height);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

        genBuffersForFBRendering();

        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsDark();

        // Initialize ImGui for GLFW and OpenGL
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        // glEnable(GL_CULL_FACE); //todo: FIX quads are drawn backwards.............

        initFreeType();

        m_renderer = std::make_unique<Renderer>();
    }

    Engine::~Engine()
    {
    }

// ################ PUBLIC ################
#pragma region PUBLIC
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
            m_frameTime = frameTime.count();
            m_lastFrameTime = endTime;

            //? Physics Loop
            for (auto e : m_physicsScene)
            {
                for (auto e2 : m_physicsScene)
                {
                    if (e != e2)
                        e->checkCollision(*e2);
                }
            }
            glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
            glClear(GL_COLOR_BUFFER_BIT);
            //? Update + Draw Loop
            for (auto &e : m_scene)
            {
                e->update(m_frameTime);
                if (e->drawEnabled)
                    m_renderer->render(e->getShader(), e->getVAO(), e->verticesCount, e->getTexture());
            }

            //? Late Update Loop
            for (auto &e : m_scene)
            {
                e->lateUpdate(m_frameTime);
                if (e->toDestroy)
                    m_entitiesToDestroy.push(e->getID());
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glClear(GL_COLOR_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_framebufferTexture);
            m_framebufferShader->Bind();

            glBindVertexArray(m_fbVAO);
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            m_framebufferShader->Unbind();
            glBindTexture(GL_TEXTURE_2D, 0);

            //? Destroy
            while (!m_entitiesToDestroy.empty())
            {
                int id = m_entitiesToDestroy.front();

                // Erase from scene
                m_scene.erase(
                    std::remove_if(
                        m_scene.begin(), m_scene.end(),
                        [&](const auto &e)
                        { return e->getID() == id; }),
                    m_scene.end());

                // Erase from physics scene
                m_physicsScene.erase(
                    std::remove_if(
                        m_physicsScene.begin(), m_physicsScene.end(),
                        [&](const auto &e)
                        { return e->getID() == id; }),
                    m_physicsScene.end());

                m_entitiesToDestroy.pop();
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
                cachePosition.y -= ((ch.Size.y)) * 1.3 * realScale;
                cachePosition.x = copyX;
            }
            else if (*c == ' ')
            {
                cachePosition.x += (ch.Advance >> 6) * realScale;
            }
            else
            {
                float xpos = cachePosition.x + ch.Bearing.x * realScale;
                float ypos = cachePosition.y - (256 - ch.Bearing.y) * realScale;

                m_transforms[workingIndex] = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(256 * realScale, 256 * realScale, 0));
                m_letterMap[workingIndex] = ch.TextureID;

                // render quad
                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                cachePosition.x += (ch.Advance >> 6) * realScale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
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
#pragma endregion
#pragma region PRIVATE
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
        int fontSize = 256;
        FT_Set_Pixel_Sizes(face, fontSize, fontSize);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures(1, &m_fontTextures);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_fontTextures);
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, fontSize, fontSize, 128, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

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

    void Engine::renderGUI()
    {
        if (glfwGetKey(m_window, GLFW_KEY_F12) == GLFW_PRESS)
            m_showDebugUI = true;

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (m_showDebugUI)
            ImGui::ShowMetricsWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Engine::genBuffersForFBRendering()
    {
        glGenFramebuffers(1, &m_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

        glGenTextures(1, &m_framebufferTexture);
        glBindTexture(GL_TEXTURE_2D, m_framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_framebufferTexture, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "Framebuffer is not complete" << std::endl;
            exit(-1);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_framebufferShader = std::make_shared<Shader>("../shaders/fb_texture.vs", "../shaders/fb_texture.fs");
        m_framebufferShader->Bind();
        m_framebufferShader->SetUniform("textureSampler", 0);
        m_framebufferShader->Unbind();

        glGenVertexArrays(1, &m_fbVAO);
        glGenBuffers(1, &m_fbVBO);
        // glGenBuffers(1, &m_fbEBO);

        glBindVertexArray(m_fbVAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_fbVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_fbVertices), m_fbVertices, GL_STATIC_DRAW);

        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_fbEBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_fbIndices), m_fbIndices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // Texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

#pragma endregion
}
