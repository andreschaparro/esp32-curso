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

const handleCommandInputClick = (event) => {
  const checkBox = event.currentTarget;
  if (!checkBox || checkBox.type !== "checkbox") {
    handleError("El elemento que genero el evento no es un checkbox.");
    return;
  }
  const isChecked = checkBox.checked ? 1 : 0;
  const channelId = getChannelIdFromElement(checkBox);
  const data = {
    channel: channelId,
    command: isChecked,
  };
  sendPostCommand(data);
};

const getChannelIdFromElement = (element) => {
  return parseInt(element.getAttribute("data-target"));
};

const sendPostCommand = (data) => {
  const xhr = new XMLHttpRequest();
  xhr.open("POST", "api/command", true);
  xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
  xhr.onreadystatechange = () => {
    if (xhr.readyState === XMLHttpRequest.DONE) {
      if (xhr.status === 200) {
        console.log("Solicitud POST exitosa");
      } else {
        handleError("Error en la solicitud POST: " + xhr.status);
      }
    }
  };
  xhr.send(JSON.stringify(data));
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
  const commandInputs = document.querySelectorAll(".switch input");
  commandInputs.forEach((command) => {
    command.addEventListener("click", handleCommandInputClick);
  });
  setInterval(sendGetData, 5000);
};
