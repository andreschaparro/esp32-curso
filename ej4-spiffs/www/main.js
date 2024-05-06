const handleNavLinkClick = (event) => {
  event.preventDefault();
  const containerId = event.currentTarget.getAttribute("data-target");
  showContainer(containerId);
};

const showContainer = (containerId) => {
  const containers = document.querySelectorAll(".container-to-control");
  containers.forEach((container) => {
    container.style.display = container.id === containerId ? "block" : "none";
  });
};

window.onload = () => {
  showContainer("home");
  const navLinks = document.querySelectorAll("#nav-mobile a");
  navLinks.forEach((link) => {
    link.addEventListener("click", handleNavLinkClick);
  });
};
