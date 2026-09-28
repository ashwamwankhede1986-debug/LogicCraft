# ==============================================================================
# LOGICCRAFT — 64-BIT ASSEMBLY LANGUAGE PROGRAMMING (COA / ALP CO2)
# File: logic_ops_64.s
# Architecture: x86-64 (AMD64 / Intel 64)
# ABI: Microsoft x64 Calling Convention
# Assembler Syntax: GNU AS (.intel_syntax noprefix)
# ==============================================================================
# Demonstrates:
#  1. 64-bit General Purpose Registers: RAX, RBX, RCX, RDX, R8, R9
#  2. 64-bit Logic Instructions: MOV, AND, OR, NOT, XOR
#  3. Comparison & Processor Status Flags: CMP, TEST, ZF (Zero Flag)
#  4. Conditional Branching: JE (Jump if Equal), JNE (Jump if Not Equal)
# ==============================================================================

    .intel_syntax noprefix
    .text

# ------------------------------------------------------------------------------
# FUNCTION 1: asm_and_64
# Signature: uint64_t asm_and_64(uint64_t a, uint64_t b)
# Parameters:
#   RCX = Operand A (64-bit)
#   RDX = Operand B (64-bit)
# Return:
#   RAX = Result (A AND B)
# ------------------------------------------------------------------------------
    .global asm_and_64
asm_and_64:
    mov rax, rcx          # Load 64-bit operand A into RAX
    and rax, rdx          # Execute 64-bit bitwise AND with operand B
    ret                   # Return evaluated result in RAX

# ------------------------------------------------------------------------------
# FUNCTION 2: asm_or_64
# Signature: uint64_t asm_or_64(uint64_t a, uint64_t b)
# Parameters:
#   RCX = Operand A (64-bit)
#   RDX = Operand B (64-bit)
# Return:
#   RAX = Result (A OR B)
# ------------------------------------------------------------------------------
    .global asm_or_64
asm_or_64:
    mov rax, rcx          # Load 64-bit operand A into RAX
    or  rax, rdx          # Execute 64-bit bitwise OR with operand B
    ret                   # Return evaluated result in RAX

# ------------------------------------------------------------------------------
# FUNCTION 3: asm_not_64
# Signature: uint64_t asm_not_64(uint64_t a)
# Parameters:
#   RCX = Operand A (64-bit)
# Return:
#   RAX = Result (NOT A, masked to bit 0)
# ------------------------------------------------------------------------------
    .global asm_not_64
asm_not_64:
    mov rax, rcx          # Load operand into RAX
    xor rax, 1            # Invert lowest bit (0 -> 1, 1 -> 0)
    ret

# ------------------------------------------------------------------------------
# FUNCTION 4: asm_xor_64
# Signature: uint64_t asm_xor_64(uint64_t a, uint64_t b)
# Parameters:
#   RCX = Operand A (64-bit)
#   RDX = Operand B (64-bit)
# Return:
#   RAX = Result (A XOR B)
# ------------------------------------------------------------------------------
    .global asm_xor_64
asm_xor_64:
    mov rax, rcx          # Load 64-bit operand A into RAX
    xor rax, rdx          # Execute 64-bit bitwise XOR with operand B
    ret

# ------------------------------------------------------------------------------
# FUNCTION 5: asm_branch_verify_64
# Signature: uint64_t asm_branch_verify_64(uint64_t a, uint64_t b, uint64_t opType, uint64_t expected)
# Parameters:
#   RCX = Operand A
#   RDX = Operand B
#   R8  = OpType (0 = AND, 1 = OR, 2 = XOR)
#   R9  = Expected Output Value
# Return:
#   RAX = 100 if Branch Verified (Match), 404 if Mismatch
# Demonstrates: CMP, JE, JNE, Conditional Control Flow (CO2)
# ------------------------------------------------------------------------------
    .global asm_branch_verify_64
asm_branch_verify_64:
    # Check Operation Type
    cmp r8, 0             # Compare opType with 0 (AND)
    je  .op_and_branch

    cmp r8, 1             # Compare opType with 1 (OR)
    je  .op_or_branch

    # Default to XOR
    mov rax, rcx
    xor rax, rdx
    jmp .verify_result

.op_and_branch:
    mov rax, rcx
    and rax, rdx
    jmp .verify_result

.op_or_branch:
    mov rax, rcx
    or  rax, rdx

.verify_result:
    # Compare computed RAX with expected R9
    cmp rax, r9           # Compare Result with Expected
    jne .branch_mismatch  # Conditional branch if not equal (JNE)

.branch_match:
    mov rax, 100          # Code 100: Branch match verified
    ret

.branch_mismatch:
    mov rax, 404          # Code 404: Branch mismatch detected
    ret
