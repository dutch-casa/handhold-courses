#include <gtest/gtest.h>
#include "../src/solution.h"
#include <cmath>
#include <numbers>

TEST(Circle, Area) {
    Circle c(5.0);
    EXPECT_NEAR(c.area(), std::numbers::pi * 25.0, 0.001);
}

TEST(Circle, Perimeter) {
    Circle c(3.0);
    EXPECT_NEAR(c.perimeter(), 2.0 * std::numbers::pi * 3.0, 0.001);
}

TEST(Rectangle, Area) {
    Rectangle r(4.0, 6.0);
    EXPECT_NEAR(r.area(), 24.0, 0.001);
}

TEST(Rectangle, Perimeter) {
    Rectangle r(4.0, 6.0);
    EXPECT_NEAR(r.perimeter(), 20.0, 0.001);
}

TEST(Triangle, Area345) {
    Triangle t(3.0, 4.0, 5.0);
    EXPECT_NEAR(t.area(), 6.0, 0.001);
}

TEST(Triangle, Perimeter) {
    Triangle t(3.0, 4.0, 5.0);
    EXPECT_NEAR(t.perimeter(), 12.0, 0.001);
}

TEST(CompositeShape, SumOfChildren) {
    CompositeShape comp;
    comp.add(std::make_unique<Circle>(1.0));
    comp.add(std::make_unique<Rectangle>(2.0, 3.0));
    EXPECT_NEAR(comp.area(), std::numbers::pi + 6.0, 0.001);
}

TEST(TotalArea, MultipleShapes) {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(1.0));
    shapes.push_back(std::make_unique<Rectangle>(2.0, 3.0));
    EXPECT_NEAR(total_area(shapes), std::numbers::pi + 6.0, 0.001);
}

TEST(Shape, Names) {
    Circle c(1.0);
    Rectangle r(1.0, 1.0);
    Triangle t(3.0, 4.0, 5.0);
    CompositeShape comp;
    EXPECT_EQ(c.name(), "Circle");
    EXPECT_EQ(r.name(), "Rectangle");
    EXPECT_EQ(t.name(), "Triangle");
    EXPECT_EQ(comp.name(), "Composite");
}
