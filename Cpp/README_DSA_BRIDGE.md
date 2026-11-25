# LogicCraft Native C++ DSA Bridge

This CLI bridge lets external tools (including the LogicCraft backend) call the project's procedural C++ searching and sorting implementations.


## Build (Windows)
Run `build_dsa_bridge.bat` in this directory. It requires MinGW-w64 `g++` available on PATH.

## Input
One record per line, pipe-delimited: `id|type|name|inputCount|output`.
Avoid pipe characters in fields. The backend should sanitize them before sending.

## Commands
- `logiccraft_dsa_bridge.exe search id linear <target>`
- `logiccraft_dsa_bridge.exe search id binary <target>`
- `logiccraft_dsa_bridge.exe search name linear <target>`
- `logiccraft_dsa_bridge.exe search name binary <target>`
- `logiccraft_dsa_bridge.exe sort id|name|type bubble|selection|insertion|quick|merge asc|desc`

Search output begins `SEARCH_RESULT|index|comparisons`; a found record is on the following line. Sort output begins `SORT_METRICS|comparisons|swaps` followed by sorted records.
Binary search sorts a copy internally by the selected key before searching. Maximum input size: 5,000 records.
