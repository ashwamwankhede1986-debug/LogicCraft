const inputNames = ["A", "B", "C"];
const truthTableBody = document.querySelector("#truth-table-body");
let circuit;
let outputs = {};
let selectedGateId = null;
let eventQueue = [];
let undoHistory = [];
let lastCppAction = { title: "Ready", detail: "Edit a gate or input in Circuit." };
let requestSequence = 0;
let saveQueue = Promise.resolve();

const defaultCircuit = {
  circuitName: "LogicCraft Review-2 Reference Circuit",
  inputs: { A: 1, B: 0, C: 1 },
  gates: [
    { id: "G1", type: "AND", name: "Primary_AND_Gate", inputSources: ["A", "B"] },
    { id: "G2", type: "OR", name: "Secondary_OR_Gate", inputSources: ["G1", "C"] },
    { id: "G3", type: "NOT", name: "Output_Inverter", inputSources: ["G2"] },
  ],
};

function escapeHtml(value) {
  return String(value).replace(/[&<>"']/g, (character) => ({
    "&": "&amp;", "<": "&lt;", ">": "&gt;", "\"": "&quot;", "'": "&#39;",
  })[character]);
}

function setMessage(message, isError = false) {
  const element = document.querySelector("#app-message");
  element.textContent = message;
  element.classList.toggle("is-hidden", !message);
  element.classList.toggle("is-error", isError);
}

async function api(path, options = {}) {
  const response = await fetch(path, {
    headers: { "Content-Type": "application/json" },
    ...options,
  });
  const body = await response.json();
  if (!response.ok) {
    const error = new Error(body.error || body.message || `Backend request failed (${response.status}).`);
    error.status = response.status;
    error.body = body;
    throw error;
  }
  return body;
}

function gatePosition(index) {
  return { x: 170 + index * 185, y: 150 + (index % 2) * 70 };
}

function sourceValue(source) {
  return source.startsWith("G") ? Number(outputs[source] || 0) : Number(circuit.inputs[source]);
}

function wireIsHigh(source) {
  return Boolean(sourceValue(source));
}

function renderCircuit() {
  const width = Math.max(850, 170 + (circuit.gates.length - 1) * 185 + 260);
  const height = 355;
  const svg = document.querySelector(".circuit-diagram");
  svg.setAttribute("viewBox", `0 0 ${width} ${height}`);
  svg.style.width = `${width}px`;

  const gatePositions = new Map(circuit.gates.map((gate, index) => [gate.id, gatePosition(index)]));
  const wires = [];
  const labels = [];

  circuit.gates.forEach((gate) => {
    const position = gatePositions.get(gate.id);
    const centerY = position.y;
    gate.inputSources.forEach((source, inputIndex) => {
      const inputY = centerY + (inputIndex - (gate.inputSources.length - 1) / 2) * 24;
      let path;
      if (source.startsWith("G")) {
        const sourcePosition = gatePositions.get(source);
        const sourceX = sourcePosition.x + 120;
        const middleX = Math.round((sourceX + position.x) / 2);
        path = `M${sourceX} ${sourcePosition.y}H${middleX}V${inputY}H${position.x}`;
      } else {
        path = `M74 ${inputY}H${position.x}`;
        labels.push(`<text x="47" y="${inputY + 4}">${escapeHtml(source)}</text>`);
      }
      wires.push(`<path class="wire${wireIsHigh(source) ? " is-high" : ""}" d="${path}" aria-hidden="true"></path>`);
    });
  });

  const lastGate = circuit.gates[circuit.gates.length - 1];
  const lastPosition = gatePositions.get(lastGate.id);
  const outputX = lastPosition.x + 190;
  wires.push(`<path class="wire${outputs[lastGate.id] ? " is-high" : ""}" d="M${lastPosition.x + 120} ${lastPosition.y}H${outputX - 20}" aria-hidden="true"></path>`);
  document.querySelector("#wire-layer").innerHTML = wires.join("");
  document.querySelector("#input-label-layer").innerHTML = labels.join("");

  document.querySelector("#gate-layer").innerHTML = circuit.gates.map((gate, index) => {
    const position = gatePositions.get(gate.id);
    const centerY = position.y;
    const inputPins = gate.inputSources.map((_, inputIndex) => {
      const inputY = centerY + (inputIndex - (gate.inputSources.length - 1) / 2) * 24;
      return `<circle class="gate-pin" cx="${position.x}" cy="${inputY}" r="4"></circle>`;
    }).join("");
    const selected = gate.id === selectedGateId;
    const output = Number(outputs[gate.id] || 0);
    return `<g class="gate${output ? " is-high" : ""}${selected ? " is-selected" : ""}" data-gate-id="${gate.id}" role="button" tabindex="0" aria-label="Select ${gate.type} gate ${gate.id}" aria-pressed="${selected}">
      <rect x="${position.x}" y="${centerY - 39}" width="120" height="78" rx="12"></rect>
      ${inputPins}<circle class="gate-pin output-pin" cx="${position.x + 120}" cy="${centerY}" r="4"></circle>
      <text class="gate-symbol" x="${position.x + 60}" y="${centerY - 3}">${gate.type}</text>
      <text class="gate-id" x="${position.x + 60}" y="${centerY + 20}">${gate.id}</text>
      <text class="gate-output-value" x="${position.x + 60}" y="${centerY + 55}">OUT ${output}</text>
    </g>`;
  }).join("");

  const outputNode = document.querySelector("#output-node");
  outputNode.setAttribute("transform", `translate(${outputX - 815} ${lastPosition.y - 124})`);
  outputNode.classList.toggle("is-high", Boolean(outputs[lastGate.id]));
  document.querySelector("#diagram-output-value").textContent = String(outputs[lastGate.id] || 0);
  document.querySelector("#circuit-count").textContent = `${circuit.gates.length} gate${circuit.gates.length === 1 ? "" : "s"} · ${circuit.gates.reduce((count, gate) => count + gate.inputSources.filter((source) => source.startsWith("G")).length, 0)} connections`;
  document.querySelector("#circuit-name").textContent = circuit.circuitName;

  document.querySelectorAll(".gate[data-gate-id]").forEach((element) => {
    const select = () => {
      selectedGateId = element.dataset.gateId;
      renderCircuit();
      renderSelectedGate();
    };
    element.addEventListener("click", select);
    element.addEventListener("keydown", (event) => {
      if (event.key === "Enter" || event.key === " ") {
        event.preventDefault();
        select();
      }
    });
  });
  renderResults();
  renderSelectedGate();
}

