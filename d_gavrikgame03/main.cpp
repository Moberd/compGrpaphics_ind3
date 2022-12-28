#include "headers.h"
#include "misc.h"
#include "Battlefield.h"
#include <array>

void SetIcon(sf::Window& wnd);

bool isCamActive = false;
bool isCamTouched = false;
glm::vec2 mousePos;
glm::vec2 mouseDelta;
// w a s d
std::array<bool, 4> handles = { false, false, false, false };

int main() {
	sf::Window window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "World of teapot", sf::Style::Default, sf::ContextSettings(24));
	SetIcon(window);
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	checkOpenGLerror(1);
	Scene* s = new Battlefield(
		{
			std::make_pair("triang.vert", "triang.frag"),
		},
		std::make_pair("meshes/teapot.obj", "meshes/tinkoff.jpg"), // tank
		std::make_pair("meshes/MirTanka/Field.obj", "meshes/MirTanka/Field.png"), // field.
		{
			std::make_tuple("meshes/Cup02.obj", "meshes/Cup02.jpg", glm::vec3(0.14f)), // enemies
			std::make_tuple("meshes/tea_cup.obj", "meshes/tea_cup.png", glm::vec3(0.02f)),
		},
		{
			std::make_pair("meshes/MirTanka/Barrel.obj", "meshes/MirTanka/Barrel.png"), // decorations
			std::make_pair("meshes/MirTanka/ChristmasTree.obj", "meshes/MirTanka/ChristmasTree.png"),
			std::make_pair("meshes/MirTanka/Tree.obj", "meshes/MirTanka/Tree.png"),
			std::make_pair("meshes/MirTanka/Stone-1.obj", "meshes/MirTanka/Stone-1.png"),
			std::make_pair("meshes/MirTanka/Stone-2.obj", "meshes/MirTanka/Stone-1.png"),
		},
		std::make_tuple("meshes/sphere.obj", "meshes/voda.jpg", glm::vec3(0.6f))); // missile model. vec3 param is scale factor. just in case obj is too big

	while (window.isOpen()) {
		sf::Event event;
		mouseDelta = glm::vec2(0.f);
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) { window.close(); } // poll events
			else if (event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width,
					event.size.height);
			}
			else if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code)
				{
				case sf::Keyboard::W:
					handles[0] = true;
					break;

				case sf::Keyboard::A:
					handles[1] = true;
					break;

				case sf::Keyboard::S:
					handles[2] = true;
					break;

				case sf::Keyboard::D:
					handles[3] = true;
					break;

				case sf::Keyboard::F: {
					Battlefield* bf = dynamic_cast<Battlefield*>(s);
					if (!bf) break;
					bf->player->SwitchHeadlight();
				} break;

					// these are just for lulz
				case sf::Keyboard::E: {
					Battlefield* bf = dynamic_cast<Battlefield*>(s);
					if (!bf) break;
					bf->player->ChangeExp(-10.f); // just random delta
				} break;

				case sf::Keyboard::R: {
					Battlefield* bf = dynamic_cast<Battlefield*>(s);
					if (!bf) break;
					bf->player->ChangeExp(10.f); // just random delta
				} break;

				case sf::Keyboard::C: {
					Battlefield* bf = dynamic_cast<Battlefield*>(s);
					if (!bf) break;
					bf->player->ChangeCutOff(0.01f); // just random delta
				} break;

				case sf::Keyboard::V: {
					Battlefield* bf = dynamic_cast<Battlefield*>(s);
					if (!bf) break;
					bf->player->ChangeCutOff(-0.01f); // just random delta
				} break;

				default:
					break;
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {

				switch (event.mouseButton.button)
				{
				case sf::Mouse::Left: {
					Battlefield* bf = dynamic_cast<Battlefield*>(s);
					if (!bf) break;
					bf->player->DropMissile();
				} break;
				case sf::Mouse::Right:
					isCamActive = true;
					break;
				default:
					break;
				}
			}
			else if (event.type == sf::Event::KeyReleased) {
				switch (event.key.code)
				{
				case sf::Keyboard::W:
					handles[0] = false;
					break;

				case sf::Keyboard::A:
					handles[1] = false;
					break;

				case sf::Keyboard::S:
					handles[2] = false;
					break;

				case sf::Keyboard::D:
					handles[3] = false;
					break;

				default:
					break;
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				switch (event.mouseButton.button)
				{
				case sf::Mouse::Right:
					isCamActive = false;
					break;
				default:
					break;
				}
			}
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				s->camera->ProcessMouseScroll(event.mouseWheel.delta);
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				if (!isCamTouched) {
					mousePos = glm::vec2(event.mouseMove.x, event.mouseMove.y);
					isCamTouched = true;
				}
				auto newMousePos = glm::vec2(event.mouseMove.x, event.mouseMove.y);
				mouseDelta = newMousePos - mousePos;
				mousePos = newMousePos;
			}
		}
		if (isCamActive) s->camera->ProcessMouse(mouseDelta);
		for (size_t i = 0; i < 4; i++) {
			if (handles[i]) s->camera->ProcessKeyboard((Camera::Camera_Movement)i, s->GetDeltaTime());
		}
		s->ResetClock();

		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		s->Draw();

		window.display();
	}

	return 0;
}

void SetIcon(sf::Window& wnd)
{
	sf::Image image;

	// Вместо рисования пикселей, можно загрузить иконку из файла (image.loadFromFile("icon.png"))
	image.create(16, 16);
	for (int i = 0; i < 16; ++i)
		for (int j = 0; j < 16; ++j)
			image.setPixel(i, j, {
				(sf::Uint8)(i * 16), (sf::Uint8)(j * 16), 0 });

	wnd.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
}