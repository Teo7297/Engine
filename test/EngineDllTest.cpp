#include <../Engine>

class ent : public engine::Entity
{
public:
    ent(std::string name) : Entity(name){};
    ~ent() override{};

    void init() override
    {
        position = glm::vec3(400, 300, 0);
        rotation = glm::angleAxis(glm::radians(45.f), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void update(const float frameTime) override
    {
        Entity::update(frameTime);

        time += frameTime;
    }

private:
    float time = 0.f;
    float speed = .001f;
    glm::vec3 dir = glm::vec3(1.f, 0.f, 0.f);
};

int main()
{
    auto app = engine::Engine("test app");

    auto e = std::make_shared<ent>("myEntity");
    app.addEntity(e);

    app.start();

    return 0;
}