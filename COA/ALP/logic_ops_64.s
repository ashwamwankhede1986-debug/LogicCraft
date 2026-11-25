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
# FUNCTION 4B: asm_nand_64
# Signature: uint64_t asm_nand_64(uint64_t a, uint64_t b)
# ------------------------------------------------------------------------------
    .global asm_nand_64
asm_nand_64:
    mov rax, rcx
    and rax, rdx
    xor rax, 1            # Invert bit 0
    ret

# ------------------------------------------------------------------------------
# FUNCTION 4C: asm_nor_64
# Signature: uint64_t asm_nor_64(uint64_t a, uint64_t b)
# ------------------------------------------------------------------------------
    .global asm_nor_64
asm_nor_64:
    mov rax, rcx
    or  rax, rdx
    xor rax, 1            # Invert bit 0
    ret

# ------------------------------------------------------------------------------
# FUNCTION 4D: asm_xnor_64
# Signature: uint64_t asm_xnor_64(uint64_t a, uint64_t b)
# ------------------------------------------------------------------------------
    .global asm_xnor_64
asm_xnor_64:
    mov rax, rcx
    xor rax, rdx
    xor rax, 1            # Invert bit 0
    ret

# ------------------------------------------------------------------------------
# FUNCTION 4E: asm_add_64
# Signature: uint64_t asm_add_64(uint64_t a, uint64_t b)
# ------------------------------------------------------------------------------
    .global asm_add_64
asm_add_64:
    mov rax, rcx
    add rax, rdx
    ret

# ------------------------------------------------------------------------------
# FUNCTION 4F: asm_sub_64
# Signature: uint64_t asm_sub_64(uint64_t a, uint64_t b)
# ------------------------------------------------------------------------------
    .global asm_sub_64
asm_sub_64:
    mov rax, rcx
    sub rax, rdx
    ret

# ------------------------------------------------------------------------------
# FUNCTION 4G: asm_cmp_64
# Signature: int64_t asm_cmp_64(uint64_t a, uint64_t b)
# ------------------------------------------------------------------------------
    .global asm_cmp_64
asm_cmp_64:
    cmp rcx, rdx
    je  .cmp_eq
    ja  .cmp_gt
    mov rax, -1
    ret
.cmp_eq:
    xor rax, rax
    ret
.cmp_gt:
    mov rax, 1
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

# ==============================================================================
# CO3 REQUIREMENT 1: COMPUTATIONAL ARITHMETIC ALGORITHM
# ==============================================================================

# ------------------------------------------------------------------------------
# FUNCTION 6: asm_mul_successive_addition_64
# Signature: uint64_t asm_mul_successive_addition_64(uint64_t multiplicand, uint64_t multiplier)
# Purpose: Calculates total circuit propagation delay (GateCount * DelayPerGate)
#          or truth-table matrix dimension using Successive Addition Algorithm.
# Parameters:
#   RCX = Multiplicand (e.g. propagation delay in picoseconds/nanoseconds)
#   RDX = Multiplier   (e.g. gate count)
# Return:
#   RAX = Product (accumulated sum)
# Register Usage:
#   RAX = Accumulator for partial sums
#   RCX = Addend (preserved)
#   RDX = Decrementing loop counter
# Flags: ZF tested via DEC and JNZ
# ------------------------------------------------------------------------------
    .global asm_mul_successive_addition_64
asm_mul_successive_addition_64:
    xor rax, rax          # Clear accumulator to 0

    # Boundary check 1: if multiplier is 0, return 0
    test rdx, rdx
    jz   .mul_done

    # Boundary check 2: if multiplicand is 0, return 0
    test rcx, rcx
    jz   .mul_done

.mul_loop:
    add rax, rcx          # Add multiplicand to accumulator
    dec rdx               # Decrement loop counter
    jnz .mul_loop         # Repeat until multiplier reaches 0

.mul_done:
    ret

# ------------------------------------------------------------------------------
# FUNCTION 7: asm_compute_truth_table_rows_64
# Signature: uint64_t asm_compute_truth_table_rows_64(uint64_t numInputs)
# Purpose: Computes 2^N total truth table permutations using repeated doubling.
# Parameters:
#   RCX = Number of Boolean inputs N
# Return:
#   RAX = 2^N rows
# ------------------------------------------------------------------------------
    .global asm_compute_truth_table_rows_64
asm_compute_truth_table_rows_64:
    mov rax, 1            # Base value: 2^0 = 1
    test rcx, rcx         # Check if N == 0
    jz   .pow_done

.pow_loop:
    add rax, rax          # Doubling via successive addition (RAX = RAX + RAX)
    dec rcx               # Decrement exponent
    jnz .pow_loop         # Continue until RCX reaches 0

.pow_done:
    ret

# ==============================================================================
# CO3 REQUIREMENT 2: STRING & DATA PROCESSING IN 64-BIT ASSEMBLY
# ==============================================================================

