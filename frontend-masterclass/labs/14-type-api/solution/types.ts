export type User = {
  readonly id: number;
  readonly name: string;
  readonly email: string;
  readonly role: "admin" | "editor" | "viewer";
};

export type RequestState<T> =
  | { readonly kind: "idle" }
  | { readonly kind: "loading" }
  | { readonly kind: "success"; readonly data: T }
  | { readonly kind: "error"; readonly message: string };

export interface PaginatedResponse<T> {
  readonly items: readonly T[];
  readonly page: number;
  readonly totalPages: number;
}

export type UserUpdate = Partial<Omit<User, "id">>;

export function renderState(state: RequestState<User[]>): string {
  switch (state.kind) {
    case "idle":
      return "Ready to load users.";
    case "loading":
      return "Loading users...";
    case "success":
      return `Loaded ${state.data.length} users.`;
    case "error":
      return `Error: ${state.message}`;
  }
}
