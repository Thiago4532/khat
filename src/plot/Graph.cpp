#include "plot/Graph.hpp"
#include "rng.hpp"
#include <cerrno>
#include <complex>
#include <cstring>
#include <iostream>
#include <queue>
#include <cmath>
#include <random>

//Setters Implementations

void Graph::setGraphBox(const sf::RectangleShape& graphBox) {
    _graphBox = graphBox;
    _graphBox.setFillColor(sf::Color::Transparent);
    _graphBox.setOutlineColor(sf::Color::Black);
    _graphBox.setOutlineThickness(2);
    _graphTexture.create(_graphBox.getLocalBounds().width, _graphBox.getLocalBounds().height);
}

void Graph::setPlotBox(const sf::RectangleShape& plotBox) {
    _plotBox = plotBox;
    _plotBox.setFillColor(sf::Color(0, 0, 0, 0));
    _plotBox.setOutlineColor(sf::Color::Black);
    _plotBox.setOutlineThickness(2);
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    _plotTexture.create(plotBox.getLocalBounds().width, plotBox.getLocalBounds().height, settings);
    _Nx = _plotTexture.getTexture().getSize().x;
    _Ny = _plotTexture.getTexture().getSize().y;

    _lines1 = new sf::Vertex[_Nx * _Ny];
    _lines2 = new sf::Vertex[_Nx * _Ny];

    _collision = new bool*[_Nx * 6];
    for (int i = 0; i < _Nx * 6; i++)
        _collision[i] = new bool[_Ny * 6];
}

void Graph::setTitle(const std::string& title) {
    _title = title;
}

void Graph::setFontSize(const int& fontSize) {
    _fontSize = fontSize;
}

void Graph::setFont(const sf::Font& font) {
    _font = font;
}

void Graph::setBottomLeft(const sf::Vector2f& bottomLeft) {
    _bottomLeft = bottomLeft;
}

void Graph::setTopRight(const sf::Vector2f& topRight) {
    _topRight = topRight;
}

void Graph::setText() {
    _text.setFont(_font);
    _text.setCharacterSize(_fontSize);
    _text.setString(_title);
    _text.setOrigin(
        _text.getLocalBounds().left + 0.5f * _text.getLocalBounds().width,
        _text.getLocalBounds().top);
    _text.setPosition(
        _graphBox.getGlobalBounds().left + 0.5f * _graphBox.getGlobalBounds().width,
        0.5f * (_plotBox.getGlobalBounds().top + _graphBox.getGlobalBounds().top + _plotBox.getGlobalBounds().height + _graphBox.getGlobalBounds().height));
    _text.setFillColor(sf::Color::Black);
}

void Graph::setXScale(const double& x0, const double& xf, const int& N) {
    double idealScale = (xf - x0) / N;
    double convertedScale = idealScale * pow(10, -floor(log10(idealScale)));
    double realScale {};
    realScale = convertedScale < 1.5 ? 1 : convertedScale < 3.5 ? 2
                                                                : 5;
    realScale *= pow(10, floor(log10(idealScale)));
    double number = ceil(x0 / realScale) * realScale;
    sf::Text aux;
    while (number <= xf) {
        if (abs(number / realScale) < 0.01)
            number = 0.0;
        double x = convertPoint(sf::Vector2f(number, 0.f)).x;
        std::string s;
        std::stringstream out;
        out << std::fixed;
        out.precision(floor(log10(idealScale)) < 0 ? -floor(log10(idealScale)) : 0);
        out << number;
        s = out.str();
        aux.setString(s);
        aux.setFont(_font);
        aux.setCharacterSize(0.5 * _fontSize);
        aux.setOrigin(
            aux.getLocalBounds().left + 0.5f * aux.getLocalBounds().width,
            aux.getLocalBounds().top);
        aux.setPosition(sf::Vector2f(
            _plotBox.getGlobalBounds().left + 2 + x,
            _plotBox.getGlobalBounds().top + _plotBox.getGlobalBounds().height + 0.1 * (_graphBox.getGlobalBounds().top - _plotBox.getGlobalBounds().top + _graphBox.getGlobalBounds().height - _plotBox.getGlobalBounds().height)));
        aux.setFillColor(sf::Color::Black);
        _xScale.push_back(aux);
        number += realScale;
    }
}

