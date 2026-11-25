# LogicCraft — Changes After Antigravity Stopped

## Implemented / verified in this pass
- Confirmed the New Project path creates an empty circuit (`gates: []`, `connections: []`).
- Confirmed Gate Palette collapse/expand controls and handlers were already present; the canvas is resized after toggling.
- Removed Technical Reports and Test & Verification Evidence navigation buttons from the sidebar. Their screens and report endpoints remain in the project.
- Confirmed Ctrl/Meta/Shift selection supports multi-selection, and the Backspace/Delete handler avoids deleting while focus is in inputs, textareas, contenteditable fields, or selects. Delete removes related connections and creates an undo action.
- Confirmed the group-rotation handler operates on actual selected gate objects and their coordinates. It is a planar rotation about the selection's center; the stylized 3D viewport does not yet provide independent X/Y/Z rotations.
- Changed Component Analysis defaults to Binary Search and Merge Sort, while other available algorithms remain selectable.
- Corrected the COA carry calculation to compute expanded carry-lookahead terms directly rather than ripple-chaining carries.
- Widened the Booth accumulator to N+1 bits so the signed minimum operand can be negated correctly during recoding.
- Added Java Swing login gate for the Java GUI launch path (`run_java.bat --gui` / `logiccraft.Main --gui`). First launch creates a local account; credentials use PBKDF2-HMAC-SHA256 with salt and are stored under the user's home directory, not as plaintext.
- Updated automated tests for removed navigation entries, palette toggles, search/sort defaults, and arithmetic boundary cases.

## Validation performed
- `node --check frontend/app.js`: passed
- `node --check backend/server.js`: passed
- `node --check test_suite.js`: passed
- `javac` compiling all Java source files: passed
- COA C++/assembly compile: passed on the available Linux environment
- C++ coursework module compile: passed on the available Linux environment
- CG algorithm source compilation: passed
- LogicCraft Node test suite: **142 passed, 0 failed**
- Exhaustive signed 4-bit arithmetic API checks: **256 Booth pairs and 256 CLA pairs passed**

## Remaining limitations / work not claimed as complete
- The Java Swing login gate protects the Java Swing GUI launch path only. It is not a web-app login gate and does not secure direct browser access to the Node web application.
- Circuit group rotation is planar (around the canvas center) in the stylized 3D view. Independent 3-axis gate/group orientation and full 3D transform persistence are not implemented yet.
- The frontend/backend search and sorting services are implemented in JavaScript. The C++ DSA implementations exist in the coursework module but are not directly wired into the web API.
- Native Windows OpenGL executable verification could not be run in this Linux environment. The test suite skips Windows binary assertions on non-Windows hosts. The previous Windows executable in the archive cannot be executed here, so its CG runtime test results remain unverified.
- No browser-based visual interaction test (real browser automation) was performed in this pass.

## Launch notes
- Web application: run `run_web.bat` on Windows.
- Java GUI with Swing login gate: run `run_java.bat --gui` on Windows.
