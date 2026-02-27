#pragma once
#include <memory>
#include <vector>
#include <string>
#include <span>

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double radius);
    double area() const override;
    double perimeter() const override;
    std::string name() const override;
private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height);
    double area() const override;
    double perimeter() const override;
    std::string name() const override;
private:
    double width_, height_;
};

class Triangle : public Shape {
public:
    Triangle(double a, double b, double c);
    double area() const override;
    double perimeter() const override;
    std::string name() const override;
private:
    double a_, b_, c_;
};

class CompositeShape : public Shape {
public:
    void add(std::unique_ptr<Shape> shape);
    double area() const override;
    double perimeter() const override;
    std::string name() const override;
private:
    std::vector<std::unique_ptr<Shape>> children_;
};

double total_area(std::span<const std::unique_ptr<Shape>> shapes);
