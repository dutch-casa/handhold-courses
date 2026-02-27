import "./card.css";

// TODO: Create a Card component
// Props: readonly title: string, readonly value: string, readonly change: string
//
// Structure:
//   <div className="card">
//     <h3 className="card-title">{title}</h3>
//     <p className="card-value">{value}</p>
//     <p className="card-change">{change}</p>
//   </div>

// TODO: Create a CardSkeleton component
// Same layout as Card but with skeleton placeholder elements:
//   <div className="card">
//     <div className="skeleton skeleton-title" />
//     <div className="skeleton skeleton-value" />
//     <div className="skeleton skeleton-change" />
//   </div>

export function Card({ title, value, change }: {
  readonly title: string;
  readonly value: string;
  readonly change: string;
}) {
  return (
    <div className="card">
      <h3>{title}</h3>
      <p>{value}</p>
      <p>{change}</p>
    </div>
  );
}

export function CardSkeleton() {
  return (
    <div className="card">
      {/* skeleton placeholders */}
    </div>
  );
}
