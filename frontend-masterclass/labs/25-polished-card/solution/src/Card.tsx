import "./card.css";

export function Card({ title, value, change }: {
  readonly title: string;
  readonly value: string;
  readonly change: string;
}) {
  const isPositive = change.startsWith("+");

  return (
    <div className="card" tabIndex={0}>
      <h3 className="card-title">{title}</h3>
      <p className="card-value">{value}</p>
      <p className={`card-change ${isPositive ? "card-change--positive" : "card-change--negative"}`}>
        {change}
      </p>
    </div>
  );
}

export function CardSkeleton() {
  return (
    <div className="card card--skeleton" aria-hidden="true">
      <div className="skeleton skeleton-title" />
      <div className="skeleton skeleton-value" />
      <div className="skeleton skeleton-change" />
    </div>
  );
}
