#include "solution.h"
#include <cmath>
#include <numbers>

Circle::Circle(double radius) : radius_(radius) {}

double Circle::area() const {
    // TODO: pi * r * r
    return 0.0;
}

double Circle::perimeter() const {
    // TODO: 2 * pi * r
    return 0.0;
}

std::string Circle::name() const { return "Circle"; }

Rectangle::Rectangle(double width, double height) : width_(width), height_(height) {}

double Rectangle::area() const {
    // TODO: w * h
    return 0.0;
}

double Rectangle::perimeter() const {
    // TODO: 2 * (w + h)
    return 0.0;
}

std::string Rectangle::name() const { return "Rectangle"; }

Triangle::Triangle(double a, double b, double c) : a_(a), b_(b), c_(c) {}

double Triangle::area() const {
    // TODO: Heron's formula
    return 0.0;
}

double Triangle::perimeter() const {
    // TODO: a + b + c
    return 0.0;
}

std::string Triangle::name() const { return "Triangle"; }

void CompositeShape::add(std::unique_ptr<Shape> shape) {
    // TODO: take ownership
}

double CompositeShape::area() const {
    // TODO: sum of children
    return 0.0;
}

double CompositeShape::perimeter() const {
    // TODO: sum of children
    return 0.0;
}

std::string CompositeShape::name() const { return "Composite"; }

double total_area(std::span<const std::unique_ptr<Shape>> shapes) {
    // TODO: sum all areas
    return 0.0;
}
