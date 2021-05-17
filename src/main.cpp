#include "Parser.h"
#include "fmt/core.h"
#include "nix/dynamic_loader.hpp"
#include "nix/stdlib.hpp"
#include "nix/tempfile.hpp"
#include "nix/unistd.hpp"
#include "plot/Graph.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

int main() {
    srand(time(0));
    std::string input;
    std::getline(std::cin, input);
    Lexer lexer(input);
    Parser parser(lexer);

    std::string s = parser.eval() + ';';
    fmt::print("{}\n", s);

    auto pipefd = nix::fpipe();

    fmt::print(pipefd.fp_write(), "#include <math.h>\ndouble sum(double x, double y) {{ return {}; }}", s);
    nix::fclose(pipefd.fp_write());

    nix::tempfile temp("/tmp/main_XXXXXX");
    pid_t pid = nix::fork([&]() {
        nix::dup2(pipefd.fd_read(), STDIN_FILENO);
        nix::execv("/usr/bin/gcc", { "gcc", "-o", temp, "-x", "c", "-", "--shared", "-O2", "-lm", "-Wl,--as-needed" });
    });
    nix::fclose(pipefd.fp_read());

    int status;
    nix::wait(status);
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

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Teste",
        sf::Style::Close | sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);
    window.setPosition({ 460, 0 });

    window.clear(sf::Color::Black);
    window.display();

    sf::Font font;
    font.loadFromFile("resources/Roboto.ttf");

    std::string title = input + " = 0";

    sf::RectangleShape graphRect(sf::Vector2f(1920, 1080));
    graphRect.setPosition(sf::Vector2f(0, 0));

    sf::RectangleShape plotRect(sf::Vector2f(1920, 1080));
    plotRect.setPosition(sf::Vector2f(0, 0));

    Graph myGraph(graphRect, plotRect, sf::Vector2f(a, b), sf::Vector2f(c, d),
        title, 30, font);

    myGraph.plotClear(sf::Color::Black);

    std::thread thr([&] {
        myGraph.plotRelation(func, sf::Color(255, 64, 64));
    });

    // thr.join();
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

    // double x, y;
    // scanf("%lf %lf", &x, &y);
    // printf("%lf\n", func(x, y));
    return 0;
}