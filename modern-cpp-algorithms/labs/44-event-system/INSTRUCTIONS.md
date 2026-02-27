# Event System — Design Patterns in Modern C++

A type-erased event dispatcher. Simple interface: subscribe to event types, emit events, handlers fire automatically.

## Tasks

- [ ] Implement `EventBus::subscribe<E>(handler)` — register a handler for event type E; returns a subscription ID
- [ ] Implement `EventBus::unsubscribe(id)` — remove a subscription by ID
- [ ] Implement `EventBus::emit<E>(event)` — dispatch event to all handlers registered for type E

## Hints

- Use `std::type_index` (from `<typeindex>`) as a map key to route events by type
- Store handlers as `std::any` wrapping `std::function<void(const E&)>`
- On emit: look up the vector of handlers for `typeid(E)`, cast each `std::any` back to `std::function<void(const E&)>`, invoke
- Assign incrementing IDs to each subscription for unsubscribe

Use `Run Tests` to check your progress.