# ------------------------------------------------------------------------------
# FUNCTION 8: asm_parse_gate_type_64
# Signature: uint64_t asm_parse_gate_type_64(const char* gateName)
# Purpose: Parses an ASCII gate name string into a discrete numeric opcode.
# Parameters:
#   RCX = Pointer to null-terminated ASCII string
# Return:
#   RAX = Opcode: 1=AND, 2=OR, 3=NOT, 4=XOR, 5=NAND, 6=NOR, 7=XNOR, 0=Unknown
# Register Usage:
#   AL, DL = Byte character comparisons
#   RCX = String base pointer
# ------------------------------------------------------------------------------
    .global asm_parse_gate_type_64
asm_parse_gate_type_64:
    xor rax, rax          # Default return = 0 (Unknown)
    test rcx, rcx         # Null pointer check
    jz   .parse_done

    movzx rdx, byte ptr [rcx] # Read 1st byte
    test dl, dl           # Check for empty string
    jz   .parse_done

    # Check for 'A' / 'a' -> AND
    cmp dl, 'A'
    je  .check_and
    cmp dl, 'a'
    je  .check_and

    # Check for 'O' / 'o' -> OR
    cmp dl, 'O'
    je  .check_or
    cmp dl, 'o'
    je  .check_or

    # Check for 'N' / 'n' -> NOT, NAND, NOR
    cmp dl, 'N'
    je  .check_n_prefix
    cmp dl, 'n'
    je  .check_n_prefix

    # Check for 'X' / 'x' -> XOR, XNOR
    cmp dl, 'X'
    je  .check_x_prefix
    cmp dl, 'x'
    je  .check_x_prefix

    jmp .parse_done

.check_and:
    mov al, byte ptr [rcx + 1]
    or  al, 0x20          # Lowercase normalization
    cmp al, 'n'
    jne .parse_done
    mov al, byte ptr [rcx + 2]
    or  al, 0x20
    cmp al, 'd'
    jne .parse_done
    cmp byte ptr [rcx + 3], 0 # Ensure null termination
    jne .parse_done
    mov rax, 1            # Opcode 1: AND
    ret

.check_or:
    mov al, byte ptr [rcx + 1]
    or  al, 0x20
    cmp al, 'r'
    jne .parse_done
    cmp byte ptr [rcx + 2], 0
    jne .parse_done
    mov rax, 2            # Opcode 2: OR
    ret

.check_n_prefix:
    mov al, byte ptr [rcx + 1]
    or  al, 0x20
    cmp al, 'o'
    je  .check_not_or_nor
    cmp al, 'a'
    je  .check_nand
    jmp .parse_done

.check_not_or_nor:
    mov al, byte ptr [rcx + 2]
    or  al, 0x20
    cmp al, 't'
    je  .match_not
    cmp al, 'r'
    je  .match_nor
    jmp .parse_done

.match_not:
    cmp byte ptr [rcx + 3], 0
    jne .parse_done
    mov rax, 3            # Opcode 3: NOT
    ret

.match_nor:
    cmp byte ptr [rcx + 3], 0
    jne .parse_done
    mov rax, 6            # Opcode 6: NOR
    ret

.check_nand:
    mov al, byte ptr [rcx + 2]
    or  al, 0x20
    cmp al, 'n'
    jne .parse_done
    mov al, byte ptr [rcx + 3]
    or  al, 0x20
    cmp al, 'd'
    jne .parse_done
    cmp byte ptr [rcx + 4], 0
    jne .parse_done
    mov rax, 5            # Opcode 5: NAND
    ret

.check_x_prefix:
    mov al, byte ptr [rcx + 1]
    or  al, 0x20
    cmp al, 'o'
    je  .check_xor
    cmp al, 'n'
    je  .check_xnor
    jmp .parse_done

.check_xor:
    mov al, byte ptr [rcx + 2]
    or  al, 0x20
    cmp al, 'r'
    jne .parse_done
    cmp byte ptr [rcx + 3], 0
    jne .parse_done
    mov rax, 4            # Opcode 4: XOR
    ret

.check_xnor:
    mov al, byte ptr [rcx + 2]
    or  al, 0x20
    cmp al, 'o'
    jne .parse_done
    mov al, byte ptr [rcx + 3]
    or  al, 0x20
    cmp al, 'r'
    jne .parse_done
    cmp byte ptr [rcx + 4], 0
    jne .parse_done
    mov rax, 7            # Opcode 7: XNOR
    ret

.parse_done:
    ret

# ------------------------------------------------------------------------------
# FUNCTION 9: asm_validate_command_64
# Signature: uint64_t asm_validate_command_64(const char* cmdString)
# Purpose: Validates interactive circuit control commands in 64-bit Assembly.
# Parameters:
#   RCX = Pointer to command ASCII string
# Return:
#   RAX = Command ID: 1=ADD_GATE, 2=DEL_GATE, 3=CONNECT, 4=SIMULATE, 5=TOGGLE, 0=Invalid
# ------------------------------------------------------------------------------
    .global asm_validate_command_64
