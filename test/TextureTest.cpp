#include <../Engine>

class ent : public engine::Rectangle
{
public:
    ent(engine::IInput *input, std::string name) : Rectangle(input, name){};
    ~ent(){};

    void init() override
    {
        Rectangle::init();

        // position = glm::vec3(150, 150, 0);
    }

    void update(const float frameTime) override
    {
        Rectangle::update(frameTime);
        if (m_input->getKey(GLFW_KEY_UP))
        {
            position += 0.5f * glm::vec3{0, 1, 0} * frameTime;
        }
    }
};

const Path tPathAvatar = "C:\\Users\\teoca\\OneDrive\\Immagini\\avatar.png";
const Path tPathBricks = "C:\\Users\\teoca\\Desktop\\GamesFromScratch\\Engine\\textures\\bricks.jpg";
const char *vPath = "C:\\Users\\teoca\\Desktop\\GamesFromScratch\\Engine\\shaders\\simple_texture.vs";
const char *fPath = "C:\\Users\\teoca\\Desktop\\GamesFromScratch\\Engine\\shaders\\simple_texture.fs";

int main()
{
    std::cout << "Texture rendering test!\n";

    auto app = engine::Engine("test app");

    auto e = app.makeEntity<ent>("bricks");
    e->scale = {300, 300, 0};
    e->position = {50, 150, 0};
    auto shader = std::make_shared<engine::Shader>(vPath, fPath);
    e->addShader(shader);
    auto t = std::make_shared<engine::Texture>(tPathBricks);
    e->addTexture(t);

    auto e2 = app.makeEntity<ent>("avatar");
    e2->scale = {300, 300, 0};
    e2->position = {400, 150, 0};
    auto shader2 = std::make_shared<engine::Shader>(vPath, fPath);
    e2->addShader(shader2);
    auto t2 = std::make_shared<engine::Texture>(tPathAvatar, 1);
    e2->addTexture(t2);

    app.start();

    return 0;
}