function renderResults() {
  const lastGate = circuit.gates[circuit.gates.length - 1];
  const output = Number(outputs[lastGate.id] || 0);
  for (const name of inputNames) {
    const toggle = document.querySelector(`[data-input="${name}"]`);
    toggle.setAttribute("aria-checked", String(Boolean(circuit.inputs[name])));
    toggle.querySelector("b").textContent = String(circuit.inputs[name]);
  }

  document.querySelector("#output-result").textContent = String(output);
  document.querySelector("#output-state").textContent = output ? "HIGH" : "LOW";
  document.querySelector("#output-lamp").classList.toggle("is-high", Boolean(output));
  document.querySelector(".output-source").textContent = `${lastGate.id} · ${lastGate.type}`;
  document.querySelector(".expression").textContent = `OUTPUT = ${lastGate.id} (${lastGate.type})`;
  inputNames.forEach((name) => {
    const consumers = circuit.gates.filter((gate) => gate.inputSources.includes(name)).map((gate) => gate.id);
    document.querySelector(`[data-input="${name}"]`).closest(".input-row").querySelector("small").textContent =
      consumers.length ? `CONNECTED · ${consumers.join(", ")}` : "UNCONNECTED";
  });

  document.querySelector(".gate-results").innerHTML = circuit.gates.map((gate, index) => `
    ${index ? '<span class="result-arrow" aria-hidden="true">→</span>' : ""}
    <button type="button" class="gate-result${gate.id === selectedGateId ? " is-current" : ""}" data-select-gate="${gate.id}">
      <span class="result-index">${String(index + 1).padStart(2, "0")}</span>
      <span><strong>${gate.type}</strong><small>${gate.id} · ${gate.inputSources.join(" → ")}</small></span>
      <span class="result-value${outputs[gate.id] ? " is-high" : ""}">${Number(outputs[gate.id] || 0)}</span>
    </button>`).join("");
  document.querySelectorAll("[data-select-gate]").forEach((button) => {
    button.addEventListener("click", () => {
      selectedGateId = button.dataset.selectGate;
      renderCircuit();
    });
  });

  renderTruthTable();
  renderCppDemo();
}

function recordUndo(action) {
  if (!circuit) return;
  undoHistory.push({ action, circuit: structuredClone(circuit), selectedGateId });
  if (undoHistory.length > 32) undoHistory.shift();
  lastCppAction = { title: action, detail: "Recorded on the circuit undo stack." };
  renderCppDemo();
}