void Graph::setYScale(const double& y0, const double& yf, const int& N) {
    double idealScale = (yf - y0) / N;
    double convertedScale = idealScale * pow(10, -floor(log10(idealScale)));
    double realScale {};
    realScale = convertedScale < 1.5 ? 1 : convertedScale < 3.5 ? 2
                                                                : 5;
    realScale *= pow(10, floor(log10(idealScale)));
    double number = ceil(y0 / realScale) * realScale;
    sf::Text aux;
    while (number <= yf) {
        if (abs(number / realScale) < 0.01)
            number = 0.0;
        double y = convertPoint(sf::Vector2f(0.f, number)).y;
        std::string s;
        std::stringstream out;
        out << std::fixed;
        out.precision(floor(log10(idealScale)) < 0 ? -floor(log10(idealScale)) : 0);
        out << number;
        s = out.str();
        aux.setString(s);
        aux.setFont(_font);
        aux.setCharacterSize(0.5 * _fontSize);
        aux.setOrigin(
            aux.getLocalBounds().left + aux.getLocalBounds().width,
            aux.getLocalBounds().top + 0.5f * aux.getLocalBounds().height);
        aux.setPosition(sf::Vector2f(
            _plotBox.getGlobalBounds().left - 0.1 * (_plotBox.getGlobalBounds().left - _graphBox.getGlobalBounds().left),
            _plotBox.getGlobalBounds().top + y));
        aux.setFillColor(sf::Color::Black);
        _xScale.push_back(aux);
        number += realScale;
    }
}

//Getters Implementations

sf::RectangleShape Graph::getGraphBox() const {
    return _graphBox;
}

sf::RectangleShape Graph::getPlotBox() const {
    return _plotBox;
}

std::string Graph::getTitle() const {
    return _title;
}

int Graph::getFontSize() const {
    return _fontSize;
}

sf::Font Graph::getFont() const {
    return _font;
}

sf::Vector2f Graph::getBottomLeft() const {
    return _bottomLeft;
}

sf::Vector2f Graph::getTopRight() const {
    return _topRight;
}

sf::Texture Graph::getPlotTexture() const {
    return _plotTexture.getTexture();
}

sf::Text Graph::getText() const {
    return _text;
}

//Constructor Implementation

Graph::Graph(const sf::RectangleShape& graphBox, const sf::RectangleShape& plotBox, const sf::Vector2f& bottomLeft, const sf::Vector2f& topRight, const std::string& title, const int& fontSize, const sf::Font& font) {
    setGraphBox(graphBox);
    setPlotBox(plotBox);
    setTitle(title);
    setFontSize(fontSize);
    setFont(font);
    setBottomLeft(bottomLeft);
    setTopRight(topRight);
    setText();
    setXScale(bottomLeft.x, topRight.x, plotBox.getLocalBounds().width / 100);
    setYScale(bottomLeft.y, topRight.y, plotBox.getLocalBounds().height / 100);
    display();
}

//Conversion Function

sf::Vector2f Graph::convertPoint(const sf::Vector2f& point) const {
    sf::Vector2f realPoint {};

    realPoint.x = 0.5f + (point.x - _bottomLeft.x) * ((_plotTexture.getTexture().getSize().x - 1) / (_topRight.x - _bottomLeft.x));
    realPoint.y = 0.5f + (_topRight.y - point.y) * ((_plotTexture.getTexture().getSize().y - 1) / (_topRight.y - _bottomLeft.y));
    return realPoint;
}

//Basic Plot Functions Implementations
void Graph::plotClear(const sf::Color& color) {
    _plotTexture.clear(color);
}

