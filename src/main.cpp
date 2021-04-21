#include <cmath>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "Parser.h"
#include "plot/Graph.hpp"
#include <cerrno>
#include <unistd.h>

int main() {
    std::string input;
    std::getline(std::cin, input);
    Lexer lexer(input);
    Parser parser(lexer);
    auto f = [&parser](double x, double y) -> double {
        errno = 0;
        auto calc = parser.evaluate(x, y);
        if (errno != 0)
            return NAN;

        return calc;
    };

    float a, b, c, d;
    std::cin >> a >> b >> c >> d;

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Teste",
        sf::Style::Close | sf::Style::Titlebar);
    window.setVerticalSyncEnabled(true);

    sf::Font font;
    font.loadFromFile("resources/Roboto.ttf");

    std::string title = input + " = 0";

    sf::RectangleShape graphRect(sf::Vector2f(800, 800));
    graphRect.setPosition(sf::Vector2f(100, 100));

    sf::RectangleShape plotRect(sf::Vector2f(600, 600));
    plotRect.setPosition(sf::Vector2f(200, 200));

    Graph myGraph(graphRect, plotRect, sf::Vector2f(a, b), sf::Vector2f(c, d),
        title, 30, font);

    myGraph.plotClear();

    std::thread thr([&] {
        myGraph.plotRelation(f, sf::Color::Blue);
    });

    // thr.join();
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                myGraph.terminate();
                thr.join();
                return 0;
            }
        }

        myGraph.display();
        window.clear(sf::Color::Cyan);
        window.draw(myGraph);
        window.display();
    }

    myGraph.terminate();
    thr.join();
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

//     cout << "Lexer:" << endl;
//     for (auto e : lex)
//         cout << e << " ";
//     cout << endl;

//     Parser p(lex);
//     cout << "Parser:" << endl;
//     cout << p << endl;
//     double x, y;
//     cin >> x >> y;
//     std::cout << p.evaluate(x, y) << std::endl;
// }