function renderCppDemo() {
  const depth = document.querySelector("#undo-depth");
  if (!depth) return;
  depth.innerHTML = `${undoHistory.length} <small>/ 32</small>`;
  document.querySelector("#undo-circuit").disabled = undoHistory.length === 0;
  document.querySelector("#undo-demo-button").disabled = undoHistory.length === 0;
  document.querySelector("#undo-history").innerHTML = undoHistory.length
    ? undoHistory.slice().reverse().map((entry, index) => `<li><span>${String(index + 1).padStart(2, "0")}</span><strong>${escapeHtml(entry.action)}</strong><small>${entry.circuit.gates.length} gates</small></li>`).join("")
    : '<li class="empty-list">No circuit actions yet. Edit a gate to add an undo entry.</li>';
  document.querySelector("#queue-count").innerHTML = `${eventQueue.length} <small>events</small>`;
  document.querySelector("#event-queue").innerHTML = eventQueue.length
    ? eventQueue.map((event) => `<li><span>${String(event.step).padStart(2, "0")}</span><strong>${escapeHtml(event.description)}</strong><small>G${event.targetGateId.slice(1)}</small></li>`).join("")
    : '<li class="empty-list">Run the circuit simulation to see FIFO events.</li>';
  if (eventQueue.length && lastCppAction.title === "Ready") {
    const last = eventQueue[eventQueue.length - 1];
    lastCppAction = { title: `Event ${last.eventId}`, detail: last.description };
  }
  document.querySelector("#last-cpp-action").textContent = lastCppAction.title;
  document.querySelector("#last-cpp-action-detail").textContent = lastCppAction.detail;
}

async function undoCircuit() {
  const entry = undoHistory.pop();
  if (!entry) return;
  circuit = entry.circuit;
  selectedGateId = entry.selectedGateId;
  const sequence = ++requestSequence;
  setMessage(`Undoing: ${entry.action}`);
  renderCircuit();
  saveQueue = saveQueue.catch(() => {}).then(async () => {
    const result = await api("/api/circuit", { method: "PUT", body: JSON.stringify(circuit) });
    if (sequence === requestSequence) {
      applySimulation(result);
      lastCppAction = { title: "Undo", detail: entry.action };
      renderCppDemo();
    }
  }).catch((error) => {
    if (sequence === requestSequence) {
      undoHistory.push(entry);
      renderCppDemo();
      setMessage(error.message, true);
    }
  });
}

function renderSelectedGate() {
  const panel = document.querySelector("#selected-gate-panel");
  const gateIndex = circuit.gates.findIndex((gate) => gate.id === selectedGateId);
  if (gateIndex < 0) {
    panel.innerHTML = '<p class="selection-hint">Select a gate on the canvas to edit its connections.</p>';
    return;
  }
  const gate = circuit.gates[gateIndex];
  const availableSources = [
    ...inputNames.map((name) => ({ id: name, label: `Input ${name}` })),
    ...circuit.gates.slice(0, gateIndex).map((sourceGate) => ({ id: sourceGate.id, label: `${sourceGate.id} · ${sourceGate.type} output` })),
  ];
  panel.innerHTML = `<div class="selected-gate-heading"><div><span class="panel-kicker">SELECTED GATE</span><h3>${gate.id} · ${gate.type}</h3></div>
    <button class="delete-gate-button" id="delete-gate-button" type="button" aria-label="Delete gate ${gate.id}">Delete</button></div>
    <div class="connection-fields">${gate.inputSources.map((source, index) => `
      <label class="connection-field"><span>Input ${index + 1}</span>
        <select data-gate-input="${gate.id}" data-input-index="${index}" aria-label="${gate.id} input ${index + 1}">
          ${availableSources.map((item) => `<option value="${item.id}"${item.id === source ? " selected" : ""}>${item.label}</option>`).join("")}
        </select>
      </label>`).join("")}
    </div>`;

  panel.querySelectorAll("[data-gate-input]").forEach((select) => {
    select.addEventListener("change", () => {
      const targetGate = circuit.gates.find((item) => item.id === select.dataset.gateInput);
      recordUndo(`Connect ${targetGate.id} input ${Number(select.dataset.inputIndex) + 1}`);
      targetGate.inputSources[Number(select.dataset.inputIndex)] = select.value;
      saveCircuit();
    });
  });
  panel.querySelector("#delete-gate-button").addEventListener("click", () => {
    if (circuit.gates.length === 1) {
      setMessage("Keep at least one gate in the circuit.", true);
      return;
    }
    recordUndo(`Delete ${gate.id} (${gate.type})`);
    circuit.gates = circuit.gates.filter((item) => item.id !== gate.id);
    circuit.gates.forEach((item) => {
      item.inputSources = item.inputSources.map((source) => source === gate.id ? "A" : source);
    });
    selectedGateId = circuit.gates[Math.max(0, gateIndex - 1)].id;
    saveCircuit();
  });
}

