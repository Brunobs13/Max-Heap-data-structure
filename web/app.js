const nodes = {
  status: document.getElementById("status"),
  sizeCapacity: document.getElementById("sizeCapacity"),
  currentMax: document.getElementById("currentMax"),
  feedback: document.getElementById("feedback"),
  heapArray: document.getElementById("heapArray"),
  heapTree: document.getElementById("heapTree"),
  metrics: document.getElementById("metrics"),
  insertInput: document.getElementById("insertInput"),
  buildInput: document.getElementById("buildInput"),
  resizeInput: document.getElementById("resizeInput"),
  insertBtn: document.getElementById("insertBtn"),
  buildBtn: document.getElementById("buildBtn"),
  resizeBtn: document.getElementById("resizeBtn"),
  deleteBtn: document.getElementById("deleteBtn"),
  clearBtn: document.getElementById("clearBtn"),
  refreshBtn: document.getElementById("refreshBtn"),
};

function parseIntegerList(raw) {
  if (!raw.trim()) {
    return [];
  }

  return raw
    .split(",")
    .map((item) => item.trim())
    .filter(Boolean)
    .map((item) => Number(item))
    .filter((item) => Number.isFinite(item));
}

function setFeedback(message, error = false) {
  nodes.feedback.textContent = message;
  nodes.feedback.style.color = error ? "#ffab9a" : "#8fe1af";
}

async function request(path, options = {}) {
  const response = await fetch(path, {
    headers: { "Content-Type": "application/json" },
    ...options,
  });

  const body = await response.json().catch(() => ({}));
  if (!response.ok) {
    throw new Error(body.detail || "request failed");
  }

  return body;
}

function renderState(state) {
  nodes.sizeCapacity.textContent = `${state.size} / ${state.capacity}`;
  nodes.currentMax.textContent = state.max === null ? "-" : String(state.max);

  nodes.heapArray.innerHTML = "";
  if (!state.array.length) {
    const placeholder = document.createElement("div");
    placeholder.className = "array-node";
    placeholder.textContent = "empty";
    nodes.heapArray.appendChild(placeholder);
  } else {
    state.array.forEach((value) => {
      const item = document.createElement("div");
      item.className = "array-node";
      item.textContent = String(value);
      nodes.heapArray.appendChild(item);
    });
  }

  nodes.heapTree.innerHTML = "";
  if (!state.levels.length) {
    const level = document.createElement("div");
    level.className = "level";
    level.innerHTML = '<div class="level-node">-</div>';
    nodes.heapTree.appendChild(level);
  } else {
    state.levels.forEach((values) => {
      const level = document.createElement("div");
      level.className = "level";
      values.forEach((value) => {
        const item = document.createElement("div");
        item.className = "level-node";
        item.textContent = String(value);
        level.appendChild(item);
      });
      nodes.heapTree.appendChild(level);
    });
  }
}

function renderMetrics(payload) {
  const keys = [
    ["requests_total", "Requests"],
    ["insert_ops", "Insert Ops"],
    ["delete_ops", "Delete Ops"],
    ["build_ops", "Build Ops"],
    ["resize_ops", "Resize Ops"],
    ["clear_ops", "Clear Ops"],
    ["errors_total", "Errors"],
  ];

  nodes.metrics.innerHTML = "";
  keys.forEach(([key, label]) => {
    const card = document.createElement("div");
    card.className = "metric-card";
    card.innerHTML = `<span>${label}</span><strong>${payload[key] ?? 0}</strong>`;
    nodes.metrics.appendChild(card);
  });
}

async function refreshAll() {
  try {
    const [state, metrics] = await Promise.all([
      request("/api/heap/state"),
      request("/api/metrics"),
    ]);

    renderState(state);
    renderMetrics(metrics);
    nodes.status.textContent = "online";
  } catch (error) {
    nodes.status.textContent = "offline";
    setFeedback(error.message || "failed to refresh", true);
  }
}

async function handleInsert() {
  const values = parseIntegerList(nodes.insertInput.value);
  if (!values.length) {
    setFeedback("Insert requires at least one valid integer.", true);
    return;
  }

  try {
    const response = await request("/api/heap/insert", {
      method: "POST",
      body: JSON.stringify({ values }),
    });

    renderState(response.state);
    await refreshAll();
    setFeedback(response.message);
  } catch (error) {
    setFeedback(error.message, true);
  }
}

async function handleBuild() {
  const values = parseIntegerList(nodes.buildInput.value);
  try {
    const response = await request("/api/heap/build", {
      method: "POST",
      body: JSON.stringify({ values }),
    });

    renderState(response.state);
    await refreshAll();
    setFeedback(response.message);
  } catch (error) {
    setFeedback(error.message, true);
  }
}

async function handleResize() {
  const capacity = Number(nodes.resizeInput.value);
  if (!Number.isInteger(capacity) || capacity <= 0) {
    setFeedback("Resize requires a positive integer.", true);
    return;
  }

  try {
    const response = await request("/api/heap/resize", {
      method: "POST",
      body: JSON.stringify({ capacity }),
    });

    renderState(response.state);
    await refreshAll();
    setFeedback(response.message);
  } catch (error) {
    setFeedback(error.message, true);
  }
}

async function handleDelete() {
  try {
    const response = await request("/api/heap/delete-max", { method: "POST" });
    renderState(response.state);
    await refreshAll();
    setFeedback(`${response.message}. removed=${response.removed}`);
  } catch (error) {
    setFeedback(error.message, true);
  }
}

async function handleClear() {
  try {
    const response = await request("/api/heap/clear", { method: "POST" });
    renderState(response.state);
    await refreshAll();
    setFeedback(response.message);
  } catch (error) {
    setFeedback(error.message, true);
  }
}

nodes.insertBtn.addEventListener("click", handleInsert);
nodes.buildBtn.addEventListener("click", handleBuild);
nodes.resizeBtn.addEventListener("click", handleResize);
nodes.deleteBtn.addEventListener("click", handleDelete);
nodes.clearBtn.addEventListener("click", handleClear);
nodes.refreshBtn.addEventListener("click", refreshAll);

refreshAll();
setInterval(refreshAll, 5000);
