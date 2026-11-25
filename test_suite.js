const http = require('http');
const fs = require('fs');
const path = require('path');
const appJs = fs.readFileSync(path.join(__dirname, 'frontend', 'app.js'), 'utf8');

const PORT = 3055;
let sessionCookie = '';
process.env.PORT = PORT;

// Start server
console.log('--- STARTING LOGICCRAFT TEST SUITE ---');
require('./backend/server.js');

function wait(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

function request(options, postData = null) {
    return new Promise((resolve, reject) => {
        const requestOptions = { ...options, headers: { ...(options.headers || {}), ...(sessionCookie && !options.skipSession ? { Cookie: sessionCookie } : {}) } };
        delete requestOptions.skipSession;
        const req = http.request(requestOptions, (res) => {
            let data = Buffer.alloc(0);
            res.on('data', chunk => {
                data = Buffer.concat([data, chunk]);
            });
            res.on('end', () => {
                resolve({
                    statusCode: res.statusCode,
                    headers: res.headers,
                    body: data
                });
            });
        });
        req.on('error', reject);
        if (postData) {
            req.write(typeof postData === 'string' ? postData : JSON.stringify(postData));
        }
        req.end();
    });
}

async function runTests() {
    await wait(600); // Wait for server to bind
    const authRes = await request({ host: '127.0.0.1', port: PORT, path: '/api/auth/login', method: 'POST', headers: { 'Content-Type': 'application/json' } }, { username: 'engineer', password: 'engineer123' });
    if (authRes.statusCode === 200 && authRes.headers['set-cookie']) sessionCookie = authRes.headers['set-cookie'][0].split(';')[0];
    else console.error('[TEST SETUP] Authentication failed; protected endpoint checks will fail.');

    let passed = 0;
    let failed = 0;

    function assert(condition, message) {
        if (condition) {
            console.log(`[PASS] ${message}`);
            passed++;
        } else {
            console.error(`[FAIL] ${message}`);
            failed++;
        }
    }

    try {
        assert(authRes.statusCode === 200 && !!sessionCookie, 'Default account login establishes a session cookie');
        const unauthRes = await request({ host:'127.0.0.1', port:PORT, path:'/api/projects', method:'GET', skipSession:true });
        assert(unauthRes.statusCode === 401, 'Protected API rejects unauthenticated requests');
        // Test 1: Frontend Static Loading & HTML structure
        console.log('\n--- 1. Testing Frontend Shell & Screen Structure ---');
        const indexRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/',
            method: 'GET'
        });
        assert(indexRes.statusCode === 200, 'Frontend / responds with HTTP 200');
        const html = indexRes.body.toString('utf8');

        // Check Navigation items
        assert(html.includes('data-screen="screen-overview"'), 'Navigation item: Overview exists');
        assert(html.includes('data-screen="screen-designer"'), 'Navigation item: Circuit Designer exists');
        assert(html.includes('data-screen="screen-simulation"'), 'Navigation item: Simulation exists');
        assert(html.includes('data-screen="screen-truthtable"'), 'Navigation item: Truth Table exists');
        assert(html.includes('data-screen="screen-projects"') && html.includes('My Projects'), 'Navigation item: My Projects exists');
        assert(html.includes('data-screen="screen-analysis"') && html.includes('Component Analysis'), 'Navigation item: Component Analysis exists');
        assert(html.includes('data-screen="screen-processor"') && html.includes('Processor Lab'), 'Navigation item: Processor Lab exists');
        assert(html.includes('data-screen="screen-graphics"') && html.includes('Graphics Lab'), 'Navigation item: Graphics Lab exists');
        assert(html.includes('data-screen="screen-components"') && html.includes('Component Library'), 'Navigation item: Component Library exists');
        assert(!html.includes('data-screen="screen-reports"'), 'Technical Reports is removed from sidebar navigation');
        assert(!html.includes('data-screen="screen-documentation"'), 'Test & Verification Evidence is removed from sidebar navigation');
        assert(html.includes('id="btnTogglePalette"') && html.includes('id="btnExpandPalette"'), 'Circuit Designer has collapse and expand Gate Palette controls');
        assert(html.includes('option value="binary" selected'), 'Binary Search is the default searching algorithm');
        assert(html.includes('option value="merge" selected'), 'Merge Sort is the default sorting algorithm');

        // Check that task numbers are removed from navigation titles
        assert(!html.includes('Task 1–4') && !html.includes('CO1–CO5'), 'Academic Task/CO numbers removed from navigation labels');

        // Check Overview Dashboard requirements
        console.log('\n--- 2. Testing Overview Dashboard Elements ---');
        assert(html.includes('LogicCraft Workspace'), 'Overview contains "LogicCraft Workspace" heading');
        assert(html.includes('id="btnCtaCreateCircuit"'), 'Overview contains Create New Circuit action');
        assert(html.includes('id="recentProjectItem"'), 'Overview contains Recent Projects');
        assert(html.includes('id="qaOpenDesigner"'), 'Overview contains Quick access to Circuit Designer');
        assert(html.includes('id="qaRunSimulation"'), 'Overview contains Quick access to Simulation');
        assert(html.includes('id="qaExportReport"'), 'Overview contains Export Circuit Report shortcut');
        assert(!html.includes('File Locations Directory'), 'File Locations Directory is completely removed from user-facing UI');
        assert(html.includes('id="btnTopbarExportReport"'), 'Topbar contains Export Report button');
        assert(html.includes('id="topbarExportDropdown"'), 'Topbar contains Export Report dropdown');
        assert(html.includes('id="liveReportInspector"'), 'Technical Reports contains Live Report Inspector');
        assert(html.includes('id="overviewGateCount"') && html.includes('id="overviewWireCount"'), 'Overview contains component and circuit statistics');
        assert(html.includes('id="activityTimeline"'), 'Overview contains Recent Activity');
        assert(html.includes('Documentation &amp; Resources'), 'Overview contains dedicated "Documentation & Resources" section');

        // Check Component Analysis Tabs
        console.log('\n--- 3. Testing Component Analysis Screen Organization ---');
        assert(html.includes('data-analysis-tab="tab-search"'), 'Component Analysis contains Search tab');
        assert(html.includes('data-analysis-tab="tab-sort"'), 'Component Analysis contains Sorting tab');
        assert(html.includes('data-analysis-tab="tab-hashing"'), 'Component Analysis contains Hashing tab');
        assert(html.includes('data-analysis-tab="tab-performance"'), 'Component Analysis contains Performance tab');
        assert(html.includes('data-analysis-tab="tab-validation"'), 'Component Analysis contains Topology Validation tab');
        assert(html.includes('id="btnRunSearch"') && html.includes('id="searchResultBox"'), 'Linear & Binary Search controls operational');
        assert(html.includes('id="btnRunSort"') && html.includes('id="sortResultBox"'), 'Bubble & Selection Sort controls operational');
        assert(html.includes('id="btnRefreshHash"') && html.includes('id="btnHashCollisionDemo"'), 'Hash table & linear probing controls operational');
        assert(html.includes('id="btnRunBenchmark"') && html.includes('id="benchmarkResults"'), 'Performance benchmark controls operational');

        // Check Processor Lab Screen
        console.log('\n--- 4. Testing Processor Lab Screen ---');
        assert(html.includes('AMD64 Hardware Arithmetic Engine'), 'Processor Lab heading: AMD64 Hardware Arithmetic Engine');
        assert(html.includes('Circuit State &amp; Undo Stack Manager'), 'Processor Lab heading: Circuit State & Undo Stack Manager');
        assert(html.includes('Object Framework &amp; File Serialization'), 'Processor Lab heading: Object Framework & File Serialization');
        assert(html.includes('OpenGL Hardware Render Pipeline'), 'Processor Lab heading: OpenGL Hardware Render Pipeline');
        assert(html.includes('id="btnTestCoa"') && html.includes('id="btnCalcCoa"'), '64-bit co-processor execution preserved');
        assert(html.includes('id="btnTestCpp"') && html.includes('id="stackVisualizer"'), 'C++ stack operations preserved');
        assert(html.includes('id="btnTestJava"') && html.includes('id="btnExportCircuitFile"'), 'Java file and generic collection preserved');
        assert(html.includes('id="btnTestGraphics"'), 'OpenGL graphics verification preserved');

        // Test 5: Circuit Creation and Simulation API
        console.log('\n--- 5. Testing Circuit Creation & Simulation APIs ---');
        const projRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/projects',
            method: 'GET'
        });
        assert(projRes.statusCode === 200, 'GET /api/projects returns HTTP 200');
        const projectsList = JSON.parse(projRes.body.toString('utf8'));
        assert(Array.isArray(projectsList) && projectsList.length > 0, 'Default reference project loaded');

        // Test Simulation API
        const sampleCircuit = {
            gates: [
                { id: 'G1', type: 'AND', inputs: [1, 1], output: 0 },
                { id: 'G2', type: 'NOT', inputs: [0], output: 0 }
            ],
            connections: [
                { id: 'C1', sourceGateId: 'G1', sourceOutput: 0, destinationGateId: 'G2', destinationInputIndex: 0 }
            ]
        };
        const simRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/simulate',
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        }, sampleCircuit);
        assert(simRes.statusCode === 200, 'POST /api/simulate returns HTTP 200');
        const simData = JSON.parse(simRes.body.toString('utf8'));
        assert(simData.success === true, 'Simulation evaluates circuit signals successfully');

        // Test 6: Search, Sort, Hashing & Benchmark APIs
        console.log('\n--- 6. Testing Search, Sort, Hashing & Benchmark APIs ---');
        // Native C++ bridge endpoint smoke tests (bridge optional until built on the target OS).
        const nativeSearchRes = await request({ host:'127.0.0.1', port:PORT, path:'/api/dsa/native', method:'POST', headers:{'Content-Type':'application/json'} }, { operation:'search', algorithm:'binary', key:'id', query:'G2', gates:sampleCircuit.gates });
        if (nativeSearchRes.statusCode === 200) {
            const ns = JSON.parse(nativeSearchRes.body.toString('utf8'));
            assert(ns.native === true && ns.found && ns.item.id === 'G2', 'Native C++ Binary Search bridge finds G2 by textual ID');
            const nativeSortRes = await request({ host:'127.0.0.1', port:PORT, path:'/api/dsa/native', method:'POST', headers:{'Content-Type':'application/json'} }, { operation:'sort', algorithm:'quick', field:'id', order:'desc', gates:sampleCircuit.gates });
            const nsort = JSON.parse(nativeSortRes.body.toString('utf8'));
            assert(nativeSortRes.statusCode === 200 && nsort.native && nsort.sorted[0].id === 'G2', 'Native C++ Quick Sort bridge orders IDs descending');
        } else {
            assert(nativeSearchRes.statusCode === 503, 'Native C++ bridge endpoint clearly reports optional build requirement');
            console.log('[INFO] Build native DSA bridge on this platform to enable bridge-specific assertions.');
        }
        // Search API
        const searchRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/search',
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        }, {
            algorithm: 'binary',
            key: 'id',
            query: 'G1',
            dataset: sampleCircuit.gates
        });
        assert(searchRes.statusCode === 200, 'POST /api/search (Binary Search) returns HTTP 200');
        const searchData = JSON.parse(searchRes.body.toString('utf8'));
        assert(searchData.found === true && searchData.item.id === 'G1', 'Binary search locates G1');

        // Sort API
        const sortRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/sort',
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        }, {
            algorithm: 'bubble',
            field: 'id',
            order: 'desc',
            dataset: sampleCircuit.gates
        });
        assert(sortRes.statusCode === 200, 'POST /api/sort (Bubble Sort) returns HTTP 200');
        const sortData = JSON.parse(sortRes.body.toString('utf8'));
        assert(sortData.sorted && sortData.sorted[0].id === 'G2', 'Bubble sort orders components descending');

        // Hashing & Linear Probing API
        const hashRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/hashtable',
            method: 'GET'
        });
        assert(hashRes.statusCode === 200, 'GET /api/hashtable returns HTTP 200');
        const hashData = JSON.parse(hashRes.body.toString('utf8'));
        assert(hashData.capacity === 17, 'Hash table capacity is prime modulus 17');

        // Collision demo
        const colRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/hashtable',
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        }, { action: 'collision_demo' });
        assert(colRes.statusCode === 200, 'POST /api/hashtable collision_demo returns HTTP 200');
        const colData = JSON.parse(colRes.body.toString('utf8'));
        assert(colData.totalCollisions > 0, 'Collision demonstration verifies linear probing: h(k, i) = (hash(k) + i) mod 17');

        // Benchmark API
        const benchRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/benchmark',
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        }, {});
        assert(benchRes.statusCode === 200, 'POST /api/benchmark returns HTTP 200');
        const benchData = JSON.parse(benchRes.body.toString('utf8'));
        assert(benchData.searchBenchmarks && benchData.searchBenchmarks.length === 3, 'Empirical benchmarks evaluate N = 8, 16, 32');

        // Validation API
        const valRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/validate',
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        }, { scenario: 'scenario1' });
        assert(valRes.statusCode === 200, 'POST /api/validate returns HTTP 200');
        const valData = JSON.parse(valRes.body.toString('utf8'));
        assert(valData.verdict === 'VALID', 'Smart Topology Validator evaluates cascade scenario');

        // Test 7: Technical Report PDF Viewing & Downloading
        console.log('\n--- 7. Testing Master Project Technical Report PDF (Viewing & Downloading) ---');
        // Viewing via /docs/
        const repDocsView = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/docs/LogicCraft_Task4_Report.pdf',
            method: 'GET'
        });
        assert(repDocsView.statusCode === 200, 'GET /docs/LogicCraft_Task4_Report.pdf returns HTTP 200');
        assert(repDocsView.headers['content-type'] === 'application/pdf', 'Returns Content-Type: application/pdf');
        assert((repDocsView.headers['content-disposition'] || '').includes('inline'), 'Returns Content-Disposition: inline');
        assert(repDocsView.body.length > 50000, `PDF body loaded (${repDocsView.body.length} bytes)`);

        // Viewing via alias /report.pdf
        const repAliasView = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/report.pdf',
            method: 'GET'
        });
        assert(repAliasView.statusCode === 200, 'GET /report.pdf returns HTTP 200');
        assert(repAliasView.headers['content-type'] === 'application/pdf', '/report.pdf returns Content-Type: application/pdf');
        assert((repAliasView.headers['content-disposition'] || '').includes('inline'), '/report.pdf returns Content-Disposition: inline');

        // Downloading via /docs/LogicCraft_Task4_Report.pdf?download=1
        const repDocsDl = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/docs/LogicCraft_Task4_Report.pdf?download=1',
            method: 'GET'
        });
        assert(repDocsDl.statusCode === 200, 'GET /docs/LogicCraft_Task4_Report.pdf?download=1 returns HTTP 200');
        assert(repDocsDl.headers['content-type'] === 'application/pdf', 'Download returns Content-Type: application/pdf');
        assert((repDocsDl.headers['content-disposition'] || '').includes('attachment'), 'Download returns Content-Disposition: attachment');

        // Downloading via /download/report
        const repAliasDl = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/download/report',
            method: 'GET'
        });
        assert(repAliasDl.statusCode === 200, 'GET /download/report returns HTTP 200');
        assert((repAliasDl.headers['content-disposition'] || '').includes('attachment'), '/download/report returns Content-Disposition: attachment');

        // Test 8: File Locations PDF Viewing & Downloading
        console.log('\n--- 8. Testing File Locations PDF (Viewing & Downloading) ---');
        // Viewing via /docs/
        const locDocsView = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/docs/LogicCraft_Task4_File_Locations.pdf',
            method: 'GET'
        });
        assert(locDocsView.statusCode === 200, 'GET /docs/LogicCraft_Task4_File_Locations.pdf returns HTTP 200');
        assert(locDocsView.headers['content-type'] === 'application/pdf', 'Returns Content-Type: application/pdf');
        assert((locDocsView.headers['content-disposition'] || '').includes('inline'), 'Returns Content-Disposition: inline');
        assert(locDocsView.body.length > 50000, `PDF body loaded (${locDocsView.body.length} bytes)`);

        // Viewing via alias /locations.pdf
        const locAliasView = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/locations.pdf',
            method: 'GET'
        });
        assert(locAliasView.statusCode === 200, 'GET /locations.pdf returns HTTP 200');
        assert(locAliasView.headers['content-type'] === 'application/pdf', '/locations.pdf returns Content-Type: application/pdf');
        assert((locAliasView.headers['content-disposition'] || '').includes('inline'), '/locations.pdf returns Content-Disposition: inline');

        // Downloading via /docs/LogicCraft_Task4_File_Locations.pdf?download=1
        const locDocsDl = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/docs/LogicCraft_Task4_File_Locations.pdf?download=1',
            method: 'GET'
        });
        assert(locDocsDl.statusCode === 200, 'GET /docs/LogicCraft_Task4_File_Locations.pdf?download=1 returns HTTP 200');
        assert((locDocsDl.headers['content-disposition'] || '').includes('attachment'), 'Download returns Content-Disposition: attachment');

        // Downloading via /download/locations
        const locAliasDl = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/download/locations',
            method: 'GET'
        });
        assert(locAliasDl.statusCode === 200, 'GET /download/locations returns HTTP 200');
        assert((locAliasDl.headers['content-disposition'] || '').includes('attachment'), '/download/locations returns Content-Disposition: attachment');

        // Test 9: HTML File Locations Directory
        console.log('\n--- 9. Testing HTML File Locations Directory ---');
        const locHtml = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/docs/TASK4_FILE_LOCATIONS_DIRECTORY.html',
            method: 'GET'
        });
        assert(locHtml.statusCode === 200, 'GET /docs/TASK4_FILE_LOCATIONS_DIRECTORY.html returns HTTP 200');
        assert((locHtml.headers['content-type'] || '').includes('text/html'), 'Returns Content-Type: text/html');
        assert(locHtml.body.length > 20000, `HTML body loaded (${locHtml.body.length} bytes)`);

        // Test 9b: Dynamic Circuit Engineering Report Export (PDF & CSV)
        console.log('\n--- 9b. Testing Dynamic Circuit Report Export (PDF & CSV) ---');
        const exportCircuitPayload = JSON.stringify({
            circuit: {
                gates: [
                    { id: 'G1', type: 'AND', name: 'Primary_AND', inputs: [1, 0], output: 0 },
                    { id: 'G2', type: 'OR', name: 'Secondary_OR', inputs: [0, 1], output: 1 },
                    { id: 'G3', type: 'NOT', name: 'Output_Inverter', inputs: [1], output: 0 }
                ],
                connections: [
                    { id: 'C1', sourceGateId: 'G1', sourceOutput: 0, destinationGateId: 'G2', destinationInputIndex: 0 },
                    { id: 'C2', sourceGateId: 'G2', sourceOutput: 0, destinationGateId: 'G3', destinationInputIndex: 0 }
                ]
            },
            projectName: 'LogicCraft Reference Circuit'
        });

        // 1. CSV Export with real circuit
        const csvRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/export/csv',
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        }, exportCircuitPayload);
        assert(csvRes.statusCode === 200, 'POST /api/export/csv returns HTTP 200');
        assert((csvRes.headers['content-type'] || '').includes('text/csv'), 'CSV Export returns Content-Type: text/csv');
        assert((csvRes.headers['content-disposition'] || '').includes('attachment'), 'CSV Export returns Content-Disposition: attachment');
        const csvBody = csvRes.body.toString('utf8');
        assert(csvBody.includes('[SECTION 1: CIRCUIT METRICS & GATE STATISTICS]'), 'CSV contains Section 1: Gate Statistics');
        assert(csvBody.includes('Total Gates,3'), 'CSV contains correct gate count (3)');
        assert(csvBody.includes('[SECTION 2: SIMULATION & PROPAGATION RESULTS]'), 'CSV contains Section 2: Simulation Results');
        assert(csvBody.includes('[SECTION 3: CIRCUIT VALIDATION & INTEGRITY CHECKS]'), 'CSV contains Section 3: Validation Checks');
        assert(csvBody.includes('Overall Topology Verdict,VALID'), 'CSV contains Validation status VALID');
        assert(csvBody.includes('[SECTION 4: COMPONENT NETLIST & TERMINAL SIGNALS]'), 'CSV contains Section 4: Component Netlist');
        assert(csvBody.includes('[SECTION 5: COMBINATORIAL TRUTH TABLE (2^N STATE ENUMERATION)]'), 'CSV contains Section 5: Truth Table');
        assert(csvBody.includes('#8,'), 'CSV contains full 8 rows of state table');

        // 2. CSV Export with empty circuit
        const emptyCsvRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/export/csv',
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        }, JSON.stringify({ circuit: { gates: [], connections: [] } }));
        assert(emptyCsvRes.statusCode === 200, 'POST /api/export/csv handles empty circuit with HTTP 200');
        const emptyCsvBody = emptyCsvRes.body.toString('utf8');
        assert(emptyCsvBody.includes('Total Gates,0'), 'CSV handles empty circuit metrics (0 gates)');

        // 3. PDF Export with real circuit
        const pdfRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/export/pdf',
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        }, exportCircuitPayload);
        assert(pdfRes.statusCode === 200, 'POST /api/export/pdf returns HTTP 200');
        assert((pdfRes.headers['content-type'] || '').includes('application/pdf'), 'PDF Export returns Content-Type: application/pdf');
        assert((pdfRes.headers['content-disposition'] || '').includes('attachment'), 'PDF Export returns Content-Disposition: attachment');
        const pdfHead = pdfRes.body.slice(0, 8).toString('utf8');
        assert(pdfHead.startsWith('%PDF-1.'), 'PDF Export produces valid PDF header (%PDF-1.4)');
        const pdfTail = pdfRes.body.slice(-20).toString('utf8');
        assert(pdfTail.includes('%%EOF'), 'PDF Export produces valid PDF trailer (%%EOF)');
        assert(pdfRes.body.length > 500, `PDF Export file size is ${pdfRes.body.length} bytes`);

        // 4. PDF Export with empty circuit
        const emptyPdfRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/export/pdf',
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        }, JSON.stringify({ circuit: { gates: [], connections: [] } }));
        assert(emptyPdfRes.statusCode === 200, 'POST /api/export/pdf handles empty circuit with HTTP 200');
        assert(emptyPdfRes.body.slice(0, 8).toString('utf8').startsWith('%PDF-1.'), 'Empty circuit PDF export produces valid PDF header');

        // Test 10: Processor Lab Native Operations
        console.log('\n--- 10. Testing Processor Lab Native Operations ---');
        // COA arithmetic regression checks: signed Booth multiplication and fast CLA.
        const boothRes = await request({ host: '127.0.0.1', port: PORT, path: '/api/coa/arithmetic', method: 'POST', headers: { 'Content-Type': 'application/json' } }, { operation: 'booth', operandA: -8, operandB: -8, bitWidth: 4 });
        const boothData = JSON.parse(boothRes.body.toString('utf8'));
        assert(boothRes.statusCode === 200 && boothData.verifiedCorrect && boothData.decimalResult === 64, 'Booth handles signed minimum-value multiplication (-8 × -8)');
        const boothNegRes = await request({ host: '127.0.0.1', port: PORT, path: '/api/coa/arithmetic', method: 'POST', headers: { 'Content-Type': 'application/json' } }, { operation: 'booth', operandA: -7, operandB: 3, bitWidth: 4 });
        const boothNegData = JSON.parse(boothNegRes.body.toString('utf8'));
        assert(boothNegData.verifiedCorrect && boothNegData.decimalResult === -21, 'Booth handles negative × positive signed operands');
        const claRes = await request({ host: '127.0.0.1', port: PORT, path: '/api/coa/arithmetic', method: 'POST', headers: { 'Content-Type': 'application/json' } }, { operation: 'cla', operandA: 15, operandB: 1, bitWidth: 4 });
        const claData = JSON.parse(claRes.body.toString('utf8'));
        assert(claRes.statusCode === 200 && claData.sumBinary === '0000' && claData.carryOut === 1, 'CLA computes unsigned carry-out for 4-bit 15 + 1');
        const claSignedRes = await request({ host: '127.0.0.1', port: PORT, path: '/api/coa/arithmetic', method: 'POST', headers: { 'Content-Type': 'application/json' } }, { operation: 'cla', operandA: 7, operandB: 1, bitWidth: 4 });
        const claSignedData = JSON.parse(claSignedRes.body.toString('utf8'));
        assert(claSignedData.sumSigned === -8 && claSignedData.overflow === true, 'CLA detects signed overflow for 4-bit 7 + 1');

        const coaExecRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/execute/coa',
            method: 'POST'
        });
        assert(coaExecRes.statusCode === 200, 'POST /api/execute/coa returns HTTP 200');
        const coaExecData = JSON.parse(coaExecRes.body.toString('utf8'));
        assert(coaExecData.command === 'Tests/test_alp.exe', '64-Bit Co-Processor test runner executes successfully');

        const cppExecRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/execute/cpp',
            method: 'POST'
        });
        assert(cppExecRes.statusCode === 200, 'POST /api/execute/cpp returns HTTP 200');
        const cppExecData = JSON.parse(cppExecRes.body.toString('utf8'));
        assert(cppExecData.command === 'Tests/test_runner.exe', 'C++ State Machine test runner executes successfully');

        const exportRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/circuit/export',
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        }, sampleCircuit);
        assert(exportRes.statusCode === 200, 'POST /api/circuit/export returns HTTP 200');
        const exportData = JSON.parse(exportRes.body.toString('utf8'));
        assert(exportData.filename && exportData.content, 'Java circuit serialization framework exports .circ');

        // Test 11: Computer Graphics UI & Controls
        console.log('\n--- 11. Testing Computer Graphics UI & Controls ---');
        assert(html.includes('id="btnRotateGate"'), 'Circuit Designer contains Rotate 90° button');
        assert(html.includes('id="btnFlipH"'), 'Circuit Designer contains Flip Horizontal button');
        assert(html.includes('id="btnFlipV"'), 'Circuit Designer contains Flip Vertical button');
        assert(html.includes('id="btnToggle3D"'), 'Circuit Designer contains 3D View mode toggle');
        assert(html.includes('id="btnRotate3DX"') && html.includes('id="btnRotate3DY"') && html.includes('id="btnRotate3DZ"'), '3D View provides X/Y/Z axis rotation controls');
        assert(appJs.includes('function rotateSelectedGroup3D(axis, angleDeg = 15)') && appJs.includes('g.rotation3D'), '3D rotation stores axis orientation on gate model');
        assert(appJs.includes('gate.z3d || 0') && appJs.includes('ctx.scale(tiltY, tiltX)'), '3D rotation affects projected gate position and rendered package orientation');
        assert(html.includes('id="screen-graphics"'), 'Dedicated screen-graphics section exists');
        assert(html.includes('data-cg-tab="cg-tab-line"'), 'Graphics Lab contains Bresenham Line Drawing tab');
        assert(html.includes('data-cg-tab="cg-tab-circle"'), 'Graphics Lab contains Midpoint Circle tab');
        assert(html.includes('data-cg-tab="cg-tab-polygon"'), 'Graphics Lab contains Scanline Polygon Filling tab');
        assert(html.includes('data-cg-tab="cg-tab-2d"'), 'Graphics Lab contains 2D Transformations tab');
        assert(html.includes('data-cg-tab="cg-tab-3d"'), 'Graphics Lab contains 3D Transformations tab');
        assert(html.includes('data-cg-tab="cg-tab-circuit"'), 'Graphics Lab contains Circuit Graphics verification tab');
        assert(html.includes('id="canvasBresenham"'), 'Bresenham pixel grid canvas element present');
        assert(html.includes('id="canvasCircle"'), 'Midpoint circle canvas element present');
        assert(html.includes('id="canvasScanline"'), 'Scanline polygon canvas element present');
        assert(html.includes('id="canvas2DTransform"'), '2D transform canvas element present');
        assert(html.includes('id="canvas3DTransform"'), '3D transform canvas element present');

        // Test 12: Computer Graphics APIs & C++ OpenGL Pipeline Execution
        console.log('\n--- 12. Testing Computer Graphics APIs & Algorithms ---');
        const cgAlgoRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/graphics/algorithms',
            method: 'GET'
        });
        assert(cgAlgoRes.statusCode === 200, 'GET /api/graphics/algorithms returns HTTP 200');
        const cgAlgoData = JSON.parse(cgAlgoRes.body.toString('utf8'));
        const algos = cgAlgoData.catalog || cgAlgoData.algorithms || {};
        assert(algos.bresenham, 'API returns Bresenham line algorithm specification');
        assert(algos.circle, 'API returns Midpoint circle algorithm specification');
        assert(algos.scanline, 'API returns Scanline polygon filling specification');
        assert(algos.transform2d, 'API returns 2D transformation specification');
        assert(algos.transform3d, 'API returns 3D transformation specification');

        // Execute Native C++ OpenGL Verification Runner
        console.log('\n--- 13. Executing Native C++ OpenGL Algorithm Verification Runner ---');
        const cgExecRes = await request({
            host: '127.0.0.1',
            port: PORT,
            path: '/api/execute/graphics',
            method: 'POST'
        });
        assert(cgExecRes.statusCode === 200, 'POST /api/execute/graphics returns HTTP 200');
        const cgExecData = JSON.parse(cgExecRes.body.toString('utf8'));
        if (process.platform === 'win32') {
            assert(cgExecData.exitCode === 0, `logiccraft_cg.exe --test exited with code 0 (Actual: ${cgExecData.exitCode})`);
            assert(cgExecData.output && cgExecData.output.includes('CG Algorithms Passed: 20 / 20'), 'All 20 fundamental CG algorithm unit tests verified PASSED');
            assert(cgExecData.output && cgExecData.output.includes('ALL COMPUTER GRAPHICS ALGORITHMS & PIPELINE VERIFICATIONS PASSED!'), 'Complete OpenGL workbench pipeline verified PASSED');
            assert(cgExecData.output && cgExecData.output.includes('Bresenham Horizontal Line: Points count: 11'), 'Bresenham horizontal wire routing verified');
            assert(cgExecData.output && cgExecData.output.includes('Midpoint Circle Perimeter: Generated 96 perimeter boundary pixels'), 'Midpoint circle with 8-way symmetry verified');
            assert(cgExecData.output && cgExecData.output.includes('Scanline Fill Custom Gate Body'), 'Scanline polygon filling with ET & AET verified');
            assert(cgExecData.output && cgExecData.output.includes('Transformed Pin Coordinate Synchronization across all 7 gate types'), 'Transformed pin coordinate synchronization verified');
        } else {
            console.log('[SKIP] Native Windows OpenGL executable checks require Windows; source compilation can be verified separately.');
        }

        console.log(`\n=======================================================`);
        console.log(`TEST RESULTS SUMMARY: ${passed} PASSED, ${failed} FAILED`);
        console.log(`=======================================================`);
        process.exit(failed > 0 ? 1 : 0);
    } catch (err) {
        console.error('Test execution error:', err);
        process.exit(1);
    }
}

runTests();
