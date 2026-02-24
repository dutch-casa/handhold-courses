// TODO: Import useQuery from "@tanstack/react-query"

// TODO: Create PostDetail component
// Props: { postId: number; onBack: () => void }
//
// useQuery:
//   queryKey: ["post", postId]
//   queryFn: fetch https://jsonplaceholder.typicode.com/posts/${postId} → .json()
//
// Handle: isPending → <p>Loading...</p>
// Handle: isError → <p>Error loading post</p>
//
// Render:
//   <button onClick={onBack}>Back</button>
//   <h2>{post.title}</h2>
//   <p>{post.body}</p>

export function PostDetail({
  postId,
  onBack,
}: {
  readonly postId: number;
  readonly onBack: () => void;
}) {
  return <div>{/* Build post detail here */}</div>;
}
