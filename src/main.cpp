#include "Parser.h"
#include "fmt/core.h"
#include "nix/dynamic_loader.hpp"
#include "nix/fork.hpp"
#include "nix/pipe.hpp"
#include "nix/tempfile.hpp"
#include "plot/Graph.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <random>

#if defined(KHAT_SINGLETHREAD)
#include "util/useless_thread.hpp"
using thread = useless_thread;
#else
#include <thread>
using thread = std::thread;
#endif

int main() {
    std::string input;
    std::getline(std::cin, input);
    Lexer lexer(input);
    Parser parser(lexer);

    std::string eval = parser.eval();
    fmt::print("{}\n", eval);

    parser.simplify();

    eval = parser.eval();
    fmt::print("{}\n", eval);

    nix::pipe pipe;
    fmt::print(pipe(1), "#include <math.h>\ndouble sum(double x, double y) {{ return {}; }}", eval);
    fclose(pipe(1));

    nix::tempfile temp("/tmp/main_XXXXXX");

    nix::fork gcc([&]() {
        nix::dup(pipe[0], nix::fileno::stdin);
        nix::execv("/usr/bin/gcc", { "gcc", "-o", temp, "-x", "c", "-", "--shared", "-O2", "-lm", "-Wl,--as-needed" });
    });
    pid_t pid = gcc.pid();
    fclose(pipe(0));
    int status = gcc.wait();
    if (!WIFEXITED(status)) {
        fmt::print(stderr, "gcc has not terminated correctly!\n");
        fmt::print(stderr, "Status: {}\n", status);
        return 1;
    }

    status = WEXITSTATUS(status);
    if (status != 0)
        return status;

    nix::dynamic_loader dll(temp);
    auto func = reinterpret_cast<double (*)(double, double)>(dll("sum"));

    float a, b, c, d;
    std::cin >> a >> b >> c >> d;

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Khat",
        sf::Style::Close | sf::Style::Titlebar);
    window.setVerticalSyncEnabled(true);
    window.setPosition({ 0, 0 });

    window.clear(sf::Color::Black);
    window.display();

    sf::Font font;
    font.loadFromFile("resources/Roboto.ttf");

    std::string title = input + " = 0";

    sf::RectangleShape graphRect(sf::Vector2f(1000, 1080));
    graphRect.setPosition(sf::Vector2f(0, 0));

    sf::RectangleShape plotRect(sf::Vector2f(1000, 1000));
    plotRect.setPosition(sf::Vector2f(0, 0));

    Graph myGraph(graphRect, plotRect, sf::Vector2f(a, b), sf::Vector2f(c, d),
        title, 30, font);

    myGraph.plotClear(sf::Color::Black);

    thread thr([&] {
        myGraph.plotRelation(func, sf::Color(255, 64, 64));
    });

    myGraph.display();

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

        window.clear(sf::Color::Black);
        myGraph.faz();
        window.draw(myGraph);
        window.display();
    }

    myGraph.terminate();
    thr.join();
    return 0;
}
