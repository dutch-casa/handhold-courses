// TODO: Define a User type
// - readonly id: number
// - name: string
// - email: string
// - role: "admin" | "editor" | "viewer"
// export type User = ...

// TODO: Create a RequestState<T> discriminated union
// Kinds: "idle", "loading", "success" (with data: T), "error" (with message: string)
// All properties should be readonly
// export type RequestState<T> = ...

// TODO: Create a PaginatedResponse<T> generic
// - readonly items: readonly T[]
// - readonly page: number
// - readonly totalPages: number
// export type PaginatedResponse<T> = ...

// TODO: Create UserUpdate using Partial and Omit
// Should allow updating any User field except id
// export type UserUpdate = ...

// TODO: Write a function that handles RequestState exhaustively
// export function renderState(state: RequestState<User[]>): string {
//   switch (state.kind) {
//     case "idle": ...
//     case "loading": ...
//     case "success": ...
//     case "error": ...
//   }
// }