void Graph::plotPoint(const sf::Vector2f& point, const sf::Color& color) {
    sf::Vector2f realPoint = convertPoint(point);

    sf::Vertex p[] = {
        sf::Vertex(realPoint, color),
    };
    _plotTexture.draw(p, 1, sf::Points);
}

static int k;
static sf::Clock clock2;

void Graph::plotLine(const sf::Vector2f& startPoint, const sf::Vector2f& endPoint, const sf::Color& color) {
    sf::Vector2f realStartPoint = convertPoint(startPoint), realEndPoint = convertPoint(endPoint);

    _lines1[k] = { realStartPoint, color };
    _lines2[k++] = { realEndPoint, color };
}

//Plot Functions Implementations

void Graph::plotData(const std::vector<sf::Vector2f>& data, const bool& drawLines, const bool& drawPoints, const sf::Color& lineColor, const sf::Color& pointColor) {

    for (int i = 1; i < data.size(); ++i) {

        if (drawLines) {
            plotLine(data[i - 1], data[i], lineColor);
        }

        if (drawPoints) {
            plotPoint(data[i - 1], pointColor);
            if (i == data.size() - 1) {
                plotPoint(data[i], pointColor);
            }
        }
    }
}

void Graph::plotFunction(const std::function<double(double)>& f, const sf::Color& color) {
    int N = _plotTexture.getTexture().getSize().x;

    std::vector<sf::Vector2f> data {};

    double x0 = _bottomLeft.x;
    double xf = _topRight.x;
    double step = (xf - x0) / (N - 1);

    for (int i = 0; i < N; ++i) {
        double x = x0 + i * step;
        data.push_back(sf::Vector2f(x, f(x)));
    }

    plotData(data, true, false, color);
}

void Graph::terminate() {
    _terminate = true;
}

bool Graph::codigo_do_luca(std::complex<double> const& c, double x0, double y0, double xf, double yf, bool coloca) {
    auto x = real(c);
    auto y = imag(c);

    if (std::isnan(x) || std::isnan(y))
        return true;

    if (x < x0 || x > xf || y < y0 || y > yf)
        return true;

    int i = std::floor((x - x0) * (6 * _Nx - 1) / (xf - x0));
    int j = std::floor((y - y0) * (6 * _Ny - 1) / (yf - y0));

    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            int i2 = i + di, j2 = j + dj;
            if (i2 < 0 || i2 >= 6 * _Nx || j2 < 0 || j2 >= 6 * _Ny)
                continue;

            if (_collision[i2][j2]) {
                return true;
            }
        }
    }
    if (_collision[i][j])
        return true;
    if (coloca)
        _collision[i][j] = true;
    return false;
}

