# Type a real API

Write TypeScript types for an API that returns user data. Use discriminated unions, generics, mapped types, and readonly to make invalid states unrepresentable.

## Tasks

- [ ] Define a `User` type with `readonly id`, `name`, `email`, and `role` (union of "admin" | "editor" | "viewer")
- [ ] Create a `RequestState<T>` discriminated union generic with kinds: "idle", "loading", "success" (with `data: T`), "error" (with `message: string`)
- [ ] Create a `PaginatedResponse<T>` generic with `items: readonly T[]`, `page: number`, `totalPages: number`
- [ ] Create a `UserUpdate` type using `Partial` and `Omit` — allow updating any field except `id`
- [ ] Make all domain types use `readonly` — mutations should be compile errors

## Hints

Discriminated union with generic:

```typescript
type RequestState<T> =
  | { readonly kind: "idle" }
  | { readonly kind: "loading" }
  | { readonly kind: "success"; readonly data: T }
  | { readonly kind: "error"; readonly message: string };
```

Partial + Omit for update types:

```typescript
type UserUpdate = Partial<Omit<User, "id">>;
```

Readonly arrays:

```typescript
interface PaginatedResponse<T> {
  readonly items: readonly T[];
  readonly page: number;
  readonly totalPages: number;
}
```

Use `Run Tests` in the testing panel to check your progress.
