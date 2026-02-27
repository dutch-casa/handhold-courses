#include "solution.h"
#include <cmath>
#include <numbers>
#include <numeric>

Circle::Circle(double radius) : radius_(radius) {}
double Circle::area() const { return std::numbers::pi * radius_ * radius_; }
double Circle::perimeter() const { return 2.0 * std::numbers::pi * radius_; }
std::string Circle::name() const { return "Circle"; }

Rectangle::Rectangle(double width, double height) : width_(width), height_(height) {}
double Rectangle::area() const { return width_ * height_; }
double Rectangle::perimeter() const { return 2.0 * (width_ + height_); }
std::string Rectangle::name() const { return "Rectangle"; }

Triangle::Triangle(double a, double b, double c) : a_(a), b_(b), c_(c) {}
double Triangle::area() const {
    double s = (a_ + b_ + c_) / 2.0;
    return std::sqrt(s * (s - a_) * (s - b_) * (s - c_));
}
double Triangle::perimeter() const { return a_ + b_ + c_; }
std::string Triangle::name() const { return "Triangle"; }

void CompositeShape::add(std::unique_ptr<Shape> shape) {
    children_.push_back(std::move(shape));
}
double CompositeShape::area() const {
    double sum = 0.0;
    for (const auto& child : children_) sum += child->area();
    return sum;
}
double CompositeShape::perimeter() const {
    double sum = 0.0;
    for (const auto& child : children_) sum += child->perimeter();
    return sum;
}
std::string CompositeShape::name() const { return "Composite"; }

double total_area(std::span<const std::unique_ptr<Shape>> shapes) {
    double sum = 0.0;
    for (const auto& s : shapes) sum += s->area();
    return sum;
}
