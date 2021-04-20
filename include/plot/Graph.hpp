#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <SFML/Graphics.hpp>
#include <atomic>
#include <functional>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

class Graph : public sf::Drawable {

private:
    sf::RectangleShape _graphBox {}, _plotBox {};
    sf::RenderTexture _graphTexture {}, _plotTexture;
    sf::Vector2f _bottomLeft {}, _topRight {};
    std::string _title {};
    int _fontSize {};
    std::atomic_bool _terminate {};
    sf::Font _font {};
    sf::Text _text {};
    std::vector<sf::Text> _xScale {}, _yScale {};
    std::vector<sf::Vertex> _points;

public:
    //Constructors

    Graph();
    Graph(const sf::RectangleShape& graphBox, const sf::RectangleShape& plotBox, const sf::Vector2f& bottomLeft, const sf::Vector2f& topRight, const std::string& title, const int& fontSize, const sf::Font& font);

    //Setters

    void setGraphBox(const sf::RectangleShape& graphicBox);
    void setPlotBox(const sf::RectangleShape& plotBox);
    void setTitle(const std::string& title);
    void setFontSize(const int& fontSize);
    void setFont(const sf::Font& font);
    void setBottomLeft(const sf::Vector2f& bottomLeft);
    void setTopRight(const sf::Vector2f& topRight);
    void setText();
    void setXScale(const double& x0, const double& xf, const int& N);
    void setYScale(const double& y0, const double& yf, const int& N);

    //Getters

    sf::RectangleShape getGraphBox() const;
    sf::RectangleShape getPlotBox() const;
    std::string getTitle() const;
    int getFontSize() const;
    sf::Font getFont() const;
    sf::Vector2f getBottomLeft() const;
    sf::Vector2f getTopRight() const;
    sf::Texture getPlotTexture() const;
    sf::Texture getGraphTexture() const;
    sf::Text getText() const;

    //Conversion Function

    sf::Vector2f convertPoint(const sf::Vector2f& point) const;

    //Basic Plot Functions

    void plotClear(const sf::Color& color = sf::Color::White);
    void plotPoint(const sf::Vector2f& point, const sf::Color& color = sf::Color::Black);
    void plotLine(const sf::Vector2f& startPoint, const sf::Vector2f& endPoint, const sf::Color& color = sf::Color::Black);

    //Plot Functions

    void plotData(const std::vector<sf::Vector2f>& data, const bool& drawLines = true, const bool& drawPoints = false, const sf::Color& lineColor = sf::Color::Black, const sf::Color& pointColor = sf::Color::Black);
    void plotFunction(const std::function<double(double)>& f, const sf::Color& color);
    void plotRelation(const std::function<double(double, double)>& f, const sf::Color& color);

    //Display Function
    void display();
    void terminate();

    //Draw Function

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif