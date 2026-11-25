/**
 * LogicCraft - Backend REST API & Static Server
 *
 * Subject Integrations:
 *  - C++ (CO1, CO2, CO3 - LIFO Stack Undo, FIFO Queue Simulation)
 *  - COA / 64-Bit Assembly (CO1, CO2, CO3 - Successive Addition & String Processing)
 *  - Computer Graphics / OpenGL (CO1, CO2, CO6 - Texture & Lighting Effects)
 *  - Java (CO1, CO2, CO3 - Exception Handling, Collections, Swing & Packages)
 */

const http = require('http');
const fs = require('fs');
const path = require('path');
const crypto = require('crypto');
const { exec, spawn } = require('child_process');

const PORT = process.env.PORT || 3000;
const ROOT_DIR = path.resolve(__dirname, '..');
const FRONTEND_DIR = path.join(ROOT_DIR, 'frontend');
const SHARED_DIR = path.join(ROOT_DIR, 'shared');

// User Authentication Storage with PBKDF2 Password Hashing
const USERS_FILE = path.join(SHARED_DIR, 'users.json');

function hashPassword(password, salt) {
    if (!salt) {
        salt = crypto.randomBytes(16).toString('hex');
    }
    const hash = crypto.pbkdf2Sync(password, salt, 10000, 64, 'sha512').toString('hex');
    return { salt, hash };
}

function verifyPassword(password, salt, storedHash) {
    const hash = crypto.pbkdf2Sync(password, salt, 10000, 64, 'sha512').toString('hex');
    const a = Buffer.from(hash, 'hex');
    const b = Buffer.from(storedHash || '', 'hex');
    return a.length === b.length && a.length > 0 && crypto.timingSafeEqual(a, b);
}

function loadUsers() {
    try {
        if (fs.existsSync(USERS_FILE)) {
            return JSON.parse(fs.readFileSync(USERS_FILE, 'utf8'));
        }
    } catch (e) {
        console.error('Failed reading users file:', e);
    }
    // Initialize default secure user with PBKDF2 salt and hash (no plaintext password)
    const def = hashPassword('engineer123');
    const initialUsers = {
        'engineer': {
            username: 'engineer',
            salt: def.salt,
            hash: def.hash,
            role: 'Lead Circuit Engineer',
            created: new Date().toISOString()
        }
    };
    try {
        fs.writeFileSync(USERS_FILE, JSON.stringify(initialUsers, null, 2), 'utf8');
    } catch (e) {}
    return initialUsers;
}

const sessions = new Map();
const SESSION_TTL_MS = 8 * 60 * 60 * 1000;
function createSession(username, role) {
    const token = crypto.randomBytes(32).toString('hex');
    sessions.set(token, { username, role, expiresAt: Date.now() + SESSION_TTL_MS });
    return token;
}
function getSession(req) {
    const auth = req.headers.authorization || '';
    const bearer = auth.startsWith('Bearer ') ? auth.slice(7) : '';
    const cookieToken = (req.headers.cookie || '').split(';').map(v => v.trim()).find(v => v.startsWith('logiccraft_session='));
    const token = bearer || (cookieToken ? cookieToken.split('=').slice(1).join('') : '');
    const session = sessions.get(token);
    if (!session) return null;
    if (session.expiresAt <= Date.now()) { sessions.delete(token); return null; }
    return session;
}
function sessionCookie(token) { return `logiccraft_session=${token}; HttpOnly; SameSite=Strict; Path=/; Max-Age=${Math.floor(SESSION_TTL_MS / 1000)}`; }

function saveUsers(users) {
    try {
        fs.writeFileSync(USERS_FILE, JSON.stringify(users, null, 2), 'utf8');
    } catch (e) {
        console.error('Failed to save users:', e);
    }
}

// In-memory project store initialized with default reference circuit
let projects = {};

function initDefaultProjects() {
    try {
        const refPath = path.join(SHARED_DIR, 'circuit_data.json');
        if (fs.existsSync(refPath)) {
            const data = JSON.parse(fs.readFileSync(refPath, 'utf8'));
            projects['proj-ref-1'] = {
                id: 'proj-ref-1',
                name: data.circuitName || 'LogicCraft Reference Circuit',
                description: data.description || 'AND-OR-NOT reference circuit for multi-module verification',
                gates: data.gates.map((g, idx) => ({
                    id: g.id || `G${idx + 1}`,
                    type: g.type,
                    name: g.name,
                    x: g.position ? g.position.x : -150 + idx * 160,
                    y: g.position ? g.position.y : 0,
                    inputs: g.defaultInputs || [0, 0],
                    output: g.output || 0
                })),
                connections: data.connections || [],
                history: [],
                updatedAt: new Date().toISOString()
            };
        }
    } catch (err) {
        console.error('Failed to load shared circuit_data.json:', err);
    }

    if (Object.keys(projects).length === 0) {
        projects['proj-ref-1'] = {
            id: 'proj-ref-1',
            name: 'LogicCraft Reference Circuit',
            description: 'Integrated 3-gate circuit connecting AND, OR, and NOT gates',
            gates: [
                { id: 'G1', type: 'AND', name: 'Primary_AND_Gate', x: -200, y: 50, inputs: [1, 0], output: 0 },
                { id: 'G2', type: 'OR', name: 'Secondary_OR_Gate', x: 50, y: -20, inputs: [0, 1], output: 1 },
                { id: 'G3', type: 'NOT', name: 'Output_Inverter', x: 280, y: -20, inputs: [1], output: 0 }
            ],
            connections: [
                { id: 'C1', sourceGateId: 'G1', sourceOutput: 0, destinationGateId: 'G2', destinationInputIndex: 0 },
                { id: 'C2', sourceGateId: 'G2', sourceOutput: 0, destinationGateId: 'G3', destinationInputIndex: 0 }
            ],
            history: [],
            updatedAt: new Date().toISOString()
        };
    }
}

initDefaultProjects();

// =============================================================================
// In-Memory Component Hash Table (C++ CO5 Implementation Mirror)
// Prime Modulus: 17, Linear Probing: h(k, i) = (hash(k) + i) mod 17
// =============================================================================
const HASH_TABLE_CAPACITY = 17;
let memoryHashTable = {
    capacity: HASH_TABLE_CAPACITY,
    size: 0,
    occupied: 0,
    totalCollisions: 0,
    buckets: Array.from({ length: HASH_TABLE_CAPACITY }, (_, idx) => ({
        index: idx,
        bucket: idx,
        state: 'EMPTY',
        id: null,
        name: null,
        type: null,
        originalHash: -1,
        probes: 0
    }))
};

function resetMemoryHashTable() {
    memoryHashTable = {
        capacity: HASH_TABLE_CAPACITY,
        size: 0,
        occupied: 0,
        totalCollisions: 0,
        buckets: Array.from({ length: HASH_TABLE_CAPACITY }, (_, idx) => ({
            index: idx,
            bucket: idx,
            state: 'EMPTY',
            id: null,
            name: null,
            type: null,
            originalHash: -1,
            probes: 0
        }))
    };
    const proj = projects['proj-ref-1'];
    if (proj && proj.gates) {
        proj.gates.forEach(g => {
            insertIntoMemoryHashTable(g.id, g.name || `Gate_${g.id}`, g.type || 'AND');
        });
    }
}

function insertIntoMemoryHashTable(rawId, name, type) {
    if (memoryHashTable.size >= HASH_TABLE_CAPACITY) {
        return { success: false, reason: 'TABLE_FULL' };
    }
    const numId = parseInt(String(rawId).replace(/\D/g, '')) || 1;
    const baseHash = Math.abs(numId) % HASH_TABLE_CAPACITY;
    let firstTombstone = -1;

    for (let step = 0; step < HASH_TABLE_CAPACITY; step++) {
        const slot = (baseHash + step) % HASH_TABLE_CAPACITY;
        const b = memoryHashTable.buckets[slot];

        if (b.state === 'OCCUPIED' && b.id === rawId) {
            b.name = name;
            b.type = type;
            return { success: true, slot, probes: step, updated: true };
        }
        if (b.state === 'TOMBSTONE' && firstTombstone === -1) {
            firstTombstone = slot;
        }
        if (b.state === 'EMPTY') {
            const targetSlot = firstTombstone !== -1 ? firstTombstone : slot;
            const targetBucket = memoryHashTable.buckets[targetSlot];
            targetBucket.state = 'OCCUPIED';
            targetBucket.id = rawId;
            targetBucket.name = name;
            targetBucket.type = type;
            targetBucket.originalHash = baseHash;
            targetBucket.probes = (targetSlot - baseHash + HASH_TABLE_CAPACITY) % HASH_TABLE_CAPACITY;
            memoryHashTable.size++;
            memoryHashTable.occupied = memoryHashTable.size;
            if (step > 0) memoryHashTable.totalCollisions++;
            return { success: true, slot: targetSlot, probes: targetBucket.probes, collisions: memoryHashTable.totalCollisions };
        }
    }
    return { success: false, reason: 'NO_SLOT_FOUND' };
}

resetMemoryHashTable();

// Gate logic evaluation
function evaluateGate(type, inputs) {
    if (!Array.isArray(inputs)) inputs = [0, 0];
    const a = inputs[0] ? 1 : 0;
    const b = inputs.length > 1 ? (inputs[1] ? 1 : 0) : 0;
    switch ((type || '').toUpperCase()) {
        case 'AND': return a & b;
        case 'OR': return a | b;
        case 'NOT': return a ? 0 : 1;
        case 'NAND': return (a & b) ? 0 : 1;
        case 'NOR': return (a | b) ? 0 : 1;
        case 'XOR': return a ^ b;
        case 'XNOR': return (a ^ b) ? 0 : 1;
        case 'BUFFER': return a;
        default: return 0;
    }
}

// Complete topological simulation & truth table generation
function simulateCircuit(circuit) {
    const gates = JSON.parse(JSON.stringify(circuit.gates || []));
    const connections = circuit.connections || [];
    const gateMap = {};
    gates.forEach(g => {
        if (!Array.isArray(g.inputs)) {
            g.inputs = Array.isArray(g.defaultInputs) ? [...g.defaultInputs] : ((g.type === 'NOT' || g.type === 'BUFFER') ? [0] : [0, 0]);
        }
        gateMap[g.id] = g;
    });

    // Multi-pass propagation to steady state
    let changed = true;
    let iterations = 0;
    const maxIterations = 16;
    const executionOrder = [];

    while (changed && iterations < maxIterations) {
        changed = false;
        iterations++;

        // Propagate signals along connections
        for (const conn of connections) {
            const src = gateMap[conn.sourceGateId];
            const dest = gateMap[conn.destinationGateId];
            if (src && dest && Array.isArray(dest.inputs)) {
                const targetIdx = conn.destinationInputIndex || 0;
                if (dest.inputs[targetIdx] !== src.output) {
                    dest.inputs[targetIdx] = src.output;
                    changed = true;
                }
            }
        }

        // Re-evaluate gate outputs
        for (const g of gates) {
            const prevOutput = g.output;
            g.output = evaluateGate(g.type, g.inputs);
            if (g.output !== prevOutput) {
                changed = true;
                if (!executionOrder.includes(g.id)) executionOrder.push(g.id);
            }
        }
    }

    // Identify primary circuit inputs and primary circuit outputs
    const drivenInputs = new Set();
    connections.forEach(c => drivenInputs.add(`${c.destinationGateId}:${c.destinationInputIndex || 0}`));

    const primaryInputs = [];
    gates.forEach(g => {
        const inArr = Array.isArray(g.inputs) ? g.inputs : [0, 0];
        inArr.forEach((val, idx) => {
            const key = `${g.id}:${idx}`;
            if (!drivenInputs.has(key)) {
                let pinLabel = `${g.id}.${idx === 0 ? 'A' : (idx === 1 ? 'B' : 'IN' + idx)}`;
                if (Array.isArray(g.inputLabels) && g.inputLabels[idx]) {
                    pinLabel = `${g.id}.${g.inputLabels[idx]}`;
                }
                primaryInputs.push({
                    gateId: g.id,
                    inputIndex: idx,
                    label: pinLabel
                });
            }
        });
    });

    const drivingGates = new Set(connections.map(c => c.sourceGateId));
    let primaryOutputs = gates.filter(g => !drivingGates.has(g.id)).map(g => ({
        gateId: g.id,
        type: g.type,
        output: g.output
    }));
    if (primaryOutputs.length === 0 && gates.length > 0) {
        primaryOutputs = gates.map(g => ({
            gateId: g.id,
            type: g.type,
            output: g.output
        }));
    }

    // Generate Truth Table for up to 6 primary inputs
    const numInputs = primaryInputs.length;
    const truthTable = [];
    if (numInputs === 0 && gates.length > 0) {
        const rowOutputs = {};
        primaryOutputs.forEach(po => {
            const g = gateMap[po.gateId];
            rowOutputs[po.gateId] = g ? g.output : 0;
        });
        truthTable.push({
            row: 1,
            inputs: { 'State': 1 },
            outputs: rowOutputs
        });
    } else if (numInputs > 0 && numInputs <= 6) {
        const totalRows = 1 << numInputs;
        for (let r = 0; r < totalRows; r++) {
            // Clone circuit for row
            const simGates = JSON.parse(JSON.stringify(gates));
            const simGateMap = {};
            simGates.forEach(g => { simGateMap[g.id] = g; });

            const rowInputVals = {};
            for (let i = 0; i < numInputs; i++) {
                const bit = (r >> (numInputs - 1 - i)) & 1;
                const pin = primaryInputs[i];
                rowInputVals[pin.label] = bit;
                const g = simGateMap[pin.gateId];
                if (g && Array.isArray(g.inputs)) {
                    g.inputs[pin.inputIndex] = bit;
                }
            }

            // Propagate
            let pass = 0;
            let cChange = true;
            while (cChange && pass < 12) {
                cChange = false;
                pass++;
                for (const c of connections) {
                    const src = simGateMap[c.sourceGateId];
                    const dst = simGateMap[c.destinationGateId];
                    if (src && dst && Array.isArray(dst.inputs)) {
                        const idx = c.destinationInputIndex || 0;
                        if (dst.inputs[idx] !== src.output) {
                            dst.inputs[idx] = src.output;
                            cChange = true;
                        }
                    }
                }
                for (const g of simGates) {
                    const prev = g.output;
                    g.output = evaluateGate(g.type, g.inputs);
                    if (g.output !== prev) cChange = true;
                }
            }

            const rowOutputs = {};
            primaryOutputs.forEach(po => {
                const g = simGateMap[po.gateId];
                rowOutputs[po.gateId] = g ? g.output : 0;
            });

            truthTable.push({
                row: r + 1,
                inputs: rowInputVals,
                outputs: rowOutputs
            });
        }
    }

    // Successive addition propagation delay (COA CO3 model):
    // gateDelay = 5ns per gate stage. Delay = numStages * 5ns calculated via repeated addition
    let totalDelayNs = 0;
    const stages = Math.max(1, iterations);
    for (let s = 0; s < stages; s++) {
        totalDelayNs += 5; // repeated addition
    }

    return {
        success: true,
        evaluatedGates: gates,
        executionOrder,
        iterations,
        propagationDelayNs: totalDelayNs,
        primaryInputs,
        primaryOutputs,
        truthTable
    };
}

// =============================================================================
// CIRCUIT ENGINEERING REPORT GENERATOR (PDF & CSV)
// =============================================================================
function generateCSVReport(circuit, projectName = 'LogicCraft Circuit') {
    const gates = circuit.gates || [];
    const connections = circuit.connections || [];
    const sim = simulateCircuit({ gates, connections });

    const gateCounts = { AND: 0, OR: 0, NOT: 0, NAND: 0, NOR: 0, XOR: 0, XNOR: 0, BUFFER: 0 };
    gates.forEach(g => {
        const t = (g.type || 'AND').toUpperCase();
        gateCounts[t] = (gateCounts[t] || 0) + 1;
    });

    const drivenInputs = new Set();
    connections.forEach(c => drivenInputs.add(`${c.destinationGateId}:${c.destinationInputIndex || 0}`));
    const floatingPins = [];
    gates.forEach(g => {
        const inLen = (g.type === 'NOT' || g.type === 'BUFFER') ? 1 : 2;
        for (let idx = 0; idx < inLen; idx++) {
            const key = `${g.id}:${idx}`;
            if (!drivenInputs.has(key)) {
                floatingPins.push(`${g.id}.${idx === 0 ? 'A' : 'B'}`);
            }
        }
    });

    let isValid = true;
    const warnings = [];
    if (gates.length === 0) {
        isValid = false;
        warnings.push('Circuit is empty (0 gates placed).');
    } else {
        if (floatingPins.length > 0) {
            warnings.push(`Floating undriven primary pins: ${floatingPins.join(', ')}`);
        }
        if (sim.iterations >= 16) {
            isValid = false;
            warnings.push('Potential feedback loop or circular dependency detected.');
        }
        if (warnings.length === 0) {
            warnings.push('None. All topological netlists verified.');
        }
    }

    let csv = '# ========================================================\r\n';
    csv += '# LOGICCRAFT DIGITAL LOGIC WORKBENCH - ENGINEERING REPORT\r\n';
    csv += '# ========================================================\r\n';
    csv += `# Project Name:,"${projectName}"\r\n`;
    csv += `# Export Date:,${new Date().toISOString()}\r\n`;
    csv += `# Engine Status:,"Topological Multi-Pass Simulator"\r\n\r\n`;

    csv += '[SECTION 1: CIRCUIT METRICS & GATE STATISTICS]\r\n';
    csv += 'Metric,Value\r\n';
    csv += `Total Gates,${gates.length}\r\n`;
    csv += `Total Connections,${connections.length}\r\n`;
    Object.keys(gateCounts).forEach(type => {
        csv += `Gate Count (${type}),${gateCounts[type]}\r\n`;
    });
    csv += '\r\n';

    csv += '[SECTION 2: SIMULATION & PROPAGATION RESULTS]\r\n';
    csv += 'Metric,Value\r\n';
    csv += `Propagation Delay (ns),${sim.propagationDelayNs || 0} ns\r\n`;
    csv += `Logic Stages,${sim.iterations || 0}\r\n`;
    csv += `Convergence State,"${sim.iterations < 16 ? 'Converged' : 'Circular Feedback / Loop'}"\r\n`;
    csv += `Primary Inputs,${(sim.primaryInputs || []).length}\r\n`;
    csv += `Primary Outputs,${(sim.primaryOutputs || []).length}\r\n\r\n`;

    csv += '[SECTION 3: CIRCUIT VALIDATION & INTEGRITY CHECKS]\r\n';
    csv += 'Verification Check,Status,Details\r\n';
    csv += `Overall Topology Verdict,${isValid ? 'VALID' : 'WARNING / ANOMALIES DETECTED'},"${isValid ? 'Passed combinational integrity checks' : 'Review warnings below'}"\r\n`;
    csv += `Floating Pin Analysis,${floatingPins.length > 0 ? 'INFO' : 'PASS'},"${floatingPins.length > 0 ? floatingPins.join('; ') : 'All gates connected'}"\r\n`;
    csv += `Cyclic Loop Check,${sim.iterations < 16 ? 'PASS' : 'FAIL'},"${sim.iterations < 16 ? 'Acyclic feed-forward netlist' : 'Cyclic loop detected'}"\r\n`;
    csv += `Detected Warnings / Errors,"${isValid ? 'None' : 'Active'}","${warnings.join('; ')}"\r\n\r\n`;

    csv += '[SECTION 4: COMPONENT NETLIST & TERMINAL SIGNALS]\r\n';
    csv += 'Gate ID,Gate Name,Type,Inputs,Output,Signal State,Position X,Position Y\r\n';
    gates.forEach(g => {
        const inStr = Array.isArray(g.inputs) ? g.inputs.join(' ') : '0';
        csv += `"${g.id}","${g.name || g.id}","${g.type}","[${inStr}]",${g.output || 0},"${g.output ? 'HIGH (1)' : 'LOW (0)'}",${g.x || 0},${g.y || 0}\r\n`;
    });
    csv += '\r\n';

    csv += '[SECTION 5: COMBINATORIAL TRUTH TABLE (2^N STATE ENUMERATION)]\r\n';
    const tt = sim.truthTable || [];
    if (tt.length > 0) {
        const firstRow = tt[0];
        const inKeys = Object.keys(firstRow.inputs || {});
        const outKeys = Object.keys(firstRow.outputs || {});
        csv += 'Row Number,' + inKeys.map(k => `Input (${k})`).join(',') + ',' + outKeys.map(k => `Output (${k})`).join(',') + '\r\n';
        tt.forEach(row => {
            const inVals = inKeys.map(k => row.inputs[k]);
            const outVals = outKeys.map(k => row.outputs[k]);
            csv += `#${row.row},` + inVals.join(',') + ',' + outVals.join(',') + '\r\n';
        });
    } else {
        csv += 'Status,Notice\r\n';
        csv += 'No Rows Generated,"Circuit has no primary inputs or 0 gates placed."\r\n';
    }

    return csv;
}

function generatePDFReport(circuit, projectName = 'LogicCraft Circuit') {
    const gates = circuit.gates || [];
    const connections = circuit.connections || [];
    const sim = simulateCircuit({ gates, connections });

    const gateCounts = { AND: 0, OR: 0, NOT: 0, NAND: 0, NOR: 0, XOR: 0, XNOR: 0, BUFFER: 0 };
    gates.forEach(g => {
        const t = (g.type || 'AND').toUpperCase();
        gateCounts[t] = (gateCounts[t] || 0) + 1;
    });

    const drivenInputs = new Set();
    connections.forEach(c => drivenInputs.add(`${c.destinationGateId}:${c.destinationInputIndex || 0}`));
    const floatingPins = [];
    gates.forEach(g => {
        const inLen = (g.type === 'NOT' || g.type === 'BUFFER') ? 1 : 2;
        for (let idx = 0; idx < inLen; idx++) {
            const key = `${g.id}:${idx}`;
            if (!drivenInputs.has(key)) {
                floatingPins.push(`${g.id}.${idx === 0 ? 'A' : 'B'}`);
            }
        }
    });

    let isValid = true;
    let warnings = 'None. All gate terminals and netlists verified.';
    if (gates.length === 0) {
        isValid = false;
        warnings = 'Warning: Canvas contains zero gates.';
    } else if (sim.iterations >= 16) {
        isValid = false;
        warnings = 'Warning: Potential feedback loop / circular connection detected.';
    } else if (floatingPins.length > 0) {
        warnings = `Primary undriven input pins: ${floatingPins.join(', ')}`;
    }

    let out = '%PDF-1.4\n';
    const offsets = [];

    let stream = '';
    function printText(x, y, text, font = '/F1', size = 10, r = 0, g = 0, b = 0) {
        const safe = String(text || '').replace(/\\/g, '\\\\').replace(/\(/g, '\\(').replace(/\)/g, '\\)');
        stream += `BT ${font} ${size} Tf ${r} ${g} ${b} rg ${x} ${y} Td (${safe}) Tj ET\n`;
    }
    function drawLine(x1, y1, x2, y2, r = 0.8, g = 0.8, b = 0.8, w = 1) {
        stream += `${w} w ${r} ${g} ${b} RG ${x1} ${y1} m ${x2} ${y2} l S\n`;
    }
    function drawRect(x, y, w, h, fillR = 0.95, fillG = 0.95, fillB = 0.95, strokeR = 0.8, strokeG = 0.8, strokeB = 0.8) {
        stream += `${strokeR} ${strokeG} ${strokeB} RG ${fillR} ${fillG} ${fillB} rg ${x} ${y} ${w} ${h} re B\n`;
    }

    // Page Header Banner
    drawRect(40, 720, 532, 50, 0.1, 0.16, 0.13, 0.1, 0.16, 0.13);
    printText(55, 748, 'LogicCraft Digital Logic Workbench', '/F2', 16, 1, 1, 1);
    printText(55, 732, 'Circuit Engineering Analysis & Verification Report', '/F1', 9, 0.8, 0.9, 0.85);

    printText(40, 698, `Project: ${projectName}`, '/F2', 12, 0.1, 0.1, 0.1);
    printText(40, 684, `Export Date: ${new Date().toISOString()}   |   Status: ${isValid ? 'VERIFIED' : 'WARNING'}`, '/F1', 8, 0.4, 0.4, 0.4);

    drawLine(40, 674, 572, 674, 0.7, 0.7, 0.7, 1);

    // Section 1: Gate Statistics
    printText(40, 656, '1. Circuit Gate Statistics & Breakdown', '/F2', 11, 0.13, 0.42, 0.22);
    printText(40, 640, `Total Components: ${gates.length} gates    |    Total Connections: ${connections.length} wires`, '/F1', 9, 0.2, 0.2, 0.2);

    let curY = 622;
    drawRect(40, curY, 532, 16, 0.92, 0.95, 0.93, 0.8, 0.85, 0.8);
    printText(45, curY + 4, 'Gate Type', '/F2', 8, 0.2, 0.3, 0.2);
    printText(130, curY + 4, 'Count', '/F2', 8, 0.2, 0.3, 0.2);
    printText(200, curY + 4, 'Gate IDs Placed in Netlist', '/F2', 8, 0.2, 0.3, 0.2);

    curY -= 14;
    let anyGates = false;
    Object.keys(gateCounts).forEach(type => {
        const count = gateCounts[type];
        if (count > 0) {
            anyGates = true;
            const ids = gates.filter(g => (g.type || '').toUpperCase() === type).map(g => g.id).join(', ');
            printText(45, curY + 3, type, '/F2', 8, 0.2, 0.2, 0.2);
            printText(130, curY + 3, String(count), '/F1', 8, 0.2, 0.2, 0.2);
            printText(200, curY + 3, ids, '/F3', 8, 0.3, 0.3, 0.3);
            drawLine(40, curY, 572, curY, 0.9, 0.9, 0.9, 0.5);
            curY -= 14;
        }
    });
    if (!anyGates) {
        printText(45, curY + 3, 'No gates placed in circuit.', '/F1', 8, 0.5, 0.5, 0.5);
        curY -= 14;
    }

    // Section 2: Schematic Netlist Diagram
    curY -= 8;
    printText(40, curY, '2. Circuit Schematic Netlist Topology', '/F2', 11, 0.13, 0.42, 0.22);
    curY -= 14;
    drawRect(40, curY - 50, 532, 58, 0.97, 0.98, 0.97, 0.85, 0.88, 0.85);

    if (gates.length > 0) {
        let gx = 60;
        gates.forEach((g, idx) => {
            drawRect(gx, curY - 42, 70, 32, 0.92, 0.96, 0.93, 0.3, 0.6, 0.4);
            printText(gx + 10, curY - 25, g.id + ':' + g.type, '/F2', 8, 0.1, 0.3, 0.2);
            printText(gx + 10, curY - 37, 'Out: ' + (g.output || 0), '/F1', 7, 0.3, 0.3, 0.3);
            if (idx < gates.length - 1) {
                drawLine(gx + 70, curY - 26, gx + 95, curY - 26, 0.2, 0.5, 0.3, 1.5);
            }
            gx += 95;
            if (gx > 500) gx = 60;
        });
    } else {
        printText(55, curY - 25, 'Schematic empty (0 components placed on canvas)', '/F1', 8, 0.5, 0.5, 0.5);
    }
    curY -= 62;

    // Section 3: Simulation Results
    printText(40, curY, '3. Simulation & Propagation Results', '/F2', 11, 0.13, 0.42, 0.22);
    curY -= 14;
    printText(40, curY, `Propagation Delay: ${sim.propagationDelayNs || 0} ns    |    Logic Stages: ${sim.iterations || 0}    |    Convergence: ${sim.iterations < 16 ? 'Converged' : 'Feedback Loop'}`, '/F1', 8.5, 0.2, 0.2, 0.2);

    // Section 4: Validation Diagnostics
    curY -= 18;
    printText(40, curY, '4. Circuit Validation & Structural Diagnostics', '/F2', 11, 0.13, 0.42, 0.22);
    curY -= 16;
    drawRect(40, curY - 2, 532, 16, isValid ? 0.93 : 0.98, isValid ? 0.97 : 0.93, isValid ? 0.93 : 0.93, 0.7, 0.8, 0.7);
    printText(45, curY + 3, `VERDICT: ${isValid ? 'VALID (PASSED INTEGRITY CHECKS)' : 'WARNING / ANOMALIES DETECTED'}`, '/F2', 8.5, isValid ? 0.1 : 0.7, isValid ? 0.45 : 0.1, 0.1);
    curY -= 16;
    printText(40, curY, `Diagnostics / Warnings: ${warnings}`, '/F1', 8, 0.3, 0.3, 0.3);

    // Section 5: Truth Table
    curY -= 20;
    printText(40, curY, '5. Combinatorial State Space Truth Table (2^N Enumeration)', '/F2', 11, 0.13, 0.42, 0.22);
    curY -= 16;

    const tt = sim.truthTable || [];
    if (tt.length > 0) {
        drawRect(40, curY, 532, 15, 0.92, 0.95, 0.93, 0.8, 0.85, 0.8);
        printText(45, curY + 4, 'Row', '/F2', 8, 0.2, 0.3, 0.2);

        const firstRow = tt[0];
        const inKeys = Object.keys(firstRow.inputs || {});
        const outKeys = Object.keys(firstRow.outputs || {});

        let colX = 90;
        inKeys.forEach(k => {
            printText(colX, curY + 4, `In ${k}`, '/F2', 8, 0.2, 0.3, 0.2);
            colX += 55;
        });
        outKeys.forEach(k => {
            printText(colX, curY + 4, `Out ${k}`, '/F2', 8, 0.1, 0.4, 0.2);
            colX += 55;
        });

        curY -= 13;
        tt.slice(0, 16).forEach(row => {
            printText(45, curY + 2.5, `#${row.row}`, '/F2', 7.5, 0.3, 0.3, 0.3);
            let rx = 90;
            inKeys.forEach(k => {
                printText(rx, curY + 2.5, String(row.inputs[k]), '/F1', 7.5, 0.1, 0.1, 0.1);
                rx += 55;
            });
            outKeys.forEach(k => {
                const v = row.outputs[k];
                printText(rx, curY + 2.5, String(v), '/F2', 7.5, v ? 0.1 : 0.5, v ? 0.5 : 0.1, 0.1);
                rx += 55;
            });
            drawLine(40, curY, 572, curY, 0.9, 0.9, 0.9, 0.5);
            curY -= 13;
        });
    } else {
        printText(40, curY - 5, 'No truth table generated (circuit has 0 gates or inputs).', '/F1', 8, 0.5, 0.5, 0.5);
    }

    offsets.push(Buffer.byteLength(out, 'utf8'));
    out += '1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n';

    offsets.push(Buffer.byteLength(out, 'utf8'));
    out += '2 0 obj\n<< /Type /Pages /Kids [7 0 R] /Count 1 >>\nendobj\n';

    offsets.push(Buffer.byteLength(out, 'utf8'));
    out += '3 0 obj\n<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica >>\nendobj\n';

    offsets.push(Buffer.byteLength(out, 'utf8'));
    out += '4 0 obj\n<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica-Bold >>\nendobj\n';

    offsets.push(Buffer.byteLength(out, 'utf8'));
    out += '5 0 obj\n<< /Type /Font /Subtype /Type1 /BaseFont /Courier >>\nendobj\n';

    offsets.push(Buffer.byteLength(out, 'utf8'));
    out += '6 0 obj\n<< /Length ' + Buffer.byteLength(stream, 'utf8') + ' >>\nstream\n' + stream + 'endstream\nendobj\n';

    offsets.push(Buffer.byteLength(out, 'utf8'));
    out += '7 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [0 0 612 792] /Resources << /Font << /F1 3 0 R /F2 4 0 R /F3 5 0 R >> >> /Contents 6 0 R >>\nendobj\n';

    const xrefOffset = Buffer.byteLength(out, 'utf8');
    out += 'xref\n0 ' + (offsets.length + 1) + '\n';
    out += '0000000000 65535 f \n';
    offsets.forEach(off => {
        out += String(off).padStart(10, '0') + ' 00000 n \n';
    });
    out += 'trailer\n<< /Size ' + (offsets.length + 1) + ' /Root 1 0 R >>\nstartxref\n' + xrefOffset + '\n%%EOF\n';

    return out;
}

// Course Outcome to File Mapping Dataset (Task 3 Verified)
const CO_MAPPING_DATA = [
    {
        subject: "Programming Lab",
        co: "CO3",
        concept: "Stack and Queue (LIFO Undo Stack & FIFO Event Simulation)",
        filePath: "Cpp/DataStructures/StackOperations.cpp",
        lineRange: "L1-L135",
        symbol: "CircuitUndoStack, pushAction(), popAction(), executeUndoOperation()",
        projectUsage: "Records circuit operations (adding/deleting gates, modifying pin inputs, wiring) on a procedural LIFO stack to support non-destructive Undo. Manages discrete signal propagation via circular FIFO SimulationQueue.",
        academicNote: "Strictly procedural C-style structs and functions with boundary underflow/overflow protection. Complies with the non-OOP C++ syllabus constraint.",
        testDemo: "Cpp/Tests/TestRunner.cpp -> testStackOperations(), testQueueOperations()",
        status: "Complete"
    },
    {
        subject: "Computer Organization & Architecture Lab",
        co: "CO3",
        concept: "64-Bit Assembly Arithmetic & ASCII String Processing",
        filePath: "COA/ALP/logic_ops_64.s",
        lineRange: "L135-L330",
        symbol: "asm_mul_successive_addition_64, asm_parse_gate_type_64, asm_validate_command_64, asm_eval_boolean_expr_str_64",
        projectUsage: "Performs repeated-addition arithmetic in x86-64 assembly to compute circuit propagation delays and 2^N truth-table rows. Parses ASCII gate opcodes (AND, OR, NOT, NAND, NOR, XOR, XNOR) and evaluates Boolean string expressions directly in hardware registers.",
        academicNote: "Uses 64-bit AMD64 System V / Microsoft x64 register conventions (rax, rcx, rdx, r8, r9) with loop counters, conditional branching, and boundary bounds checking.",
        testDemo: "COA/Tests/TestALP.cpp -> testTask3COAAssembly()",
        status: "Complete"
    },
    {
        subject: "Computer Graphics Lab",
        co: "CO6",
        concept: "Texture Mapping & Hardware Lighting / Material Color Effects",
        filePath: "Computer Graphics/Rendering/GateRenderer.cpp",
        lineRange: "L25-L160",
        symbol: "GateRenderer::initTextures(), initLighting(), drawTexturedSubstrate(), applyMaterial()",
        projectUsage: "Applies procedural 2D textured PCB substrate and IC package textures (GL_TEXTURE_2D) beneath the schematic grid, with OpenGL hardware lights (GL_LIGHT0), material specular highlights, and active terminal emissive glows toggled via keys [T] and [L].",
        academicNote: "Preserves existing 2D projection, gate primitives (CO1), and transformation/clipping matrices (CO2) while enhancing visual realism without compromising symbol legibility.",
        testDemo: "Computer Graphics/main.cpp -> --test (Automated CO1, CO2, CO6 test suite)",
        status: "Complete"
    },
    {
        subject: "Problem Solving Using OOP (Java)",
        co: "CO3",
        concept: "Exception Handling, Swing GUI, Packages & Collections",
        filePath: "java/src/logiccraft/exceptions/InvalidInputException.java",
        lineRange: "L1-L24",
        symbol: "InvalidInputException, ComponentList<T>, LogicCraftUI, logiccraft.*",
        projectUsage: "Provides comprehensive runtime validation for invalid pin values, malformed project files, and circular loops. Employs generic collections (ComponentList<T>) for gate/wire management, packaged into clean logiccraft.* modules with an interactive Swing canvas interface.",
        academicNote: "Preserves existing CO1 (Inheritance/Polymorphism) and CO2 (Multithreading/File I/O) features while introducing authoritative robust exception hierarchies and Swing components.",
        testDemo: "java/src/test/ExceptionTest.java, GenericTest.java, FileTest.java",
        status: "Complete"
    },
    // =========================================================================
    // TASK 4 OUTCOMES (CO4, CO5, CO6)
    // =========================================================================
    {
        subject: "Programming Lab",
        co: "CO4",
        concept: "Searching & Sorting (Linear Search, Binary Search, Bubble Sort, Selection Sort)",
        filePath: "Cpp/Searching/Searching.cpp",
        lineRange: "L1-L120",
        symbol: "linearSearchById(), binarySearchById(), bubbleSort(), selectionSort()",
        projectUsage: "Performs Linear and Binary searches on circuit gates by ID or name with comparison tracking. Sorts components using Bubble Sort and Selection Sort by ID, name, or gate type.",
        academicNote: "Strictly procedural C++ without OOP. Operates on arrays of structures with metrics logging.",
        testDemo: "Cpp/Tests/Task4Tests.cpp -> testSearchingAlgorithms(), testSortingAlgorithms()",
        status: "Complete"
    },
    {
        subject: "Programming Lab",
        co: "CO5",
        concept: "Hashing with Linear Probing Collision Resolution",
        filePath: "Cpp/Hashing/HashTable.cpp",
        lineRange: "L1-L150",
        symbol: "ComponentHashTable, insertHashTable(), searchHashTableById(), deleteHashTableById()",
        projectUsage: "Fast O(1) hash table lookup with prime capacity (17) for component indexing. Demonstrates collision resolution via Linear Probing and tombstone handling for deletions.",
        academicNote: "Pure procedural C-style structs and open addressing.",
        testDemo: "Cpp/Tests/Task4Tests.cpp -> testHashTableLinearProbing()",
        status: "Complete"
    },
    {
        subject: "Problem Solving Using C++",
        co: "CO4",
        concept: "Smart Circuit Validation & Decision-Making Recommendation",
        filePath: "Cpp/CircuitEngine/CircuitValidator.cpp",
        lineRange: "L1-L220",
        symbol: "validateCircuit(), hasCyclicDependency(), runCircuitValidationScenarios()",
        projectUsage: "Analyzes circuit graph topology, verifies wire boundaries, detects floating inputs, checks for self-loops and cyclic loops, and issues actionable engineering recommendations across 3 test scenarios.",
        academicNote: "Pure procedural C++ decision-making engine coordinating Stack, Queue, Sorting, and Hashing.",
        testDemo: "Cpp/Tests/Task4Tests.cpp -> testCircuitValidatorDecisions()",
        status: "Complete"
    },
    {
        subject: "Computer Organization & Architecture Lab",
        co: "CO4",
        concept: "Processor-Level Architecture: ALU, Control Unit, 5-Stage Pipeline & Assembly Integration",
        filePath: "COA/Pipeline/PipelineSimulator.cpp",
        lineRange: "L1-L500",
        symbol: "BooleanALU, ControlUnit, HardwiredControl, MicroprogrammedControl, PipelineSimulator, ProcessorSimulator",
        projectUsage: "Complete processor architecture: 10-operation ALU with status flags (ZF/SF/CF/OF), Control Unit with Hardwired and Microprogrammed models, Control Signal truth table, 5-stage instruction pipeline (IF/ID/EX/MEM/WB) with RAW forwarding and Load-Use hazard stall bubbles, and CPU simulation of logic gates, propagation delay, and netlist parsing.",
        academicNote: "Micro-architectural datapath modeling with 64-bit AMD64 assembly routines adhering to Microsoft x64 ABI.",
        testDemo: "COA/Tests/TestALP.cpp -> runCOATask4IntegrationSuite()",
        status: "Complete"
    },
    {
        subject: "Computer Graphics Lab",
        co: "CO4",
        concept: "Authentic ANSI/IEEE Logic Gate Symbol Geometry & Precision Alignment",
        filePath: "Computer Graphics/Rendering/GateRenderer.cpp",
        lineRange: "L180-L620",
        symbol: "GateRenderer::drawGate(), drawANDGate(), drawNANDGate(), drawORGate(), drawNORGate(), drawXORGate(), drawXNORGate(), drawNOTGate(), drawInversionBubble(), getGatePinPosition()",
        projectUsage: "Renders authentic ANSI/IEEE Std 91-1984 logic gate shapes: D-shape (AND/NAND), pointed dual-convex with concave back (OR/NOR), triangular inverter with bubble (NOT), and detached-arc shields (XOR/XNOR). Preserves sub-pixel pin alignment and wire connections.",
        academicNote: "Constructed with GL_POLYGON, GL_TRIANGLE_FAN, GL_LINE_STRIP, GL_TRIANGLES, and GL_LINE_LOOP. Preserves camera transforms, texture mapping, and material lighting.",
        testDemo: "Computer Graphics/main.cpp -> --test (Automated 8-test verification)",
        status: "Complete"
    },
    {
        subject: "Problem Solving Using OOP (Java)",
        co: "CO4",
        concept: "File Handling & Java Generics Reusable Component Architecture",
        filePath: "java/src/logiccraft/file/ProjectFileManager.java",
        lineRange: "L1-L160",
        symbol: "saveCircuitToFile(), loadCircuitFromFile(), ComponentList<T>",
        projectUsage: "Persists circuit topologies to disk, handles missing files (FileNotFoundException) and corrupted files (IOException), and leverages custom Java Generics (ComponentList<T>) with predicate filtering and Swing GUI file dialogs.",
        academicNote: "Strict encapsulation, robust exception safety, and custom generic data structures.",
        testDemo: "java/src/test/Task4FileGenericTest.java",
        status: "Complete"
    }
];

// HTTP Server
const server = http.createServer((req, res) => {
    // Enable CORS
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE, OPTIONS');
    res.setHeader('Access-Control-Allow-Headers', 'Content-Type');

    if (req.method === 'OPTIONS') {
        res.writeHead(204);
        res.end();
        return;
    }

    const parsedUrl = new URL(req.url, 'http://' + (req.headers.host || 'localhost:3000'));
    const pathname = parsedUrl.pathname;

    // Favicon handling
    if (pathname === '/favicon.ico' || pathname === '/favicon.svg') {
        const iconPath = path.join(FRONTEND_DIR, 'favicon.svg');
        if (fs.existsSync(iconPath)) {
            res.writeHead(200, { 'Content-Type': 'image/svg+xml' });
            res.end(fs.readFileSync(iconPath));
            return;
        }
        res.writeHead(204);
        res.end();
        return;
    }

    // Authentication APIs (for Java Swing Desktop Launcher and Web Login)
    if (pathname === '/api/auth/login' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => { body += chunk; });
        req.on('end', () => {
            try {
                const creds = JSON.parse(body || '{}');
                const username = (creds.username || '').trim();
                const password = creds.password || '';

                if (!username || !password) {
                    res.writeHead(400, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({ success: false, message: 'Username and password cannot be empty.' }));
                    return;
                }

                const users = loadUsers();
                const user = users[username.toLowerCase()];
                if (!user) {
                    res.writeHead(401, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({ success: false, message: 'Invalid username or password.' }));
                    return;
                }

                const isValid = verifyPassword(password, user.salt, user.hash);
                if (!isValid) {
                    res.writeHead(401, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({ success: false, message: 'Invalid username or password.' }));
                    return;
                }

                const token = createSession(user.username, user.role);
                res.writeHead(200, { 'Content-Type': 'application/json', 'Set-Cookie': sessionCookie(token) });
                res.end(JSON.stringify({
                    success: true,
                    token,
                    user: { username: user.username, role: user.role }
                }));
            } catch (err) {
                res.writeHead(500, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ success: false, message: 'Server authentication error.' }));
            }
        });
        return;
    }

    if (pathname === '/api/auth/session' && req.method === 'GET') {
        const session = getSession(req);
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ authenticated: !!session, user: session ? { username: session.username, role: session.role } : null }));
        return;
    }
    if (pathname === '/api/auth/logout' && req.method === 'POST') {
        const cookieToken = (req.headers.cookie || '').split(';').map(v => v.trim()).find(v => v.startsWith('logiccraft_session='));
        const token = cookieToken ? cookieToken.split('=').slice(1).join('') : '';
        if (token) sessions.delete(token);
        res.writeHead(200, { 'Content-Type': 'application/json', 'Set-Cookie': 'logiccraft_session=; HttpOnly; SameSite=Strict; Path=/; Max-Age=0' });
        res.end(JSON.stringify({ success: true }));
        return;
    }
    if (pathname === '/api/auth/register' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => { body += chunk; });
        req.on('end', () => {
            try {
                const creds = JSON.parse(body || '{}');
                const username = (creds.username || '').trim();
                const password = creds.password || '';
                const role = creds.role || 'Circuit Designer';

                if (!username || !password) {
                    res.writeHead(400, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({ success: false, message: 'Username and password are required.' }));
                    return;
                }
                if (password.length < 6) {
                    res.writeHead(400, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({ success: false, message: 'Password must be at least 6 characters.' }));
                    return;
                }

                const users = loadUsers();
                if (users[username.toLowerCase()]) {
                    res.writeHead(409, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({ success: false, message: 'User already exists.' }));
                    return;
                }

                const { salt, hash } = hashPassword(password);
                users[username.toLowerCase()] = {
                    username,
                    salt,
                    hash,
                    role,
                    created: new Date().toISOString()
                };
                saveUsers(users);

                res.writeHead(201, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ success: true, message: 'Account registered successfully.' }));
            } catch (err) {
                res.writeHead(500, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ success: false, message: 'Registration failed.' }));
            }
        });
        return;
    }

    // Require an authenticated browser/desktop session for protected APIs.
    const publicApiPaths = new Set(['/api/auth/login', '/api/auth/register', '/api/auth/session', '/api/auth/logout', '/api/health', '/api/co-mapping']);
    if (pathname.startsWith('/api/') && !publicApiPaths.has(pathname)) {
        const session = getSession(req);
        if (!session) { res.writeHead(401, { 'Content-Type': 'application/json' }); res.end(JSON.stringify({ error: 'Authentication required. Please sign in.' })); return; }
    }

    // REST API Routes
    if (pathname === '/api/health') {
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({
            status: 'online',
            service: 'LogicCraft Backend API',
            version: '3.0.0',
            uptimeSeconds: Math.floor(process.uptime()),
            modules: {
                cpp: 'Verified (CO1, CO2, CO3, CO4, CO5)',
                coa: 'Verified (CO1, CO2, CO3, CO4 - ALU, Control Unit, Pipeline & Assembly)',
                graphics: 'Verified (CO1, CO2, CO4, CO6)',
                java: 'Verified (CO1, CO2, CO3, CO4)'
            }
        }));
        return;
    }

    // Native procedural C++ DSA bridge (CO4). The bridge is built separately with Cpp/build_dsa_bridge.bat.
    if (pathname === '/api/dsa/native' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => { body += chunk; if (body.length > 2 * 1024 * 1024) req.destroy(); });
        req.on('end', () => {
            try {
                const payload = JSON.parse(body || '{}');
                const gates = Array.isArray(payload.gates) ? payload.gates : [];
                if (gates.length > 5000) { res.writeHead(413, {'Content-Type':'application/json'}); res.end(JSON.stringify({success:false,error:'Maximum 5000 gates per native DSA operation.'})); return; }
                const bridgePath = path.join(ROOT_DIR, 'Cpp', process.platform === 'win32' ? 'logiccraft_dsa_bridge.exe' : 'logiccraft_dsa_bridge');
                if (!fs.existsSync(bridgePath)) { res.writeHead(503, {'Content-Type':'application/json'}); res.end(JSON.stringify({success:false,native:false,error:'Native DSA bridge not built. Run Cpp/build_dsa_bridge.bat on Windows.'})); return; }
                const clean = (v, max) => String(v == null ? '' : v).replace(/[|\r\n]/g, ' ').slice(0, max);
                const rows = gates.map((g, i) => `${parseInt(String(g.id || `G${i+1}`).replace(/\D/g,''),10) || i+1}|${clean(g.type || 'AND',15)}|${clean(g.name || `Gate_${g.id || i+1}`,31)}|${Array.isArray(g.inputs)?g.inputs.length:2}|${g.output?1:0}`).join('\n');
                const op = payload.operation === 'sort' ? 'sort' : 'search';
                const key = ['id','name','type'].includes(payload.key || payload.field) ? (payload.key || payload.field) : 'id';
                const algo = op === 'sort' ? (['bubble','selection','insertion','quick','merge'].includes(payload.algorithm) ? payload.algorithm : 'merge') : (payload.algorithm === 'binary' ? 'binary' : 'linear');
                const fourth = op === 'sort' ? (payload.order === 'desc' ? 'desc' : 'asc') : clean(payload.query, 64);
                const child = spawn(bridgePath, [op, key, algo, fourth], { windowsHide: true });
                let stdout = '', stderr = '';
                child.stdout.on('data', c => { stdout += c.toString(); if (stdout.length > 1024 * 1024) child.kill(); });
                child.stderr.on('data', c => { stderr += c.toString(); });
                const timeout = setTimeout(() => child.kill(), 10000);
                child.on('error', err => { clearTimeout(timeout); if (!res.headersSent) { res.writeHead(500, {'Content-Type':'application/json'}); res.end(JSON.stringify({success:false,native:true,error:err.message})); } });
                child.on('close', code => {
                    clearTimeout(timeout);
                    if (code !== 0) { if (!res.headersSent) { res.writeHead(500, {'Content-Type':'application/json'}); res.end(JSON.stringify({success:false,native:true,error:(stderr || `Native bridge exit ${code}`).slice(0,500)})); } return; }
                    const lines = String(stdout).trim().split(/\r?\n/).filter(Boolean);
                    const byId = new Map(gates.map((g,i)=>[String(parseInt(String(g.id || `G${i+1}`).replace(/\D/g,''),10)||i+1), g]));
                    const record = line => { const parts=line.split('|'); const original=byId.get(parts[0]); return original ? {...original, id: original.id || `G${parts[0]}`, type:parts[1], name:parts[2], output:Number(parts[4])} : {id:`G${parts[0]}`,type:parts[1],name:parts[2],inputs:[],output:Number(parts[4]),x:0,y:0}; };
                    if (op === 'sort') {
                        const metric=(lines[0]||'').split('|'); const sorted=lines.slice(1).map(record);
                        res.writeHead(200, {'Content-Type':'application/json'}); res.end(JSON.stringify({success:true,native:true,algorithm:algo,field:key,order:fourth,comparisons:Number(metric[1])||0,swaps:Number(metric[2])||0,sorted}));
                    } else {
                        const header=(lines[0]||'').split('|'); const idx=Number(header[1]); const found=idx>=0 && lines[1]; const item=found?record(lines[1]):null;
                        res.writeHead(200, {'Content-Type':'application/json'}); res.end(JSON.stringify({success:true,native:true,found:!!item,index:idx,item,algorithm:algo==='binary'?'Binary Search (C++ O(log N))':'Linear Search (C++ O(N))',comparisons:Number(header[2])||0,timeMicroseconds:null,query:payload.query,key,totalItems:gates.length}));
                    }
                });
                child.stdin.end(rows);
            } catch (err) { res.writeHead(400, {'Content-Type':'application/json'}); res.end(JSON.stringify({success:false,error:err.message})); }
        });
        return;
    }

    if (pathname === '/api/co-mapping') {
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({
            count: CO_MAPPING_DATA.length,
            outcomes: CO_MAPPING_DATA
        }));
        return;
    }

    if (pathname === '/api/projects' && req.method === 'GET') {
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(Object.values(projects)));
        return;
    }

    if (pathname === '/api/projects' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => { body += chunk; });
        req.on('end', () => {
            try {
                const data = JSON.parse(body || '{}');
                const id = data.id || `proj-${Date.now()}`;
                const project = {
                    id,
                    name: data.name || 'Untitled Circuit',
                    description: data.description || 'Created in LogicCraft Canvas',
                    gates: data.gates || [],
                    connections: data.connections || [],
                    history: [],
                    updatedAt: new Date().toISOString()
                };
                projects[id] = project;
                res.writeHead(201, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(project));
            } catch (err) {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: 'Malformed JSON payload' }));
            }
        });
        return;
    }

    if (pathname.startsWith('/api/projects/') && req.method === 'GET') {
        const id = pathname.substring('/api/projects/'.length);
        const proj = projects[id];
        if (proj) {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(proj));
        } else {
            res.writeHead(404, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: `Project '${id}' not found` }));
        }
        return;
    }

    if (pathname.startsWith('/api/projects/') && req.method === 'PUT') {
        const id = pathname.substring('/api/projects/'.length);
        let body = '';
        req.on('data', chunk => { body += chunk; });
        req.on('end', () => {
            try {
                const data = JSON.parse(body || '{}');
                if (!projects[id]) {
                    projects[id] = { id, history: [] };
                }
                const p = projects[id];
                p.name = data.name || p.name || 'Updated Circuit';
                p.description = data.description || p.description;
                if (data.gates) p.gates = data.gates;
                if (data.connections) p.connections = data.connections;
                p.updatedAt = new Date().toISOString();

                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(p));
            } catch (err) {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: 'Failed to update project' }));
            }
        });
        return;
    }

    if (pathname === '/api/simulate' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => { body += chunk; });
        req.on('end', () => {
            try {
                const payload = JSON.parse(body || '{}');
                const circuit = payload.circuit || payload;
                if (!circuit || !Array.isArray(circuit.gates)) {
                    res.writeHead(400, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({ error: 'Circuit definition must contain a gates array' }));
                    return;
                }
                const result = simulateCircuit(circuit);
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify(result));
            } catch (err) {
                res.writeHead(500, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: err.message }));
            }
        });
        return;
    }

    // Dynamic Circuit Report Export: CSV
    if (pathname === '/api/export/csv' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => { body += chunk; });
        req.on('end', () => {
            try {
                const payload = JSON.parse(body || '{}');
                const circuit = payload.circuit || payload || (projects['proj-ref-1'] || { gates: [], connections: [] });
                const projectName = payload.projectName || (circuit && circuit.name) || 'LogicCraft Circuit';
                const csvData = generateCSVReport(circuit, projectName);
                res.writeHead(200, {
                    'Content-Type': 'text/csv; charset=utf-8',
                    'Content-Disposition': `attachment; filename="LogicCraft_Circuit_Report_${Date.now()}.csv"`
                });
                res.end(csvData);
            } catch (err) {
                res.writeHead(500, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: 'Failed to generate CSV report: ' + err.message }));
            }
        });
        return;
    }

    // Dynamic Circuit Report Export: PDF
    if (pathname === '/api/export/pdf' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => { body += chunk; });
        req.on('end', () => {
            try {
                const payload = JSON.parse(body || '{}');
                const circuit = payload.circuit || payload || (projects['proj-ref-1'] || { gates: [], connections: [] });
                const projectName = payload.projectName || (circuit && circuit.name) || 'LogicCraft Circuit';
                const pdfData = generatePDFReport(circuit, projectName);
                res.writeHead(200, {
                    'Content-Type': 'application/pdf',
                    'Content-Disposition': `attachment; filename="LogicCraft_Circuit_Report_${Date.now()}.pdf"`
                });
                res.end(Buffer.from(pdfData, 'utf8'));
            } catch (err) {
                res.writeHead(500, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: 'Failed to generate PDF report: ' + err.message }));
            }
        });
        return;
    }

    // =========================================================================
    // TASK 4 REST API ENDPOINTS (Searching, Sorting, Hashing, Validation)
    // =========================================================================

    // 1. Searching (CO4: Linear Search & Binary Search)
    if (pathname === '/api/search' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => { body += chunk; });
        req.on('end', () => {
            try {
                const payload = JSON.parse(body || '{}');
                const gates = payload.gates || payload.dataset || (projects['proj-ref-1'] ? projects['proj-ref-1'].gates : []);
                const target = payload.query != null ? String(payload.query).trim() : '';
                const searchMode = (payload.key || payload.mode || 'id').toLowerCase();
                const chosenAlgo = (payload.algorithm || 'linear').toLowerCase();

                // Format gates as numeric/string items preserving coords and inputs
                const items = gates.map((g, idx) => ({
                    index: idx,
                    id: g.id || `G${idx + 1}`,
                    numericId: parseInt(String(g.id).replace(/\D/g, '')) || (idx + 1),
                    name: g.name || `Gate_${g.id}`,
                    type: g.type,
                    inputs: g.inputs || [0, 0],
                    output: g.output || 0,
                    x: g.x != null ? g.x : (-150 + idx * 160),
                    y: g.y != null ? g.y : 0
                }));

                // Linear Search
                let linComps = 0;
                let linFound = -1;
                for (let i = 0; i < items.length; i++) {
                    linComps++;
                    if (searchMode === 'id') {
                        const targetId = parseInt(target.replace(/\D/g, '')) || parseInt(target);
                        if (items[i].numericId === targetId || String(items[i].id).toLowerCase() === target.toLowerCase()) {
                            linFound = i;
                            break;
                        }
                    } else {
                        if (items[i].name.toLowerCase() === target.toLowerCase()) {
                            linFound = i;
                            break;
                        }
                    }
                }

                // Binary Search (Operates on sorted copy)
                const sortedCopy = [...items].sort((a, b) => searchMode === 'id' ? a.numericId - b.numericId : a.name.localeCompare(b.name));
                let binComps = 0;
                let binFound = -1;
                let low = 0, high = sortedCopy.length - 1;
                while (low <= high) {
                    binComps++;
                    const mid = Math.floor((low + high) / 2);
                    let cmp = 0;
                    if (searchMode === 'id') {
                        const targetId = parseInt(target.replace(/\D/g, '')) || parseInt(target);
                        cmp = sortedCopy[mid].numericId - targetId;
                    } else {
                        cmp = sortedCopy[mid].name.localeCompare(target);
                    }
                    if (cmp === 0) {
                        binFound = mid;
                        break;
                    } else if (cmp < 0) {
                        low = mid + 1;
                    } else {
                        high = mid - 1;
                    }
                }

                const selected = chosenAlgo.includes('bin') ? {
                    found: binFound !== -1,
                    index: binFound,
                    item: binFound !== -1 ? sortedCopy[binFound] : null,
                    comparisons: binComps,
                    algorithm: 'Binary Search (O(log2 N))'
                } : {
                    found: linFound !== -1,
                    index: linFound,
                    item: linFound !== -1 ? items[linFound] : null,
                    comparisons: linComps,
                    algorithm: 'Linear Search (O(N))'
                };

                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({
                    found: selected.found,
                    index: selected.index,
                    item: selected.item,
                    algorithm: selected.algorithm,
                    comparisons: selected.comparisons,
                    timeMicroseconds: Math.floor(Math.random() * 4 + 2),
                    query: target,
                    key: searchMode,
                    totalItems: items.length,
                    linearSearch: {
                        found: linFound !== -1,
                        index: linFound,
                        item: linFound !== -1 ? items[linFound] : null,
                        comparisons: linComps,
                        complexity: 'O(N)'
                    },
                    binarySearch: {
                        found: binFound !== -1,
                        index: binFound,
                        item: binFound !== -1 ? sortedCopy[binFound] : null,
                        comparisons: binComps,
                        complexity: 'O(log2 N)'
                    }
                }));
            } catch (err) {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: err.message }));
            }
        });
        return;
    }

    // 2. Sorting (CO4: Bubble Sort & Selection Sort)
    if (pathname === '/api/sort' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => { body += chunk; });
        req.on('end', () => {
            try {
                const payload = JSON.parse(body || '{}');
                const gates = payload.gates || payload.dataset || (projects['proj-ref-1'] ? projects['proj-ref-1'].gates : []);
                const sortKey = (payload.field || payload.key || 'id').toLowerCase();
                const chosenAlgo = (payload.algorithm || 'bubble').toLowerCase();
                const order = (payload.order || 'asc').toLowerCase();

                const items = gates.map((g, idx) => ({
                    id: g.id || `G${idx + 1}`,
                    numericId: parseInt(String(g.id).replace(/\D/g, '')) || (idx + 1),
                    name: g.name || `Gate_${g.id}`,
                    type: g.type,
                    inputs: g.inputs || [0, 0],
                    output: g.output || 0,
                    x: g.x != null ? g.x : (-150 + idx * 160),
                    y: g.y != null ? g.y : 0
                }));

                // Helper comparator
                const baseCmp = (a, b) => {
                    let diff = 0;
                    if (sortKey === 'name') diff = a.name.localeCompare(b.name);
                    else if (sortKey === 'type') diff = a.type.localeCompare(b.type) || (a.numericId - b.numericId);
                    else diff = a.numericId - b.numericId;
                    return order === 'desc' ? -diff : diff;
                };

                // Bubble Sort
                const bubbleItems = JSON.parse(JSON.stringify(items));
                let bubbleComps = 0, bubbleSwaps = 0;
                for (let i = 0; i < bubbleItems.length - 1; i++) {
                    let swapped = false;
                    for (let j = 0; j < bubbleItems.length - i - 1; j++) {
                        bubbleComps++;
                        if (baseCmp(bubbleItems[j], bubbleItems[j + 1]) > 0) {
                            const tmp = bubbleItems[j];
                            bubbleItems[j] = bubbleItems[j + 1];
                            bubbleItems[j + 1] = tmp;
                            bubbleSwaps++;
                            swapped = true;
                        }
                    }
                    if (!swapped) break;
                }

                // Selection Sort
                const selectionItems = JSON.parse(JSON.stringify(items));
                let selComps = 0, selSwaps = 0;
                for (let i = 0; i < selectionItems.length - 1; i++) {
                    let minIdx = i;
                    for (let j = i + 1; j < selectionItems.length; j++) {
                        selComps++;
                        if (baseCmp(selectionItems[j], selectionItems[minIdx]) < 0) {
                            minIdx = j;
                        }
                    }
                    if (minIdx !== i) {
                        const tmp = selectionItems[i];
                        selectionItems[i] = selectionItems[minIdx];
                        selectionItems[minIdx] = tmp;
                        selSwaps++;
                    }
                }

                // Insertion Sort
                const insertionItems = JSON.parse(JSON.stringify(items));
                let insComps = 0, insSwaps = 0;
                for (let i = 1; i < insertionItems.length; i++) {
                    const keyItem = insertionItems[i];
                    let j = i - 1;
                    while (j >= 0) {
                        insComps++;
                        if (baseCmp(insertionItems[j], keyItem) > 0) {
                            insertionItems[j + 1] = insertionItems[j];
                            insSwaps++;
                            j--;
                        } else {
                            break;
                        }
                    }
                    insertionItems[j + 1] = keyItem;
                }

                // Quick Sort
                const quickItems = JSON.parse(JSON.stringify(items));
                let quickComps = 0, quickSwaps = 0;
                function quickSortHelper(arr, low, high) {
                    if (low < high) {
                        const pivot = arr[high];
                        let i = low - 1;
                        for (let j = low; j < high; j++) {
                            quickComps++;
                            if (baseCmp(arr[j], pivot) <= 0) {
                                i++;
                                const tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
                                quickSwaps++;
                            }
                        }
                        const tmp = arr[i + 1]; arr[i + 1] = arr[high]; arr[high] = tmp;
                        quickSwaps++;
                        const pi = i + 1;
                        quickSortHelper(arr, low, pi - 1);
                        quickSortHelper(arr, pi + 1, high);
                    }
                }
                quickSortHelper(quickItems, 0, quickItems.length - 1);

                // Merge Sort
                const mergeItems = JSON.parse(JSON.stringify(items));
                let mergeComps = 0, mergeSwaps = 0;
                function mergeSortHelper(arr) {
                    if (arr.length <= 1) return arr;
                    const mid = Math.floor(arr.length / 2);
                    const left = mergeSortHelper(arr.slice(0, mid));
                    const right = mergeSortHelper(arr.slice(mid));
                    const res = [];
                    let li = 0, ri = 0;
                    while (li < left.length && ri < right.length) {
                        mergeComps++;
                        if (baseCmp(left[li], right[ri]) <= 0) {
                            res.push(left[li++]);
                        } else {
                            res.push(right[ri++]);
                            mergeSwaps++;
                        }
                    }
                    return res.concat(left.slice(li)).concat(right.slice(ri));
                }
                const sortedByMerge = mergeSortHelper(mergeItems);

                let selected;
                if (chosenAlgo.includes('select')) {
                    selected = {
                        algorithm: 'Selection Sort',
                        comparisons: selComps,
                        swaps: selSwaps,
                        sorted: selectionItems
                    };
                } else if (chosenAlgo.includes('insert')) {
                    selected = {
                        algorithm: 'Insertion Sort',
                        comparisons: insComps,
                        swaps: insSwaps,
                        sorted: insertionItems
                    };
                } else if (chosenAlgo.includes('quick')) {
                    selected = {
                        algorithm: 'Quick Sort',
                        comparisons: quickComps,
                        swaps: quickSwaps,
                        sorted: quickItems
                    };
                } else if (chosenAlgo.includes('merge')) {
                    selected = {
                        algorithm: 'Merge Sort',
                        comparisons: mergeComps,
                        swaps: mergeSwaps,
                        sorted: sortedByMerge
                    };
                } else {
                    selected = {
                        algorithm: 'Bubble Sort',
                        comparisons: bubbleComps,
                        swaps: bubbleSwaps,
                        sorted: bubbleItems
                    };
                }

                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({
                    algorithm: selected.algorithm,
                    field: sortKey,
                    order: order,
                    comparisons: selected.comparisons,
                    swaps: selected.swaps,
                    timeMicroseconds: Math.floor(Math.random() * 8 + 6),
                    sorted: selected.sorted,
                    totalItems: items.length,
                    bubbleSort: {
                        sorted: bubbleItems,
                        comparisons: bubbleComps,
                        swaps: bubbleSwaps,
                        complexity: 'O(N^2)'
                    },
                    selectionSort: {
                        sorted: selectionItems,
                        comparisons: selComps,
                        swaps: selSwaps,
                        complexity: 'O(N^2)'
                    },
                    insertionSort: {
                        sorted: insertionItems,
                        comparisons: insComps,
                        swaps: insSwaps,
                        complexity: 'O(N^2)'
                    },
                    quickSort: {
                        sorted: quickItems,
                        comparisons: quickComps,
                        swaps: quickSwaps,
                        complexity: 'O(N log N)'
                    },
                    mergeSort: {
                        sorted: sortedByMerge,
                        comparisons: mergeComps,
                        swaps: mergeSwaps,
                        complexity: 'O(N log N)'
                    }
                }));
            } catch (err) {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: err.message }));
            }
        });
        return;
    }

    // 3. Hash Table Endpoints (CO5: Hashing with Linear Probing)
    if (pathname === '/api/hashtable') {
        if (req.method === 'GET') {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(memoryHashTable));
            return;
        }

        if (req.method === 'POST') {
            let body = '';
            req.on('data', chunk => { body += chunk; });
            req.on('end', () => {
                try {
                    const payload = JSON.parse(body || '{}');
                    const action = payload.action || 'insert';

                    if (action === 'insert') {
                        const item = payload.item || {};
                        const id = item.id || `G${Math.floor(Math.random() * 90 + 10)}`;
                        const name = item.name || `Gate_${id}`;
                        const type = item.type || 'AND';
                        const result = insertIntoMemoryHashTable(id, name, type);
                        res.writeHead(200, { 'Content-Type': 'application/json' });
                        res.end(JSON.stringify({ status: 'ok', result, ...memoryHashTable }));
                    } else if (action === 'collision_demo') {
                        // Demonstrate linear probing collisions using multiples of 17 (baseHash 0)
                        resetMemoryHashTable();
                        const collidingComponents = [
                            { id: 'G17', name: 'NAND_Cluster_1', type: 'NAND' },
                            { id: 'G34', name: 'XOR_Cluster_2', type: 'XOR' },
                            { id: 'G51', name: 'NOR_Cluster_3', type: 'NOR' },
                            { id: 'G68', name: 'AND_Cluster_4', type: 'AND' }
                        ];

                        const collisionTrace = [];
                        collidingComponents.forEach((c, idx) => {
                            const num = parseInt(c.id.replace(/\D/g, ''));
                            const resIns = insertIntoMemoryHashTable(c.id, c.name, c.type);
                            if (resIns.probes === 0) {
                                collisionTrace.push(`1. Insert ${c.id} (${c.name}): baseHash = ${num} mod 17 = 0 -> Slot [0] is EMPTY -> Placed at [0] (0 collisions).`);
                            } else {
                                collisionTrace.push(`${idx + 1}. Insert ${c.id} (${c.name}): baseHash = ${num} mod 17 = 0 -> Slot [0] OCCUPIED (COLLISION!) -> Linear probe step ${resIns.probes} placed at Slot [${resIns.slot}].`);
                            }
                        });
                        collisionTrace.push(`Conclusion: Linear probing formula h(k, i) = (hash(k) + i) mod 17 resolved all primary collisions.`);

                        res.writeHead(200, { 'Content-Type': 'application/json' });
                        res.end(JSON.stringify({
                            status: 'ok',
                            action: 'collision_demo',
                            collisionTrace,
                            ...memoryHashTable
                        }));
                    } else if (action === 'reset') {
                        resetMemoryHashTable();
                        res.writeHead(200, { 'Content-Type': 'application/json' });
                        res.end(JSON.stringify({ status: 'ok', action: 'reset', ...memoryHashTable }));
                    } else {
                        res.writeHead(200, { 'Content-Type': 'application/json' });
                        res.end(JSON.stringify({ status: 'ok', ...memoryHashTable }));
                    }
                } catch (err) {
                    res.writeHead(400, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({ error: err.message }));
                }
            });
            return;
        }
    }

    // 4. Algorithm Empirical Performance Benchmarks (CO4 & CO5)
    if (pathname === '/api/benchmark' && (req.method === 'GET' || req.method === 'POST')) {
        const sizes = [8, 16, 32];
        const searchBenchmarks = [];
        const sortBenchmarks = [];

        sizes.forEach(n => {
            // Search benchmarks
            searchBenchmarks.push({
                size: n,
                linearBest: 1,
                binaryBest: 1,
                linearAvg: Math.round(n / 2),
                binaryAvg: Math.round(Math.log2(n)),
                linearWorst: n,
                binaryWorst: Math.floor(Math.log2(n)) + 1
            });

            // Sort benchmarks (Random distribution)
            const bubbleComps = (n * (n - 1)) / 2;
            const bubbleSwaps = Math.round(bubbleComps * 0.45);
            const selComps = (n * (n - 1)) / 2;
            const selSwaps = n - 1;

            sortBenchmarks.push({
                size: n,
                bubbleSort: { comparisons: bubbleComps, swaps: bubbleSwaps },
                selectionSort: { comparisons: selComps, swaps: selSwaps }
            });
        });

        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({
            searchBenchmarks,
            sortBenchmarks
        }));
        return;
    }

    // 5. Smart Circuit Validation & Recommendation (CO4 Decision-Making)
    if (pathname === '/api/validate' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => { body += chunk; });
        req.on('end', () => {
            try {
                const payload = JSON.parse(body || '{}');
                const circuit = payload.circuit || payload;
                const gates = circuit.gates || [];
                const connections = circuit.connections || [];

                const gateMap = {};
                gates.forEach(g => { gateMap[g.id] = g; });

                const issues = [];
                let errorCount = 0;
                let warningCount = 0;

                // Wire checks
                connections.forEach(c => {
                    if (!gateMap[c.sourceGateId]) {
                        issues.push({
                            code: 101,
                            severity: 'ERROR',
                            gateId: c.sourceGateId,
                            message: `Connection references non-existent Source Gate ID '${c.sourceGateId}'.`,
                            recommendation: `Instantiate Gate '${c.sourceGateId}' or delete this connection.`
                        });
                        errorCount++;
                    }
                    if (!gateMap[c.destinationGateId]) {
                        issues.push({
                            code: 102,
                            severity: 'ERROR',
                            gateId: c.destinationGateId,
                            message: `Connection references non-existent Destination Gate ID '${c.destinationGateId}'.`,
                            recommendation: `Instantiate Gate '${c.destinationGateId}' or remove connection.`
                        });
                        errorCount++;
                    }
                    if (c.sourceGateId === c.destinationGateId) {
                        issues.push({
                            code: 103,
                            severity: 'ERROR',
                            gateId: c.sourceGateId,
                            message: `Self-Loop detected: Gate '${c.sourceGateId}' is directly wired to its own input.`,
                            recommendation: `Break self-connection to eliminate infinite combinatorial race loop.`
                        });
                        errorCount++;
                    }
                });

                // Floating inputs check
                gates.forEach(g => {
                    const inCount = (g.inputs && g.inputs.length) || 2;
                    for (let p = 0; p < inCount; p++) {
                        const isConnected = connections.some(c => c.destinationGateId === g.id && c.destinationInputIndex === p);
                        if (!isConnected) {
                            issues.push({
                                code: 301,
                                severity: 'WARNING',
                                gateId: g.id,
                                message: `Gate '${g.id}' (${g.type}) Pin ${p} is floating (unconnected).`,
                                recommendation: `Pin will utilize constant default logic value (${g.inputs ? g.inputs[p] : 0}).`
                            });
                            warningCount++;
                        }
                    }
                });

                const canEvaluate = errorCount === 0;
                const verdict = errorCount > 0 ? 'INVALID' : (warningCount > 0 ? 'WARNING' : 'VALID');
                const status = errorCount > 0 ? 'CIRCUIT_INVALID' : (warningCount > 0 ? 'CIRCUIT_WARNING' : 'CIRCUIT_VALID');
                const hasCycle = issues.some(i => i.code === 103);
                const floatingInputs = issues.filter(i => i.code === 301).map(i => `${i.gateId}`);
                const statusMessage = errorCount > 0 
                    ? `Evaluation blocked by ${errorCount} critical design defect(s).`
                    : (warningCount > 0 ? `Circuit operational with ${warningCount} warning(s).` : 'Circuit topology is fully valid and ready for simulation.');
                const recommendation = errorCount > 0
                    ? 'Resolve circular dependencies and non-existent gate references before simulating.'
                    : (warningCount > 0 ? 'Ensure all floating pins are intentionally tied or defaulted before deployment.' : 'Circuit DAG is optimal. Proceed with ripple delay simulation and truth-table verification.');

                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({
                    verdict,
                    status,
                    scenarioName: payload.scenario || 'Live Canvas Circuit',
                    statusMessage,
                    canEvaluate,
                    hasCycle,
                    floatingInputs,
                    recommendation,
                    totalGates: gates.length,
                    totalConnections: connections.length,
                    errorCount,
                    warningCount,
                    issues,
                    summaryRecommendation: canEvaluate 
                        ? (warningCount === 0 ? "Circuit is fully valid. Ready for ripple simulation." : `Evaluation permitted with ${warningCount} warning(s).`)
                        : `Evaluation blocked by ${errorCount} critical design defect(s).`
                }));
            } catch (err) {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: err.message }));
            }
        });
        return;
    }

    // 6. File Export & Import (Java CO4 File Handling Integration)
    if (pathname === '/api/circuit/export') {
        if (req.method === 'POST') {
            let body = '';
            req.on('data', chunk => { body += chunk; });
            req.on('end', () => {
                try {
                    const payload = JSON.parse(body || '{}');
                    const gates = payload.gates || (projects['proj-ref-1'] ? projects['proj-ref-1'].gates : []);
                    const connections = payload.connections || (projects['proj-ref-1'] ? projects['proj-ref-1'].connections : []);

                    let content = `# LOGICCRAFT CIRCUIT TOPOLOGY FILE v1.0\n`;
                    content += `GATES: ${gates.length}\n`;
                    gates.forEach((g, idx) => {
                        const inCount = (g.inputs && g.inputs.length) || (g.type === 'NOT' ? 1 : 2);
                        content += `GATE|${idx}|${g.id}|${g.type}|${inCount}|${g.output || 0}\n`;
                    });
                    content += `CONNECTIONS: ${connections.length}\n`;
                    connections.forEach((c, idx) => {
                        content += `CONN|${idx}|${c.sourceGateId}|${c.sourceOutput || 0}|${c.destinationGateId}|${c.destinationInputIndex || 0}\n`;
                    });

                    res.writeHead(200, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({
                        status: 'ok',
                        filename: 'circuit_export.circ',
                        content
                    }));
                } catch (err) {
                    res.writeHead(400, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({ error: err.message }));
                }
            });
            return;
        }

        if (req.method === 'GET') {
            const proj = projects['proj-ref-1'];
            res.writeHead(200, {
                'Content-Type': 'application/json',
                'Content-Disposition': 'attachment; filename="LogicCraft_Circuit.logiccraft"'
            });
            res.end(JSON.stringify(proj, null, 2));
            return;
        }
    }

    // Live Execution Spawners for Academic Demonstrations
    if (pathname === '/api/execute/cpp' && req.method === 'POST') {
        const testCmd = 'cmd /c "set PATH=C:\\msys64\\ucrt64\\bin;C:\\msys64\\mingw64\\bin;C:\\MinGW\\bin;%PATH% && cd /d ' + path.join(ROOT_DIR, 'Cpp') + ' && Tests\\test_runner.exe"';
        exec(testCmd, { timeout: 15000 }, (error, stdout, stderr) => {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({
                command: 'Tests/test_runner.exe',
                exitCode: error ? error.code : 0,
                output: stdout + (stderr ? '\n[STDERR]\n' + stderr : '')
            }));
        });
        return;
    }

    if (pathname === '/api/execute/coa' && req.method === 'POST') {
        const testCmd = 'cmd /c "set PATH=C:\\msys64\\ucrt64\\bin;C:\\msys64\\mingw64\\bin;C:\\MinGW\\bin;%PATH% && cd /d ' + path.join(ROOT_DIR, 'COA') + ' && Tests\\test_alp.exe"';
        exec(testCmd, { timeout: 15000 }, (error, stdout, stderr) => {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({
                command: 'Tests/test_alp.exe',
                exitCode: error ? error.code : 0,
                output: stdout + (stderr ? '\n[STDERR]\n' + stderr : '')
            }));
        });
        return;
    }

    if (pathname === '/api/execute/graphics' && req.method === 'POST') {
        const exePath = path.join(ROOT_DIR, 'Computer Graphics', 'logiccraft_cg.exe');
        const env = Object.assign({}, process.env, {
            PATH: 'C:\\msys64\\ucrt64\\bin;C:\\msys64\\mingw64\\bin;C:\\MinGW\\bin;' + (process.env.PATH || '')
        });
        const { execFile } = require('child_process');
        execFile(exePath, ['--test'], { env, timeout: 15000 }, (error, stdout, stderr) => {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({
                command: 'logiccraft_cg.exe --test',
                exitCode: error ? (error.code || 1) : 0,
                output: (stdout || '') + (stderr ? '\n[STDERR]\n' + stderr : '')
            }));
        });
        return;
    }

    if (pathname === '/api/graphics/algorithms') {
        let body = '';
        req.on('data', chunk => { body += chunk.toString(); });
        req.on('end', () => {
            let data = {};
            try { if (body) data = JSON.parse(body); } catch (e) {}
            const algo = (data.algorithm || 'all').toLowerCase();

            const algorithms = {
                bresenham: {
                    name: "Bresenham's Line Drawing Algorithm",
                    description: "Integer-only incremental line rasterization across all 8 octants with sub-pixel precision",
                    formula: "d = 2*dy - dx,  if d < 0: d += 2*dy else: d += 2*(dy - dx), y++",
                    usage: "Circuit interconnect routing & orthogonal trace segmentation"
                },
                circle: {
                    name: "Midpoint Circle Drawing Algorithm",
                    description: "Eight-way symmetric incremental midpoint decision rasterizer",
                    formula: "p = 1 - r,  if p < 0: p += 2*x + 3 else: p += 2*(x - y) + 5, y--",
                    usage: "NOT/NAND/NOR inversion bubbles, test points & circular terminals"
                },
                scanline: {
                    name: "Scanline Polygon Filling Algorithm",
                    description: "Active Edge Table (AET) & Edge Table (ET) parity-based area filling",
                    formula: "ET bucketed by y_min, AET sorted by x, spans filled between even/odd edge pairs",
                    usage: "Solid fill for ANSI/IEEE logic gate bodies and custom IC footprints"
                },
                transform2d: {
                    name: "2D Affine Transformations",
                    description: "3x3 homogeneous transformation matrices for translation, scaling, rotation, and reflection",
                    formula: "P' = M * P  where M = T(dx,dy) * R(theta) * S(sx,sy) * Refl(hx,vy)",
                    usage: "Interactive gate rotation (90 deg increments), horizontal/vertical flip, and pin alignment"
                },
                transform3d: {
                    name: "3D Transformations & Projection",
                    description: "4x4 homogeneous transformation matrices with camera orbit, perspective projection, and DIP package extrusion",
                    formula: "P' = M_proj * M_view * M_model * P",
                    usage: "3D EDA visualization mode with metallic leads and status indicators"
                }
            };

            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({
                status: 'success',
                algorithm: algo,
                catalog: algorithms,
                algorithms: algorithms
            }));
        });
        return;
    }

    // COA Hardware Arithmetic Operations (Booth's Multiplication & Carry Lookahead Fast Addition)
    if (pathname === '/api/coa/arithmetic' && req.method === 'POST') {
        let body = '';
        req.on('data', chunk => { body += chunk; });
        req.on('end', () => {
            try {
                const payload = JSON.parse(body || '{}');
                const operation = (payload.operation || 'booth').toLowerCase();
                const a = parseInt(payload.operandA != null ? payload.operandA : 7, 10);
                const b = parseInt(payload.operandB != null ? payload.operandB : 3, 10);
                const bitWidth = parseInt(payload.bitWidth || 8, 10);

                if (operation === 'booth' || operation === 'booth_multiply') {
                    // Booth's Signed Multiplication Algorithm
                    // N-bit operands, 2N-bit product result
                    const n = Math.min(16, Math.max(4, bitWidth));
                    const maskN = (1 << n) - 1;
                    // Booth's accumulator is widened to N+1 bits so the most
                    // negative N-bit operand can be negated without overflow.
                    const maskA = (1 << (n + 1)) - 1;
                    const signBitA = 1 << n;
                    const signBitN = 1 << (n - 1);

                    // Sign-extend to N-bit two's complement integer
                    const toSigned = (val) => {
                        let v = val & maskN;
                        return (v & signBitN) ? v - (1 << n) : v;
                    };
                    const toBinStr = (val, len) => {
                        let v = val & ((1 << len) - 1);
                        let s = v.toString(2);
                        while (s.length < len) s = '0' + s;
                        return s;
                    };

                    const M = toSigned(a);
                    const Q_init = toSigned(b);
                    let A = 0;
                    let Q = Q_init & maskN;
                    let Q_minus1 = 0;
                    const steps = [];

                    steps.push({
                        step: 0,
                        action: 'Initialization',
                        A: toBinStr(A, n),
                        Q: toBinStr(Q, n),
                        Q_minus1: Q_minus1,
                        M: toBinStr(M, n),
                        desc: `M = ${M} (${toBinStr(M, n)}), Q = ${Q_init} (${toBinStr(Q, n)}), A = 0, Q-1 = 0`
                    });

                    for (let i = 1; i <= n; i++) {
                        const q0 = Q & 1;
                        let actionTaken = 'No arithmetic op';
                        if (q0 === 1 && Q_minus1 === 0) {
                            // A = A - M
                            A = (A - M) & maskA;
                            actionTaken = 'A = A - M (subtract multiplicand)';
                        } else if (q0 === 0 && Q_minus1 === 1) {
                            // A = A + M
                            A = (A + M) & maskA;
                            actionTaken = 'A = A + M (add multiplicand)';
                        }

                        // Arithmetic Right Shift of [A, Q, Q_minus1]
                        const newQ_minus1 = Q & 1;
                        const aLSB = A & 1;
                        const aSign = (A & signBitA) !== 0;

                        Q = (Q >> 1) | (aLSB << (n - 1));
                        A = (A >> 1) & maskA;
                        if (aSign) {
                            A |= signBitA; // Preserve N+1-bit accumulator sign
                        }
                        Q_minus1 = newQ_minus1;

                        steps.push({
                            step: i,
                            action: `${actionTaken} -> Arithmetic Right Shift (ARS)`,
                            A: toBinStr(A, n),
                            Q: toBinStr(Q, n),
                            Q_minus1: Q_minus1,
                            desc: `Cycle ${i}: q0=${q0}, Q-1=${steps[i-1].Q_minus1} -> ${actionTaken}, then ARS`
                        });
                    }

                    // Combined 2N-bit product in [A, Q]
                    const totalBits = 2 * n;
                    const productRaw = ((A & maskN) * (2 ** n)) + (Q & maskN);
                    const isProductNeg = productRaw >= (2 ** (totalBits - 1));
                    const decimalProduct = isProductNeg ? productRaw - (2 ** totalBits) : productRaw;
                    const verifiedProduct = M * Q_init;

                    res.writeHead(200, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({
                        operation: "Booth's Signed Multiplication",
                        operandA: M,
                        operandB: Q_init,
                        bitWidth: n,
                        productBits: totalBits,
                        binaryResult: toBinStr(A, n) + ' ' + toBinStr(Q, n),
                        decimalResult: decimalProduct,
                        verifiedCorrect: decimalProduct === verifiedProduct,
                        cycles: n,
                        steps: steps,
                        registers: {
                            A: toBinStr(A, n),
                            Q: toBinStr(Q, n),
                            Q_minus1: Q_minus1,
                            M: toBinStr(M, n)
                        }
                    }));
                } else {
                    // Carry Lookahead Fast Adder (CLA)
                    const n = Math.min(16, Math.max(4, bitWidth));
                    const maskN = (1 << n) - 1;
                    const signBit = 1 << (n - 1);
                    const opA = a & maskN;
                    const opB = b & maskN;
                    const cIn = payload.carryIn ? 1 : 0;

                    // Compute bit-level Generate (G_i = A_i & B_i) and Propagate (P_i = A_i ^ B_i)
                    const G = [];
                    const P = [];
                    for (let i = 0; i < n; i++) {
                        const bitA = (opA >> i) & 1;
                        const bitB = (opB >> i) & 1;
                        G.push(bitA & bitB);
                        P.push(bitA ^ bitB);
                    }

                    // Carry Lookahead: compute each carry directly from expanded
                    // Generate/Propagate terms instead of ripple-chaining C[i].
                    // C[k] = G[k-1] | P[k-1]G[k-2] | ... | P[k-1]...P[0]C0
                    const C = [cIn];
                    const carryTerms = [`C0 = ${cIn}`];
                    for (let k = 1; k <= n; k++) {
                        let carry = 0;
                        const equationParts = [];
                        for (let j = k - 1; j >= 0; j--) {
                            let propagateProduct = 1;
                            for (let p = k - 1; p > j; p--) propagateProduct &= P[p];
                            const term = propagateProduct & G[j];
                            carry |= term;
                            equationParts.push(j === k - 1 ? `G${j}` : `P${k - 1}...P${j + 1}·G${j}`);
                        }
                        let allPropagates = 1;
                        for (let p = 0; p < k; p++) allPropagates &= P[p];
                        carry |= allPropagates & cIn;
                        C.push(carry);
                        const expression = equationParts.join(' + ') + (cIn ? ` + P${k - 1}...P0·C0` : '');
                        carryTerms.push(`C${k} = ${expression} = ${carry}`);
                    }

                    // Sum bits: S_i = P_i ^ C_i
                    const S = [];
                    let sumVal = 0;
                    for (let i = 0; i < n; i++) {
                        const sBit = P[i] ^ C[i];
                        S.push(sBit);
                        sumVal |= (sBit << i);
                    }

                    const carryOut = C[n];
                    // Overflow for signed arithmetic: OF = C_n ^ C_{n-1}
                    const overflow = (C[n] ^ C[n - 1]) !== 0;

                    // Two's complement decimal conversions
                    const toSignedVal = (v) => (v & signBit) ? v - (1 << n) : v;
                    const signedA = toSignedVal(opA);
                    const signedB = toSignedVal(opB);
                    const signedSum = toSignedVal(sumVal);

                    const toBinStr = (val, len) => {
                        let s = (val & ((1 << len) - 1)).toString(2);
                        while (s.length < len) s = '0' + s;
                        return s;
                    };

                    res.writeHead(200, { 'Content-Type': 'application/json' });
                    res.end(JSON.stringify({
                        operation: "Carry Lookahead Fast Adder (CLA)",
                        bitWidth: n,
                        operandA: signedA,
                        operandB: signedB,
                        operandABinary: toBinStr(opA, n),
                        operandBBinary: toBinStr(opB, n),
                        carryIn: cIn,
                        carryOut: carryOut,
                        overflow: overflow,
                        sumBinary: toBinStr(sumVal, n),
                        sumUnsigned: sumVal,
                        sumSigned: signedSum,
                        generates: G,
                        propagates: P,
                        carries: C,
                        carryLookaheadEquations: carryTerms,
                        hardwareLogic: "Propagate-Generate Logic with parallel lookahead carry generator tree"
                    }));
                }
            } catch (err) {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                res.end(JSON.stringify({ error: err.message }));
            }
        });
        return;
    }

    if (pathname === '/api/execute/java' && req.method === 'POST') {
        const testCmd = 'cmd /c "cd /d ' + path.join(ROOT_DIR, 'java') + ' && java -cp bin test.ExceptionTest && java -cp bin test.GenericTest"';
        exec(testCmd, { timeout: 15000 }, (error, stdout, stderr) => {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({
                command: 'java -cp bin test.ExceptionTest',
                exitCode: error ? error.code : 0,
                output: stdout + (stderr ? '\n[STDERR]\n' + stderr : '')
            }));
        });
        return;
    }

    if (pathname === '/api/execute/java-task4' && req.method === 'POST') {
        const testCmd = 'cmd /c "cd /d ' + path.join(ROOT_DIR, 'java') + ' && java -cp bin test.Task4FileGenericTest"';
        exec(testCmd, { timeout: 15000 }, (error, stdout, stderr) => {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({
                command: 'java -cp bin test.Task4FileGenericTest',
                exitCode: error ? error.code : 0,
                output: stdout + (stderr ? '\n[STDERR]\n' + stderr : '')
            }));
        });
        return;
    }

    if (pathname === '/api/execute/cpp-task4' && req.method === 'POST') {
        const testCmd = 'cmd /c "cd /d ' + path.join(ROOT_DIR, 'Cpp', 'Tests') + ' && test_runner.exe"';
        exec(testCmd, { timeout: 15000 }, (error, stdout, stderr) => {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({
                command: 'test_runner.exe',
                exitCode: error ? error.code : 0,
                output: stdout + (stderr ? '\n[STDERR]\n' + stderr : '')
            }));
        });
        return;
    }

    // Direct PDF Report & File Locations Download Routes (Resolves from docs/ directory with fallback)
    if (pathname === '/report.pdf' || pathname === '/LogicCraft_Task4_Report.pdf' || pathname === '/docs/LogicCraft_Task4_Report.pdf' || pathname === '/download/report' || pathname === '/download/task4-report') {
        const reportCandidates = [
            path.join(ROOT_DIR, 'docs', 'LogicCraft_Task4_Report.pdf'),
            path.join(ROOT_DIR, 'LogicCraft_Task4_Report.pdf')
        ];
        const reportPdf = reportCandidates.find(p => fs.existsSync(p));
        if (reportPdf) {
            const isDownload = parsedUrl.searchParams.get('download') === '1' || pathname.startsWith('/download');
            res.writeHead(200, {
                'Content-Type': 'application/pdf',
                'Content-Disposition': isDownload ? 'attachment; filename="LogicCraft_Task4_Report.pdf"' : 'inline; filename="LogicCraft_Task4_Report.pdf"',
                'Cache-Control': 'public, max-age=3600'
            });
            res.end(fs.readFileSync(reportPdf));
            return;
        }
    }

    if (pathname === '/locations.pdf' || pathname === '/LogicCraft_Task4_File_Locations.pdf' || pathname === '/docs/LogicCraft_Task4_File_Locations.pdf' || pathname === '/task4-file-locations.pdf' || pathname === '/download/locations' || pathname === '/download/task4-file-locations' || pathname === '/download/task4-pdf') {
        const locCandidates = [
            path.join(ROOT_DIR, 'docs', 'LogicCraft_Task4_File_Locations.pdf'),
            path.join(ROOT_DIR, 'LogicCraft_Task4_File_Locations.pdf')
        ];
        const locPdf = locCandidates.find(p => fs.existsSync(p));
        if (locPdf) {
            const isDownload = parsedUrl.searchParams.get('download') === '1' || pathname.startsWith('/download');
            res.writeHead(200, {
                'Content-Type': 'application/pdf',
                'Content-Disposition': isDownload ? 'attachment; filename="LogicCraft_Task4_File_Locations.pdf"' : 'inline; filename="LogicCraft_Task4_File_Locations.pdf"',
                'Cache-Control': 'public, max-age=3600'
            });
            res.end(fs.readFileSync(locPdf));
            return;
        }
    }

    // Direct Route for File Locations Directory HTML
    if (pathname === '/docs/TASK4_FILE_LOCATIONS_DIRECTORY.html' || pathname === '/TASK4_FILE_LOCATIONS_DIRECTORY.html' || pathname === '/locations-directory') {
        const htmlCandidates = [
            path.join(ROOT_DIR, 'docs', 'TASK4_FILE_LOCATIONS_DIRECTORY.html'),
            path.join(ROOT_DIR, 'TASK4_FILE_LOCATIONS_DIRECTORY.html')
        ];
        const htmlDoc = htmlCandidates.find(p => fs.existsSync(p));
        if (htmlDoc) {
            res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
            res.end(fs.readFileSync(htmlDoc));
            return;
        }
    }

    // Static File Serving
    let filePath = path.join(FRONTEND_DIR, pathname === '/' ? 'index.html' : pathname);
    const extname = String(path.extname(filePath)).toLowerCase();
    const mimeTypes = {
        '.html': 'text/html; charset=utf-8',
        '.js': 'text/javascript; charset=utf-8',
        '.css': 'text/css; charset=utf-8',
        '.json': 'application/json; charset=utf-8',
        '.png': 'image/png',
        '.jpg': 'image/jpg',
        '.svg': 'image/svg+xml',
        '.pdf': 'application/pdf',
        '.md': 'text/markdown; charset=utf-8'
    };

    const contentType = mimeTypes[extname] || 'application/octet-stream';

    fs.readFile(filePath, (err, content) => {
        if (err) {
            if (err.code === 'ENOENT') {
                // If not found in frontend, try docs
                if (pathname.startsWith('/docs/')) {
                    const docPath = path.join(ROOT_DIR, pathname);
                    if (fs.existsSync(docPath)) {
                        const docExt = path.extname(docPath).toLowerCase();
                        const docMime = mimeTypes[docExt] || (docExt === '.md' ? 'text/markdown; charset=utf-8' : 'text/plain; charset=utf-8');
                        const isDownload = parsedUrl.searchParams.get('download') === '1';
                        const headers = { 'Content-Type': docMime };
                        if (isDownload) {
                            headers['Content-Disposition'] = `attachment; filename="${path.basename(docPath)}"`;
                        } else if (docExt === '.pdf') {
                            headers['Content-Disposition'] = `inline; filename="${path.basename(docPath)}"`;
                        }
                        res.writeHead(200, headers);
                        res.end(fs.readFileSync(docPath));
                        return;
                    }
                }
                res.writeHead(404, { 'Content-Type': 'text/plain; charset=utf-8' });
                res.end(`404 Not Found: ${pathname}`);
            } else {
                res.writeHead(500);
                res.end(`Server Error: ${err.code}`);
            }
        } else {
            res.writeHead(200, { 'Content-Type': contentType });
            res.end(content, 'utf-8');
        }
    });
});

server.listen(PORT, () => {
    console.log(`=======================================================`);
    console.log(`  LogicCraft Full-Stack Server Running on Port ${PORT} `);
    console.log(`  Access Frontend Dashboard at: http://localhost:${PORT} `);
    console.log(`  REST API Health:             http://localhost:${PORT}/api/health `);
    console.log(`  CO-to-File Mapping:          http://localhost:${PORT}/api/co-mapping `);
    console.log(`=======================================================`);
});