function renderTruthTable(truthTable = []) {
  const head = document.querySelector("#truth-table-head");
  head.innerHTML = `<tr><th>A</th><th>B</th><th>C</th>${circuit.gates.map((gate) => `<th>${gate.id} · ${gate.type}</th>`).join("")}<th class="output-column">OUTPUT</th></tr>`;
  if (!truthTable.length) return;
  truthTableBody.innerHTML = truthTable.map((row) => {
    const isCurrent = inputNames.every((name) => row.inputs[name] === circuit.inputs[name]);
    const values = circuit.gates.map((gate) => Number(row.outputs[gate.id]));
    return `<tr class="${isCurrent ? "is-current" : ""}"${isCurrent ? ' aria-current="true"' : ""}>
      <td>${row.inputs.A}</td><td>${row.inputs.B}</td><td>${row.inputs.C}</td>
      ${values.map((value) => `<td>${value}</td>`).join("")}
      <td class="output-column">${row.output}</td>
    </tr>`;
  }).join("");
}

function applySimulation(result) {
  circuit = result.circuit;
  outputs = result.outputs;
  eventQueue = result.eventQueue || [];
  renderCircuit();
  renderTruthTable(result.truthTable || []);
  setMessage("");
}

async function simulateCircuit(sequence) {
  const result = await api("/api/simulate", {
    method: "POST",
    body: JSON.stringify({ ...circuit, includeTruthTable: true }),
  });
  if (sequence === requestSequence) applySimulation(result);
}

function saveCircuit() {
  if (!circuit) return;
  const sequence = ++requestSequence;
  setMessage("Saving circuit…");
  renderCircuit();
  saveQueue = saveQueue.catch(() => {}).then(async () => {
    const result = await api("/api/circuit", {
      method: "PUT",
      body: JSON.stringify(circuit),
    });
    if (sequence === requestSequence) {
      applySimulation(result);
      lastCppAction = { title: "Circuit saved", detail: "Simulation events processed in FIFO order." };
      renderCppDemo();
      setMessage("Circuit saved");
      window.setTimeout(() => {
        if (sequence === requestSequence) setMessage("");
      }, 1800);
    }
  }).catch((error) => {
    if (sequence === requestSequence) setMessage(error.message, true);
  });
  renderCppDemo();
}

function addGate(type) {
  if (circuit.gates.length >= 24) {
    setMessage("A circuit can contain up to 24 gates.", true);
    return;
  }
  const nextId = Math.max(0, ...circuit.gates.map((gate) => Number(gate.id.slice(1)) || 0)) + 1;
  const id = `G${nextId}`;
  const inputSources = type === "NOT" ? ["A"] : ["A", "B"];
  recordUndo(`Add ${type} gate ${id}`);
  circuit.gates.push({ id, type, name: `${type}_Gate_${nextId}`, inputSources });
  selectedGateId = id;
  saveCircuit();
}

document.querySelectorAll(".signal-toggle").forEach((toggle) => {
  toggle.addEventListener("click", () => {
    const name = toggle.dataset.input;
    recordUndo(`Toggle input ${name}`);
    circuit.inputs[name] = Number(!circuit.inputs[name]);
    saveCircuit();
  });
});

document.querySelectorAll("[data-add-gate]").forEach((button) => {
  button.addEventListener("click", () => addGate(button.dataset.addGate));
});

document.querySelector("#reset-button").addEventListener("click", async () => {
  recordUndo("Reset circuit");
  const sequence = ++requestSequence;
  circuit = structuredClone(defaultCircuit);
  selectedGateId = null;
  saveQueue = saveQueue.catch(() => {}).then(async () => {
    const result = await api("/api/circuit", { method: "PUT", body: JSON.stringify(circuit) });
    if (sequence === requestSequence) applySimulation(result);
  }).catch((error) => {
    if (sequence === requestSequence) setMessage(error.message, true);
  });
});

document.querySelector("#undo-circuit").addEventListener("click", undoCircuit);
document.querySelector("#undo-demo-button").addEventListener("click", undoCircuit);