void Graph::plotRelation(double (*f)(double, double), const sf::Color& color) {
    // sf::Clock clock;
    constexpr int IT_LIMIT = 2000;

    int Nx = _Nx, Ny = _Ny;
    std::vector<sf::Vector2f> data {};

    double x0 = _bottomLeft.x;
    double xf = _topRight.x;
    double stepx = (xf - x0) / (Nx - 1);

    double y0 = _bottomLeft.y;
    double yf = _topRight.y;

    double stepy = (yf - y0) / (Ny - 1);

    std::vector<sf::Vector2<double>> pontos;

    double aux = (stepx * stepx + stepy * stepy) * 0.01;
    int priority = 0;

    // for (int ll = 0; ll < valval && !_terminate; ll++) {
    while (!_terminate) {
        if (pontos.empty()) {
            if (priority > 10000)
                break;
            int i = rng() % Nx, j = rng() % Ny;
            pontos.push_back({ x0 + i * stepx, y0 + j * stepy });
            priority++;
        } else
            priority = 0;
        // int i = pontos[ll].first, j = pontos[ll].second;
        // int i = pontos.back().first, j = pontos.back().second;

        sf::Vector2<double> p0 = pontos.back();
        pontos.pop_back();
        sf::Vector2<double> pIni = p0;

        double error = 1e18;
        double derivativex {}, derivativey {};
        double modulo;
        double e = 0.1 * pow(stepx * stepx + stepy * stepy, 0.5);

        // TODO: Avoid the hack
        for (int it = 0; !_terminate; it++) {
            if (isnan(p0.x) || isnan(p0.y))
                break;

            derivativex = (f(p0.x + e, p0.y) - f(p0.x, p0.y)) / e;
            derivativey = (f(p0.x, p0.y + e) - f(p0.x, p0.y)) / e;
            
            if (isnan(derivativex) || isnan(derivativey))
                break;

            modulo = derivativex * derivativex + derivativey * derivativey;
            if (modulo * aux >= error && it > 25)
                break;

            sf::Vector2<double> p = p0 - sf::Vector2<double>(derivativex, derivativey) * f(p0.x, p0.y) / (derivativey * derivativey + derivativex * derivativex);

            error = f(p.x, p.y) * f(p.x, p.y);

            p0 = p;
        }
        if (_terminate)
            break;

        std::complex<double> p = { p0.x, p0.y };
        if (codigo_do_luca(p, x0, y0, xf, yf)) {
            continue;
        }

        auto dx1 = derivativex;
        auto dx2 = derivativey;
        auto paux = p;

        std::complex<double> grad = { derivativex, derivativey };

        double step = (stepx + stepy) / 4.0;
        while (true) {
            auto dir = (grad / std::abs(grad)) * std::complex<double>(0, -1) * step;
            auto p1 = p;
            p += dir;

            derivativex = (f(std::real(p) + e, std::imag(p)) - f(std::real(p), std::imag(p))) / e;
            derivativey = (f(std::real(p), std::imag(p) + e) - f(std::real(p), std::imag(p))) / e;

            grad = { derivativex, derivativey };

            p -= grad * f(std::real(p), std::imag(p)) / std::norm(grad);

            derivativex = (f(std::real(p) + e, std::imag(p)) - f(std::real(p), std::imag(p))) / e;
            derivativey = (f(std::real(p), std::imag(p) + e) - f(std::real(p), std::imag(p))) / e;

            grad = { derivativex, derivativey };

            plotLine(sf::Vector2f(std::real(p1), std::imag(p1)), sf::Vector2f(std::real(p), std::imag(p)), color);
            if (codigo_do_luca(p, x0, y0, xf, yf)) {
                break;
            }
        }

        {
            auto paux = p + grad * std::complex<double>(0, -1);

            auto derivativex = (f(std::real(paux) + e, std::imag(paux)) - f(std::real(paux), std::imag(paux))) / e;
            auto derivativey = (f(std::real(paux), std::imag(paux) + e) - f(std::real(paux), std::imag(paux))) / e;

            std::complex<double> grad = { derivativex, derivativey };

            paux -= grad * f(std::real(paux), std::imag(paux)) / std::norm(grad);

            // derivativex = (f(std::real(paux) + e, std::imag(paux)) - f(std::real(paux), std::imag(paux))) / e;
            // derivativey = (f(std::real(paux), std::imag(paux) + e) - f(std::real(paux), std::imag(paux))) / e;

            // grad = { derivativex, derivativey };

            // paux -= grad * f(std::real(paux), std::imag(paux)) / std::norm(grad);

            if (!codigo_do_luca(paux, x0, y0, xf, yf, false))
                pontos.push_back({ std::real(paux), std::imag(paux) });
        }

        {
            auto paux = p + grad;

            auto derivativex = (f(std::real(paux) + e, std::imag(paux)) - f(std::real(paux), std::imag(paux))) / e;
            auto derivativey = (f(std::real(paux), std::imag(paux) + e) - f(std::real(paux), std::imag(paux))) / e;

            std::complex<double> grad = { derivativex, derivativey };

            paux -= grad * f(std::real(paux), std::imag(paux)) / std::norm(grad);

            if (!codigo_do_luca(paux, x0, y0, xf, yf, false))
                pontos.push_back({ std::real(paux), std::imag(paux) });
        }

        {
            auto paux = p - grad;

            auto derivativex = (f(std::real(paux) + e, std::imag(paux)) - f(std::real(paux), std::imag(paux))) / e;
            auto derivativey = (f(std::real(paux), std::imag(paux) + e) - f(std::real(paux), std::imag(paux))) / e;

            std::complex<double> grad = { derivativex, derivativey };

            paux -= grad * f(std::real(paux), std::imag(paux)) / std::norm(grad);

            if (!codigo_do_luca(paux, x0, y0, xf, yf, false))
                pontos.push_back({ std::real(paux), std::imag(paux) });
        }

        derivativex = dx1;
        derivativey = dx2;
        p = paux;

        grad = { derivativex, derivativey };

        while (true) {
            auto dir = (grad / std::abs(grad)) * std::complex<double>(0, 1) * step;
            auto p1 = p;
            p += dir;

            derivativex = (f(std::real(p) + e, std::imag(p)) - f(std::real(p), std::imag(p))) / e;
            derivativey = (f(std::real(p), std::imag(p) + e) - f(std::real(p), std::imag(p))) / e;

            grad = { derivativex, derivativey };

            p -= grad * f(std::real(p), std::imag(p)) / std::norm(grad);

            derivativex = (f(std::real(p) + e, std::imag(p)) - f(std::real(p), std::imag(p))) / e;
            derivativey = (f(std::real(p), std::imag(p) + e) - f(std::real(p), std::imag(p))) / e;

            grad = { derivativex, derivativey };

            plotLine(sf::Vector2f(std::real(p1), std::imag(p1)), sf::Vector2f(std::real(p), std::imag(p)), color);
            if (codigo_do_luca(p, x0, y0, xf, yf)) {
                break;
            }
        }

        {
            auto paux = p + grad * std::complex<double>(0, -1);

            auto derivativex = (f(std::real(paux) + e, std::imag(paux)) - f(std::real(paux), std::imag(paux))) / e;
            auto derivativey = (f(std::real(paux), std::imag(paux) + e) - f(std::real(paux), std::imag(paux))) / e;

            std::complex<double> grad = { derivativex, derivativey };

            paux -= grad * f(std::real(paux), std::imag(paux)) / std::norm(grad);

            if (!codigo_do_luca(paux, x0, y0, xf, yf, false))
                pontos.push_back({ std::real(paux), std::imag(paux) });
        }

        {
            auto paux = p + grad;

            auto derivativex = (f(std::real(paux) + e, std::imag(paux)) - f(std::real(paux), std::imag(paux))) / e;
            auto derivativey = (f(std::real(paux), std::imag(paux) + e) - f(std::real(paux), std::imag(paux))) / e;

            std::complex<double> grad = { derivativex, derivativey };

            paux -= grad * f(std::real(paux), std::imag(paux)) / std::norm(grad);

            if (!codigo_do_luca(paux, x0, y0, xf, yf, false))
                pontos.push_back({ std::real(paux), std::imag(paux) });
        }

        {
            auto paux = p - grad;

            auto derivativex = (f(std::real(paux) + e, std::imag(paux)) - f(std::real(paux), std::imag(paux))) / e;
            auto derivativey = (f(std::real(paux), std::imag(paux) + e) - f(std::real(paux), std::imag(paux))) / e;

            std::complex<double> grad = { derivativex, derivativey };

            paux -= grad * f(std::real(paux), std::imag(paux)) / std::norm(grad);

            if (!codigo_do_luca(paux, x0, y0, xf, yf, false))
                pontos.push_back({ std::real(paux), std::imag(paux) });
        }
    }
}

