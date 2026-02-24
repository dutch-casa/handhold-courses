let titleText = document.getElementById("title").textContent;

let paragraphCount = document.querySelectorAll("p").length;

document.getElementById("featured").classList.add("highlight");

const newParagraph = document.createElement("p");
newParagraph.textContent = "Added by JavaScript";
document.querySelector("main").appendChild(newParagraph);

if (typeof module !== "undefined") {
  module.exports = { titleText, paragraphCount };
}
