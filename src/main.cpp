#include <cmath>

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Parser.h"
#include "plot/Graph.hpp"

int main() {
    std::string input;
    std::getline(std::cin, input);
    float a, b, c, d;
    std::cin >> a >> b >> c >> d;
    Lexer lexer(input);
    Parser parser(lexer);
    auto f = [&parser](double x) -> double {
        return parser[x];
    };

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Teste",
        sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("resources/Roboto.ttf");

    std::string title = "Teste";

    sf::RectangleShape graphRect(sf::Vector2f(800, 800));
    graphRect.setPosition(sf::Vector2f(100, 100));

    sf::RectangleShape plotRect(sf::Vector2f(600, 600));
    plotRect.setPosition(sf::Vector2f(200, 200));

    Graph myGraph(graphRect, plotRect, sf::Vector2f(a, b), sf::Vector2f(c, d),
        title, 30, font);

    myGraph.plotClear();
    myGraph.plotFunction(f, sf::Color::Red);
    myGraph.display();

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Cyan);
        window.draw(myGraph);
        window.display();
    }
}

// #include <iostream>
// #include <vector>

// #include "Functions.h"
// #include "Lexer.h"
// #include "Parser.h"
// #include "aux.h"

// using namespace std;

// ostream& operator<<(ostream& out, Parser const& p) {
//     out << p.getToken();
//     for (auto const& e : p.getChildren())
//         out << " " << e;
//     return out;
// }

// int main(int, char**) {
//     std::string in;
//     getline(cin, in);
//     Lexer lex(in);
//     Parser p(lex);

//     cout << "Lexer:" << endl;
//     for (auto e : lex)
//         cout << e << " ";
//     cout << endl;

//     cout << "Parser:" << endl;
//     cout << p << endl;
//     double x;
//     cin >> x;
//     std::cout << p.evaluate(x) << std::endl;
// }