asm_validate_command_64:
    xor rax, rax
    test rcx, rcx
    jz   .cmd_done

    mov dl, byte ptr [rcx]
    test dl, dl
    jz   .cmd_done

    cmp dl, 'A'           # ADD_GATE
    je  .cmd_add
    cmp dl, 'D'           # DEL_GATE
    je  .cmd_del
    cmp dl, 'C'           # CONNECT
    je  .cmd_conn
    cmp dl, 'S'           # SIMULATE
    je  .cmd_sim
    cmp dl, 'T'           # TOGGLE
    je  .cmd_tog
    ret

.cmd_add:
    # "ADD_GATE"
    cmp byte ptr [rcx + 1], 'D'
    jne .cmd_done
    cmp byte ptr [rcx + 2], 'D'
    jne .cmd_done
    mov rax, 1
    ret

.cmd_del:
    # "DEL_GATE"
    cmp byte ptr [rcx + 1], 'E'
    jne .cmd_done
    cmp byte ptr [rcx + 2], 'L'
    jne .cmd_done
    mov rax, 2
    ret

.cmd_conn:
    # "CONNECT"
    cmp byte ptr [rcx + 1], 'O'
    jne .cmd_done
    cmp byte ptr [rcx + 2], 'N'
    jne .cmd_done
    mov rax, 3
    ret

.cmd_sim:
    # "SIMULATE"
    cmp byte ptr [rcx + 1], 'I'
    jne .cmd_done
    cmp byte ptr [rcx + 2], 'M'
    jne .cmd_done
    mov rax, 4
    ret

.cmd_tog:
    # "TOGGLE"
    cmp byte ptr [rcx + 1], 'O'
    jne .cmd_done
    cmp byte ptr [rcx + 2], 'G'
    jne .cmd_done
    mov rax, 5
    ret

.cmd_done:
    ret

# ------------------------------------------------------------------------------
# FUNCTION 10: asm_eval_boolean_expr_str_64
# Signature: uint64_t asm_eval_boolean_expr_str_64(const char* exprStr)
# Purpose: Parses and evaluates a Boolean expression ASCII string in Assembly.
#          Supported formats: "1 & 0", "1 | 1", "1 ^ 0", "~0", "~1"
# Parameters:
#   RCX = Pointer to expression ASCII string
# Return:
#   RAX = Evaluated binary output (0 or 1), or 255 if malformed string
# ------------------------------------------------------------------------------
    .global asm_eval_boolean_expr_str_64
asm_eval_boolean_expr_str_64:
    mov rax, 255          # Default: Error code 255 (Malformed)
    test rcx, rcx
    jz   .expr_done

.skip_ws1:
    mov dl, byte ptr [rcx]
    cmp dl, ' '
    jne .read_first
    inc rcx
    jmp .skip_ws1

.read_first:
    test dl, dl
    jz   .expr_done

    # Check for NOT inverter operator '~'
    cmp dl, '~'
    je  .parse_not_expr

    # Otherwise first char must be '0' or '1'
    cmp dl, '0'
    jb  .expr_done
    cmp dl, '1'
    ja  .expr_done

    # Operand A in R8
    movzx r8, dl
    sub   r8, '0'         # Convert ASCII '0'/'1' to numeric 0/1
    inc   rcx

.skip_ws2:
    mov dl, byte ptr [rcx]
    cmp dl, ' '
    jne .read_op
    inc rcx
    jmp .skip_ws2

.read_op:
    test dl, dl
    jz   .expr_done
    mov  r10b, dl         # Save operator in R10B ('&', '|', '^')
    inc  rcx

.skip_ws3:
    mov dl, byte ptr [rcx]
    cmp dl, ' '
    jne .read_second
    inc rcx
    jmp .skip_ws3

.read_second:
    cmp dl, '0'
    jb  .expr_done
    cmp dl, '1'
    ja  .expr_done

    # Operand B in R9
    movzx r9, dl
    sub   r9, '0'         # Convert ASCII to numeric 0/1

    # Execute operator
    cmp r10b, '&'
    je  .do_and
    cmp r10b, '|'
    je  .do_or
    cmp r10b, '^'
    je  .do_xor
    jmp .expr_done

.do_and:
    mov rax, r8
    and rax, r9
    ret

.do_or:
    mov rax, r8
    or  rax, r9
    ret

.do_xor:
    mov rax, r8
    xor rax, r9
    ret

.parse_not_expr:
    inc rcx
.skip_not_ws:
    mov dl, byte ptr [rcx]
    cmp dl, ' '
    jne .read_not_val
    inc rcx
    jmp .skip_not_ws

.read_not_val:
    cmp dl, '0'
    je  .not_zero
    cmp dl, '1'
    je  .not_one
    jmp .expr_done

.not_zero:
    mov rax, 1            # NOT 0 = 1
    ret

.not_one:
    mov rax, 0            # NOT 1 = 0
    ret

.expr_done:
    ret

