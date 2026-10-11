# HOTELSTAY MINI PROJECT — SEMINAR PRESENTATION SLIDES
### Topic: HotelStay — Domain-Specific Compiler Language Using Lex & Yacc
*Course:* Compiler Design Laboratory Mini Project  
*Target File Extension:* `.stay`  

---

## SLIDE 1: Title Slide
- **Title:** HotelStay: A Domain-Specific Language for Luxury Hotel Reservation & Billing Automation
- **Subtitle:** Built using LEX (Flex) and YACC (Bison)
- **Team Members:** [Student Name 1], [Student Name 2], [Student Name 3]
- **Batch / Semester:** [Your Batch / Semester]
- **Guide / Evaluator:** [Laboratory Faculty Name]
- **Department:** Computer Science & Engineering

*Speaker Note:*
"Good morning respected faculty and colleagues. Today we present our Compiler Design Mini Project titled HotelStay, a specialized Domain-Specific Language developed using Lex and Yacc for automated hotel booking and billing management."

---

## SLIDE 2: Problem Statement & Motivation
- **Why Domain-Specific Languages (DSLs)?**
  - General-purpose languages (C, C++, Java) have verbose syntax for domain-specific tasks.
  - A DSL provides concise, readable syntax tailored directly to industry workflows.
- **The Hotel Management Use-Case:**
  - Front-desk clerks and reservation engines need simple, declarative rules for room booking, amenities addition, discounts, and invoices.
- **Project Goal:**
  - Build an end-to-end compiler with custom grammar, symbol table management, semantic constraints, and invoice generation.

---

## SLIDE 3: Architecture & Compilation Pipeline
- **Pipeline Stages:**
  1. **Source Code (`*.stay`)**: Input script containing hotel domain instructions.
  2. **Lexical Analysis (`HotelStay.l` - Flex)**: Scans stream of characters, removes comments/whitespaces, and generates tokens (`GUEST`, `ROOM`, `NUMBER`, `ID`).
  3. **Syntax Analysis (`HotelStay.y` - Bison)**: LALR(1) parser verifies Context-Free Grammar (CFG) rules and expression precedence.
  4. **Symbol Table & Semantic Engine**: Maintains active guest folio in memory; checks for double-booking, missing guests, and valid constraints.
  5. **Code Execution & Target Output**: Prints occupancy logs and formatted itemized tax invoices.

---

## SLIDE 4: Custom Keywords & Tokens (10 Keywords)
*(Faculty requirement: Minimum 7 keywords for a group of 3)*

1. `GUEST` — Declares and registers a guest in the symbol table.
2. `ROOM` — Assigns room category (`STANDARD`, `DELUXE`, `SUITE`, `PRESIDENTIAL`).
3. `NIGHTS` — Sets duration of stay (supports arithmetic expressions like `(2 + 1)`).
4. `AMENITY` — Adds folio services (Spa, Dining, Airport Pickup) with cost.
5. `DISCOUNT` — Applies promotional percentage discount (0%–100%).
6. `TAX` — Configures customized GST/VAT rate (default: 12%).
7. `CHECKOUT` — Computes net balance and prints an itemized tax invoice.
8. `STATUS` — Displays live occupancy register of the hotel.
9. `PRINT` — Outputs announcements or arithmetic calculation results.
10. `RESET` — Clears guest records from the active registry.

---

## SLIDE 5: Grammar Rules & Operator Precedence
- **Expression Evaluation:**
  - Supports `+`, `-`, `*`, `/`, and parentheses `(` `)`.
  - Associativity & Precedence defined in Bison:
    ```yacc
    %left '+' '-'
    %left '*' '/'
    ```
- **Declarative Grammar Rules:**
  - `GUEST ID ';'`
  - `ROOM ID <Category> ';'`
  - `NIGHTS ID expression ';'`
  - `AMENITY ID ID expression ';'`
  - `CHECKOUT ID ';'`

---

## SLIDE 6: Symbol Table Design & Data Structures
- **Symbol Table Structure in C:**
  ```c
  typedef struct {
      char name[50];
      int cost;
  } AmenityItem;

  typedef struct {
      char name[50];
      char roomType[30];
      int roomRate;
      int nights;
      AmenityItem amenities[20];
      int amenityCount;
      int discountPercent;
      int taxPercent;
      int isCheckedOut;
  } GuestRecord;
  ```
- **State Lookup:**
  - `findGuest(name)` allows $O(n)$ lookup for quick validation before any property modification.

---

## SLIDE 7: Semantic Validation & Error Trapping
- **Our compiler handles critical error conditions:**
  - **Duplicate Registration Error:** Prevents registering an existing active guest.
  - **Undeclared Guest Error:** Operations on non-existent guests are rejected.
  - **Zero or Negative Nights:** Stay duration must be $\ge 1$.
  - **Illegal Discount Range:** Discounts exceeding 100% or below 0% are flagged.
  - **Division by Zero:** Safely caught during expression evaluation.
  - **Double Checkout Warning:** Prevents re-billing a checked-out guest.

---

## SLIDE 8: Demonstration & Sample Code (`booking.stay`)
```
GUEST Aarav;
ROOM Aarav SUITE;
NIGHTS Aarav 3;
AMENITY Aarav SPA 2500;
AMENITY Aarav DINING 3200;
DISCOUNT Aarav 10;
TAX Aarav 18;
CHECKOUT Aarav;
```
- **Live Output Highlights:**
  - Base room charge: $3 \times 8000 = \text{Rs. } 24,000$
  - Folio amenities: $\text{Rs. } 2,500 + \text{Rs. } 3,200 = \text{Rs. } 5,700$
  - Gross subtotal: $\text{Rs. } 29,700$
  - Less 10% Discount: $-\text{Rs. } 2,970$
  - Plus 18% GST: $+\text{Rs. } 4,811$
  - **Grand Total Payable:** $\text{Rs. } 31,541$

---

## SLIDE 9: Project Files & Toolchain
- `HotelStay.l`: Lexical analyzer file (Flex).
- `HotelStay.y`: Parser and semantic routines (Bison).
- `booking.stay`: Standard execution script.
- `error_demo.stay`: Robustness & error testing script.
- `build.bat`: 1-click build and execution script for Windows.
- Compatible with: MSYS2 / MinGW / GCC / JDoodle Online Flex & Bison Compiler.

---

## SLIDE 10: Conclusion & Learning Outcomes
- Successfully constructed a full working Domain-Specific Language compiler.
- Gained hands-on experience in:
  - Lex regular expressions and token generation.
  - Yacc LALR(1) parsing and shift/reduce conflict resolution.
  - Symbol table design and semantic verification in C.
  - Real-world domain automation.

*Speaker Note:*
"Thank you. We are now open for faculty questions and the live demonstration."
