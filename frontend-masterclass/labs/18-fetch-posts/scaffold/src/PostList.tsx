// TODO: Import useQuery from "@tanstack/react-query"

// TODO: Define Post interface
// - readonly id: number
// - readonly title: string
// - readonly body: string

// TODO: Create PostList component
// Props: { onSelectPost: (id: number) => void }
//
// useQuery:
//   queryKey: ["posts"]
//   queryFn: fetch https://jsonplaceholder.typicode.com/posts → .json() → .slice(0, 10)
//
// Handle: isPending → <p>Loading posts...</p>
// Handle: isError → <p>Error loading posts</p>
//
// Render: <ul> with posts mapped to <li key={post.id} onClick={() => onSelectPost(post.id)}>

export function PostList({ onSelectPost }: { readonly onSelectPost: (id: number) => void }) {
  return <div>{/* Build post list here */}</div>;
}
