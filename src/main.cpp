#include <iostream>
#include <fstream>

#include <vector>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

unsigned int wWidth = 0;
unsigned int wHeight = 0;

struct Font
{
    int m_size = 18;
    int m_color[3] = { 0, 0, 0 };
};

struct Shape
{
    sf::RectangleShape m_rectangleShape;
    sf::CircleShape m_circleShape;
    std::string m_shapeType;
    std::string m_name;
    float m_pos[2] = {0.0f, 0.0f};
    float m_velocity[2] = { 0.0f, 0.0f };
    float m_scale = 1.0f;
    float m_color[3] = { 0.0f, 0.0f, 0.0f };
    float m_width = 0.0f, m_height = 0.0f;
    float m_radius = 0.0f;
    bool m_draw = true;
    bool m_text = true;
};

void SimulateShapes(std::vector<Shape> & shapes, std::vector<sf::Text> & texts)
{
    for (size_t i=0; i<shapes.size(); ++i)
    {
        Shape& shape = shapes[i];
		sf::Text& text = texts[i];
        shape.m_pos[0] += shape.m_velocity[0];
        shape.m_pos[1] += shape.m_velocity[1];
        sf::FloatRect localBounds;
        if (shape.m_shapeType == "Rectangle")
        {
            sf::FloatRect localBounds = shape.m_rectangleShape.getLocalBounds();
            if (shape.m_pos[0] + localBounds.position.x < 0 || shape.m_pos[0] + localBounds.size.x * shape.m_scale> wWidth)
            {
                shape.m_velocity[0] *= -1.0f;
            }
            if (shape.m_pos[1] + localBounds.position.y < 0 || shape.m_pos[1] + localBounds.size.y * shape.m_scale> wHeight)
            {
                shape.m_velocity[1] *= -1.0f;
            }
            shape.m_rectangleShape.setPosition({ shape.m_pos[0], shape.m_pos[1]});
			shape.m_rectangleShape.setScale({ shape.m_scale, shape.m_scale });
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin({ textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f });
            text.setPosition(shape.m_rectangleShape.getPosition() + shape.m_rectangleShape.getLocalBounds().size * shape.m_scale / 2.0f);
        }
        else if (shape.m_shapeType == "Circle")
        {
            sf::FloatRect localBounds = shape.m_circleShape.getLocalBounds();
            if (shape.m_pos[0] + localBounds.position.x < 0 || shape.m_pos[0] + localBounds.size.x * shape.m_scale > wWidth)
            {
                shape.m_velocity[0] *= -1.0f;
            }
            if (shape.m_pos[1] + localBounds.position.y < 0 || shape.m_pos[1] + localBounds.size.y * shape.m_scale> wHeight)
            {
                shape.m_velocity[1] *= -1.0f;
            }
            shape.m_circleShape.setPosition({ shape.m_pos[0], shape.m_pos[1]});
            shape.m_circleShape.setScale({ shape.m_scale, shape.m_scale });
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin({ textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f });
            text.setPosition(shape.m_circleShape.getPosition() + shape.m_circleShape.getLocalBounds().size * shape.m_scale / 2.0f);
        }
    }
}

static void renderShapes(sf::RenderWindow& window, std::vector<Shape>& shapes, std::vector<sf::Text>& texts)
{
    for (size_t i = 0; i < shapes.size(); ++i)
    {
        Shape& shape = shapes[i];
        sf::Text& text = texts[i];
        if(shape.m_draw)
        {
            if (shape.m_shapeType == "Rectangle")
            {
                shape.m_rectangleShape.setFillColor({ uint8_t(shape.m_color[0] * 255), uint8_t(shape.m_color[1] * 255), uint8_t(shape.m_color[2] * 255)});
                window.draw(shape.m_rectangleShape);
            }
            else if (shape.m_shapeType == "Circle")
            {
                shape.m_circleShape.setFillColor({ uint8_t(shape.m_color[0] * 255), uint8_t(shape.m_color[1] * 255), uint8_t(shape.m_color[2] * 255)});
                window.draw(shape.m_circleShape);
            }
        }
        if (shape.m_text)
        {
            window.draw(text);
        }
    }
}

