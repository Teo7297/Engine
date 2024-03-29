#include <../Engine>

class ent : public engine::Entity
{
public:
    ent(engine::IInput *input, std::string name) : Entity(input, name)
    {
        vertices = std::vector<float>{0.0f, 0.5f, 0.0f,
                                      -0.5f, -0.5f, 0.0f,
                                      0.5f, -0.5f, 0.0f};
    };
    ~ent(){};

    void init() override
    {
        Entity::init();

        if (m_name == "myEntity")
            position = glm::vec3(400, 300, 0);
        else
            position = glm::vec3(400, 400, 0);
        rotation = glm::angleAxis(glm::radians(45.f), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void update(const float frameTime) override
    {
        Entity::update(frameTime);
        time += frameTime;

        if (m_input->getKey(GLFW_KEY_UP))
        {
            position += speed * upDir * frameTime;
        }
        if (m_input->getKey(GLFW_KEY_DOWN))
        {
            position += speed * downDir * frameTime;
        }
        if (m_input->getKey(GLFW_KEY_ESCAPE))
        {
            m_input->closeApp();
        }
    }

private:
    float time = 0.f;
    float speed = .5f;
    glm::vec3 upDir = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 downDir = glm::vec3(0.f, -1.f, 0.f);
};

int main()
{
    auto app = engine::Engine("test app");

    auto e = app.makeEntity<ent>("myEntity");
    auto e2 = app.makeEntity<ent>("myEntity2");
    // e2->position = {0.1, 0.1, 0};

    app.start();

    return 0;
}