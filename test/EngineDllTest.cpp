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
    ~ent() override{};

    void init() override
    {
        Entity::init();

        position = glm::vec3(400, 300, 0);
        rotation = glm::angleAxis(glm::radians(45.f), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void update(const float frameTime) override
    {
        Entity::update(frameTime);
        time += frameTime;

        if (m_input->getKey(GLFW_KEY_UP))
        {
            position += glm::vec3(0.f, speed, 0.f) * frameTime;
        }
        if (m_input->getKey(GLFW_KEY_DOWN))
        {
            position += glm::vec3(0.f, -speed, 0.f) * frameTime;
        }
    }

private:
    float time = 0.f;
    float speed = .1f;
    glm::vec3 dir = glm::vec3(1.f, 0.f, 0.f);
};

int main()
{
    auto app = engine::Engine("test app");

    auto e = app.makeEntity<ent>("myEntity");

    app.start();

    return 0;
}