document.querySelectorAll(".nav-item").forEach((button) => {
  button.addEventListener("click", () => {
    const showTruthTable = button.dataset.view === "truth-table";
    const showTask3 = button.dataset.view === "task3";
    document.querySelector("#circuit-view").classList.toggle("is-hidden", showTruthTable || showTask3);
    document.querySelector("#truth-view").classList.toggle("is-hidden", !showTruthTable);
    document.querySelector("#task3-view").classList.toggle("is-hidden", !showTask3);
    document.querySelectorAll(".nav-item").forEach((item) => {
      item.classList.toggle("is-active", item === button);
      if (item === button) item.setAttribute("aria-current", "page");
      else item.removeAttribute("aria-current");
    });
    const viewName = showTask3 ? "Task 3" : showTruthTable ? "Truth table" : "Circuit";
    document.querySelector("#breadcrumb-current").textContent = viewName;
    document.querySelector("#page-title").textContent = showTask3 ? "Task 3" : showTruthTable ? "Truth table" : "Circuit lab";
    document.querySelector("#page-description").textContent = showTruthTable
      ? "Every input combination for the current circuit."
      : showTask3
        ? "Explore the course outcome demos integrated from the LogicCraft modules."
        : "Add gates, connect their inputs, and simulate your circuit.";
    document.querySelector("#reset-button").classList.toggle("is-hidden", showTruthTable || showTask3);
  });
});

document.querySelectorAll(".task-module-tab").forEach((button) => {
  button.addEventListener("click", () => {
    document.querySelectorAll(".task-module-tab").forEach((tab) => tab.classList.toggle("is-active", tab === button));
    document.querySelectorAll(".task-module-panel").forEach((panel) => panel.classList.toggle("is-hidden", panel.dataset.panel !== button.dataset.module));
  });
});

function showJavaValidation(result, isError = false) {
  const output = document.querySelector("#java-validation-result");
  output.classList.toggle("is-error", isError);
  output.textContent = result.exception
    ? `${result.exception}: ${result.message}`
    : result.message;
}

document.querySelector("#java-validation-form").addEventListener("submit", async (event) => {
  event.preventDefault();
  try {
    const result = await api("/api/task3/java/validate", {
      method: "POST",
      body: JSON.stringify({
        gateType: document.querySelector("#java-gate-type").value,
        inputCount: Number(document.querySelector("#java-input-count").value),
      }),
    });
    showJavaValidation(result);
  } catch (error) {
    showJavaValidation(error.body || { exception: "InvalidInputException", message: error.message }, true);
  }
});

function renderJavaComponents(components) {
  document.querySelector("#java-component-count").textContent = `${components.length} / 32`;
  document.querySelector("#java-component-list").innerHTML = components.length
    ? components.map((component) => `<li><span class="component-index">${String(components.indexOf(component) + 1).padStart(2, "0")}</span><strong>${escapeHtml(component.type)}Gate</strong><button type="button" data-remove-component="${escapeHtml(component.id)}" aria-label="Remove ${escapeHtml(component.type)} component">Remove</button></li>`).join("")
    : '<li class="empty-list">The generic collection is empty.</li>';
  document.querySelectorAll("[data-remove-component]").forEach((button) => {
    button.addEventListener("click", () => updateJavaComponents({ action: "remove", id: button.dataset.removeComponent }));
  });
}

async function updateJavaComponents(payload) {
  try {
    const result = await api("/api/task3/java/components", { method: "POST", body: JSON.stringify(payload) });
    renderJavaComponents(result.components);
  } catch (error) {
    setMessage(error.message, true);
  }
}

document.querySelector("#java-add-component").addEventListener("click", () => {
  updateJavaComponents({ action: "add", type: document.querySelector("#java-component-type").value });
});
document.querySelector("#java-clear-components").addEventListener("click", () => updateJavaComponents({ action: "clear" }));

const coaAction = document.querySelector("#coa-action");
function renderCoaFields() {
  const action = coaAction.value;
  const fields = document.querySelector("#coa-arguments");
  if (action === "multiply") {
    fields.innerHTML = '<label>Multiplicand<input name="multiplicand" type="number" min="0" value="15" required></label><label>Multiplier<input name="multiplier" type="number" min="0" max="100000" value="8" required></label>';
  } else if (action === "truth-rows") {
    fields.innerHTML = '<label>Number of inputs<input name="inputs" type="number" min="0" max="63" value="3" required></label>';
  } else if (action === "parse-gate") {
    fields.innerHTML = '<label>Gate name<input name="value" type="text" maxlength="24" value="XOR" required></label>';
  } else if (action === "validate-command") {
    fields.innerHTML = '<label>Control command<input name="value" type="text" maxlength="32" value="SIMULATE" required></label>';
  } else {
    fields.innerHTML = '<label>Boolean expression<input name="value" type="text" maxlength="32" value="1 ^ 0" required></label>';
  }
}
coaAction.addEventListener("change", renderCoaFields);

