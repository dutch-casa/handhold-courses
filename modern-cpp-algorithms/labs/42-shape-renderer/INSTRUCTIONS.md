# Shape Renderer — Inheritance and Composition

Abstract base classes define the interface. Concrete shapes implement it. Composition combines them.

## Tasks

- [ ] Implement abstract `Shape` base class with pure virtual `area()`, `perimeter()`, and `name()` methods
- [ ] Implement `Circle` (constructor takes radius) — area = pi * r^2, perimeter = 2 * pi * r
- [ ] Implement `Rectangle` (constructor takes width, height) — area = w * h, perimeter = 2 * (w + h)
- [ ] Implement `Triangle` (constructor takes three side lengths) — area via Heron's formula, perimeter = a + b + c
- [ ] Implement `CompositeShape` — holds `vector<unique_ptr<Shape>>`, area and perimeter are sums of children
- [ ] Implement `total_area(shapes)` — free function taking `span<const unique_ptr<Shape>>`

## Hints

- Heron's formula: `s = (a+b+c)/2`, `area = sqrt(s*(s-a)*(s-b)*(s-c))`
- Use `std::numbers::pi` from `<numbers>` for pi
- `CompositeShape::add(unique_ptr<Shape>)` takes ownership via move
- Virtual destructor on the base class is mandatory

Use `Run Tests` to check your progress.