int main(int argc, char* argv[])
{
    std::ifstream fin("config/config.txt");

    std::string start;

    sf::Font myFont;

    std::vector<Shape> shapes;
    std::vector<sf::Text> texts;
    Font font;
    while(fin >> start)
    {
        if (start == "Window")
        {
            fin >> wWidth >> wHeight;
        }
        else if (start == "Font")
        {
            std::string path;
            if (fin >> path)
            {
                if (!myFont.openFromFile(path))
                {
                    std::cerr << "Could not load font " << path << std::endl;
                    exit(-1);
                }
                fin >> font.m_size >> font.m_color[0] >> font.m_color[1] >> font.m_color[2];
            }
        }
        else if (start == "Rectangle")
        {
            Shape s;
            fin >> s.m_name >> s.m_pos[0] >> s.m_pos[1] >> s.m_velocity[0] >> s.m_velocity[1] >> s.m_color[0]
                >> s.m_color[1] >> s.m_color[2] >> s.m_width >> s.m_height;
            s.m_color[0] /= 255.0f;
            s.m_color[1] /= 255.0f;
            s.m_color[2] /= 255.0f;
            s.m_name.resize(32);
            s.m_shapeType = "Rectangle";
            s.m_rectangleShape = sf::RectangleShape({ s.m_width , s.m_height });
            s.m_rectangleShape.setPosition({ s.m_pos[0], s.m_pos[1]});
            s.m_rectangleShape.setFillColor({ uint8_t(s.m_color[0] * 255), uint8_t(s.m_color[1] * 255), uint8_t(s.m_color[2] * 255) });
            shapes.push_back(s);
        }
        else if (start == "Circle")
        {
            Shape s;
            fin >> s.m_name >> s.m_pos[0] >> s.m_pos[1] >> s.m_velocity[0] >> s.m_velocity[1] >> s.m_color[0]
                >> s.m_color[1] >> s.m_color[2] >> s.m_radius;
            s.m_color[0] /= 255.0f;
            s.m_color[1] /= 255.0f;
            s.m_color[2] /= 255.0f;
            s.m_name.resize(32);
            s.m_shapeType = "Circle";
            s.m_circleShape = sf::CircleShape(s.m_radius, 32);
            s.m_circleShape.setPosition({ s.m_pos[0], s.m_pos[1]});
            s.m_circleShape.setFillColor({ uint8_t(s.m_color[0] * 255), uint8_t(s.m_color[1] * 255), uint8_t(s.m_color[2] * 255) });
            shapes.push_back(s);
        }
    }

    std::vector<char*> items;
    int itemIndex = 0;

    sf::RenderWindow window(sf::VideoMode({ wWidth, wHeight }), "Shape Simulation");
    window.setFramerateLimit(60);

    // initialize IMGUI and create a clock for its internal timing
    if (!ImGui::SFML::Init(window))
    {
        std::cerr << "Could not initialize the window\n";
        std::exit(-1);
    }

    sf::Clock deltaClock;

    // Scale the imgui ui and text size by 2
    ImGui::GetStyle().ScaleAllSizes(1.5f);
    ImGui::GetIO().FontGlobalScale = 1.5f;

    // main loop
    while (window.isOpen())
    {
        while (auto event = window.pollEvent()) 
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) 
            {
                window.close();
            }
        }

        for (auto& shape : shapes)
        {
            items.emplace_back((char*)(shape.m_name.c_str()));
			sf::Text text(myFont, shape.m_name, font.m_size);
			text.setFillColor({ uint8_t(font.m_color[0]), uint8_t(font.m_color[1]), uint8_t(font.m_color[2]) });
			texts.emplace_back(text);
        }

        sf::Time deltaTime = deltaClock.restart();
        ImGui::SFML::Update(window, deltaTime);

        ImGui::Begin("Shape Properties");
		ImGui::Combo("Shape", &itemIndex, items.data(), (int)items.size());
		Shape& shape = shapes[itemIndex];
		ImGui::Checkbox("Draw", &shape.m_draw);
        ImGui::SameLine();
        ImGui::Checkbox("Draw Text", &shape.m_text);
		ImGui::SliderFloat("Scale", &shape.m_scale, 0.0f, 4.0f);
		ImGui::SliderFloat2("Velocity", shape.m_velocity, -8.0f, 8.0f);
		ImGui::SameLine();
		ImGui::ColorEdit3("Color", shape.m_color);
        ImGui::InputText("Text", shape.m_name.data(), 32);
        items.clear();
        ImGui::End();

        SimulateShapes(shapes, texts);
        // basic rendering function calls
		window.clear();
        renderShapes(window, shapes, texts);
        texts.clear();
        ImGui::SFML::Render(window);
        window.display(); // swaps the bufferss
    }
    return 0;
}