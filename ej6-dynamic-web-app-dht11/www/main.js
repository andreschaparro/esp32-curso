const sendGetData = () => {
  const xhr = new XMLHttpRequest();
  xhr.onreadystatechange = () => {
    if (xhr.readyState === XMLHttpRequest.DONE) {
      if (xhr.status === 200) {
        updateWebAppWithResponse(xhr.responseText);
      } else {
        handleError("Error en la solicitud GET: " + xhr.status);
      }
    }
  };
  xhr.open("GET", "api/data", true);
  xhr.send();
};

const updateWebAppWithResponse = (responseText) => {
  try {
    const responseData = JSON.parse(responseText);
    const temp = responseData.temp.toFixed(1);
    const hum = responseData.hum.toFixed(1);
    document.getElementById("temp").textContent = `${temp}°C`;
    document.getElementById("hum").textContent = `${hum}%`;
  } catch (error) {
    handleError(
      "Error al actualizar la aplicación con la respuesta del servidor: " +
        error.message
    );
  }
};

const handleError = (errorMessage) => {
  console.error(errorMessage);
};

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
  setInterval(sendGetData, 5000);
};