document.querySelector("#coa-form").addEventListener("submit", async (event) => {
  event.preventDefault();
  const form = new FormData(event.currentTarget);
  const payload = { action: coaAction.value };
  for (const key of ["multiplicand", "multiplier", "inputs"]) {
    if (form.has(key)) payload[key] = Number(form.get(key));
  }
  if (form.has("value")) payload.value = form.get("value");
  const resultElement = document.querySelector("#coa-result");
  try {
    const result = await api("/api/task3/coa", { method: "POST", body: JSON.stringify(payload) });
    resultElement.classList.remove("is-error");
    if (result.action === "multiply") {
      resultElement.textContent = `${result.multiplicand} × ${result.multiplier} = ${result.result} · ${result.steps} repeated additions`;
    } else if (result.action === "truth-rows") {
      resultElement.textContent = `2^${result.inputs} = ${result.rows} truth-table rows · successive doubling`;
    } else if (result.action === "parse-gate") {
      resultElement.textContent = `${result.value || "(empty)"} → opcode ${result.opcode} (${result.decoded})`;
    } else if (result.action === "validate-command") {
      resultElement.textContent = `${result.value || "(empty)"} → command ID ${result.commandId} · ${result.valid ? "valid" : "unknown command"}`;
    } else {
      resultElement.textContent = result.malformed ? `${result.value || "(empty)"} → error code 255 (malformed expression)` : `${result.value} = ${result.result}`;
    }
  } catch (error) {
    resultElement.classList.add("is-error");
    resultElement.textContent = error.message;
  }
});

async function loadGraphicsSettings() {
  try {
    const settings = await api("/api/task3/graphics");
    document.querySelector("#graphics-theme").value = settings.theme;
    document.querySelector("#graphics-textures").checked = settings.textures;
    document.querySelector("#graphics-lighting").checked = settings.lighting;
    applyGraphicsPreview(settings);
  } catch (error) {
    setMessage(error.message, true);
  }
}

function applyGraphicsPreview(settings) {
  const preview = document.querySelector("#graphics-preview");
  preview.dataset.theme = settings.theme;
  preview.classList.toggle("no-texture", !settings.textures);
  preview.classList.toggle("no-lighting", !settings.lighting);
  document.querySelector("#graphics-state").textContent = `${settings.textures ? "TEXTURED" : "FLAT"} · ${settings.lighting ? "LIT" : "UNLIT"}`;
  document.querySelector(".canvas-panel").dataset.theme = settings.theme;
  document.querySelector(".canvas-panel").classList.toggle("no-texture", !settings.textures);
  document.querySelector(".canvas-panel").classList.toggle("no-lighting", !settings.lighting);
}

document.querySelector("#graphics-apply").addEventListener("click", async () => {
  const settings = {
    theme: document.querySelector("#graphics-theme").value,
    textures: document.querySelector("#graphics-textures").checked,
    lighting: document.querySelector("#graphics-lighting").checked,
  };
  try {
    applyGraphicsPreview(await api("/api/task3/graphics", { method: "PUT", body: JSON.stringify(settings) }));
    setMessage("Graphics settings applied.", false);
    window.setTimeout(() => setMessage(""), 1600);
  } catch (error) {
    setMessage(error.message, true);
  }
});

async function startApp() {
  try {
    circuit = await api("/api/circuit");
    selectedGateId = circuit.gates[circuit.gates.length - 1].id;
    document.querySelector("#backend-status").innerHTML = "<span></span> Backend connected";
    document.querySelector("#simulation-status").innerHTML = "<span></span> Simulation active";
    await simulateCircuit(++requestSequence);
    const [components] = await Promise.all([
      api("/api/task3/java/components"),
      loadGraphicsSettings(),
    ]);
    renderJavaComponents(components.components);
  } catch (error) {
    document.querySelector("#backend-status").innerHTML = "<span></span> Backend offline";
    document.querySelector("#simulation-status").innerHTML = "<span></span> Backend unavailable";
    setMessage(`${error.message} Start the app with run_web.bat.`, true);
  }
}

renderCoaFields();
startApp();