//Display Function Implementation

void Graph::display() {
    _plotTexture.clear(sf::Color::Black);

    sf::Sprite sprite;

    sprite.setOrigin(
        sprite.getLocalBounds().left + 0.5f * sprite.getLocalBounds().width,
        sprite.getLocalBounds().top + 0.5f * sprite.getLocalBounds().height);

    sprite.setPosition(
        _plotBox.getGlobalBounds().left + 0.5f * _plotBox.getGlobalBounds().width - _graphBox.getGlobalBounds().left,
        _plotBox.getGlobalBounds().top + 0.5f * _plotBox.getGlobalBounds().height - _graphBox.getGlobalBounds().top);

    _graphTexture.clear(sf::Color::Black);

    sf::RectangleShape aux = _plotBox;
    aux.move(
        -_graphBox.getGlobalBounds().left,
        -_graphBox.getGlobalBounds().top);
    _graphTexture.draw(aux);

    aux = _graphBox;
    aux.move(
        -_graphBox.getGlobalBounds().left,
        -_graphBox.getGlobalBounds().top);
    _graphTexture.draw(aux);

    for (sf::Text v : _xScale) {
        sf::Text aux_ = v;
        aux_.move(
            -_graphBox.getGlobalBounds().left,
            -_graphBox.getGlobalBounds().top);
        _graphTexture.draw(aux_);
    }

    for (sf::Text v : _yScale) {
        sf::Text aux_ = v;
        aux_.move(
            -_graphBox.getGlobalBounds().left,
            -_graphBox.getGlobalBounds().top);
        _graphTexture.draw(aux_);
    }

    sf::Text aux_ = _text;
    aux_.move(
        -_graphBox.getGlobalBounds().left,
        -_graphBox.getGlobalBounds().top);
    _graphTexture.draw(aux_);
    _graphTexture.draw(sprite);
    _graphTexture.display();
}

//Draw Function Implementation

void Graph::faz() {
    static int ini = 0;
    static int var = 0;
    static sf::Color lineColor = sf::Color(0, 255, 255);

    bool passou = false;

    int x = k;
    for (; ini < x; ini++) {
        float d = std::sqrt((_lines1[ini].position.x - _lines2[ini].position.x) * (_lines1[ini].position.x - _lines2[ini].position.x) + (_lines1[ini].position.y - _lines2[ini].position.y) * (_lines1[ini].position.y - _lines2[ini].position.y));
        sf::RectangleShape line(sf::Vector2f(d, 2.5f));
        line.setOrigin(
            line.getLocalBounds().left + 0.5f * line.getLocalBounds().width,
            line.getLocalBounds().top + 0.5f * line.getLocalBounds().height);
        line.setPosition(0.5 * (_lines1[ini].position.x + _lines2[ini].position.x), 0.5 * (_lines1[ini].position.y + _lines2[ini].position.y));
        line.setFillColor(lineColor);
        double theta;
        double pi = 3.1415926535898;

        theta = (180.0 / pi) * std::atan2((_lines1[ini].position.y - _lines2[ini].position.y), (_lines1[ini].position.x - _lines2[ini].position.x));
        line.rotate(theta);
        _plotTexture.draw(line);
        passou = true;
        // var++;
        if (var >= 100) {
            var = 0;
            lineColor = sf::Color(rng() % 256, rng() % 256, rng() % 256);
        }
    }
    if (passou)
        _plotTexture.display();
}

void Graph::draw(sf::RenderTarget& target, sf::RenderStates states) const {

    // sf::Sprite sprite;

    // sprite.setTexture(_graphTexture.getTexture());

    // sprite.setOrigin(
    //     sprite.getLocalBounds().left,
    //     sprite.getLocalBounds().top);

    // sprite.setPosition(sf::Vector2f(
    //     _graphBox.getGlobalBounds().left,
    //     _graphBox.getGlobalBounds().top));

    // target.draw(sprite);

    sf::Sprite _sprite(_plotTexture.getTexture());
    target.draw(_sprite);
}
