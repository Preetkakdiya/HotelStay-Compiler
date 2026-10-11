# HOTELSTAY MINI PROJECT — SEMINAR PRESENTATION SLIDES
### Topic: HotelStay — Domain-Specific Compiler Language Using Lex & Yacc
*Course:* Compiler Design Laboratory Mini Project  
*Target File Extension:* `.stay`  
*GitHub Repository:* [https://github.com/preetkakdiya/HotelStay-Compiler](https://github.com/preetkakdiya/HotelStay-Compiler)

---

## SLIDE 1: Title Slide
- **Title:** HotelStay: A Domain-Specific Language for Luxury Hotel Reservation & Billing Automation
- **Subtitle:** Built using LEX (Flex 2.6.4), YACC (Bison 3.8.2), and C (GCC 13.2.0)
- **Team Members:** [Student Name 1], [Student Name 2], [Student Name 3]
- **Batch / Semester:** [Your Batch / Semester]
- **Guide / Evaluator:** [Laboratory Faculty Name]
- **Department:** Computer Science & Engineering

*Speaker Note:*
"Good morning respected faculty. Today we present our Compiler Design Mini Project titled HotelStay, a specialized compiler language developed using Lex and Yacc that provides both automated luxury hotel billing operations and algorithmic control-flow constructs."

---

## SLIDE 2: Problem Statement & Motivation
- **Why a Domain-Specific Language (DSL)?**
  - General programming languages are verbose and require extensive boilerplate for business domains.
  - HotelStay provides clean, declarative statements tailored to hotel management.
- **Key Objectives:**
  - Build an end-to-end compiler featuring lexical tokenization, Context-Free Grammar parsing, and AST generation.
  - Implement dual symbol tables (guest records and variable tracking).
  - Provide automated live room inventory management and itemized tax invoice generation.
  - Implement full arithmetic evaluation, conditionals (`WHEN/OTHERWISE`), and loops (`REPEAT`).

---

## SLIDE 3: Compiler Architecture & Pipeline
- **Compilation Stages:**
  1. **Source Code (`*.stay`)**: Input script containing domain statements and control flow.
  2. **Lexical Analysis (`HotelStay.l` - Flex)**: Scans character stream, filters whitespaces/comments, and generates tokens.
  3. **Syntax Analysis (`HotelStay.y` - Bison)**: LALR(1) parser verifies CFG grammar with **0 shift/reduce conflicts** and constructs the Abstract Syntax Tree (AST).
  4. **Symbol Table & Semantic Engine**:
     - Case-insensitive guest lookup.
     - Dual symbol tables for guests and variables.
     - Live category inventory caps (Standard: 20, Deluxe: 15, Suite: 10, Presidential: 5 = 50 Total).
  5. **Execution & Target Output**: Formatted room availability logs and itemized tax invoices.

---

## SLIDE 4: Custom Keywords (16 Keywords)
*(Exceeds requirement: Minimum 7 keywords for a group of 3)*

- **Hotel Domain Keywords:**
  - `GUEST` — Registers guest into the symbol table.
  - `ROOM` — Allocates room tier (`STANDARD`, `DELUXE`, `SUITE`, `PRESIDENTIAL`).
  - `NIGHTS` — Sets duration of stay (supports arithmetic expressions like `(2 + 1)`).
  - `AMENITY` — Adds folio charges (Dining, Airport Pickup, Buffet, Laundry, Gym).
  - `DISCOUNT` — Applies promotional percentage discount (0%–100%).
  - `TAX` — Configures customized GST/VAT rate (default: 12%).
  - `CHECKOUT` — Computes net balance, releases room, and prints tax invoice.
  - `STATUS` — Displays live room inventory dashboard and occupancy register.
  - `PRINT` — Outputs announcements or arithmetic calculation results.
  - `RESET` — Removes guest records from active memory.

- **Control Flow & Variable Keywords:**
  - `MAKE` — Declares integer variable.
  - `TAKE` — Accepts interactive user input (`scanf`).
  - `WHEN` / `OTHERWISE` — Conditional branching (`if-else`).
  - `REPEAT` — Iterative loop construct (`while` loop).
  - `STOP` — Halts execution.

---

## SLIDE 5: Grammar Rules & Operator Precedence
- **Arithmetic & Relational Precedence:**
  ```yacc
  %nonassoc LOWER_THAN_OTHERWISE
  %nonassoc OTHERWISE
  %left EQ NEQ
  %left '<' '>' LTE GTE
  %left '+' '-'
  %left '*' '/'
  ```
- **Declarative Grammar Constructs:**
  - `GUEST ID ';'`
  - `ROOM ID <Category> ';'`
  - `NIGHTS ID expression ';'`
  - `AMENITY ID ID expression ';'`
  - `WHEN '(' expression ')' statement OTHERWISE statement`
  - `REPEAT '(' expression ')' statement`
  - `CHECKOUT ID ';'`

---

## SLIDE 6: Dual Symbol Table Architecture
- **Guest Folio Record (`GuestRecord`):**
  - Name, room tier, room rate, nights duration.
  - Folio amenities array (up to 20 items per guest).
  - Discount %, tax %, and checkout status.
- **Variable Table (`VarSymbol`):**
  - Name, integer value, and declaration flag.
- **Case-Insensitive Lookup:**
  - Handled via `strcasecmp`, ensuring `Rohan`, `ROHAN`, and `rohan` resolve to the same record.

---

## SLIDE 7: Live Room Inventory & Overbooking Protection
- **Total Hotel Inventory: 50 Rooms**
  - `STANDARD`: 20 Rooms (Rs. 2,500/night)
  - `DELUXE`: 15 Rooms (Rs. 4,500/night)
  - `SUITE`: 10 Rooms (Rs. 8,000/night)
  - `PRESIDENTIAL`: 5 Rooms (Rs. 18,000/night)
- **Real-Time Dynamic Tracking:**
  - On room allocation, remaining count is displayed immediately.
  - If a category is full, an `[Availability Alert]` is raised.
  - On `CHECKOUT`, rooms are dynamically released back into inventory.

---

## SLIDE 8: Semantic Validation & Error Trapping
- **Tested edge cases via `error_demo.stay`:**
  - **Duplicate Registration:** Prevents re-registering active guests.
  - **Undeclared Guest Operation:** Rejects operations on unknown guests.
  - **Duration Validation:** Rejects stay duration $\le 0$.
  - **Illegal Discount Range:** Flags discounts $< 0\%$ or $> 100\%$.
  - **Division by Zero:** Safely handled during expression evaluation.
  - **Double Checkout Warning:** Prevents re-billing a checked-out guest.

---

## SLIDE 9: Demonstration & Sample Program (`booking.stay`)
```stay
GUEST Aarav;
ROOM Aarav SUITE;
NIGHTS Aarav 3;
AMENITY Aarav DINING 3200;
AMENITY Aarav "Airport Luxury Pickup" 1500;
DISCOUNT Aarav 10;
TAX Aarav 18;
CHECKOUT Aarav;
```
- **Execution Highlights:**
  - Room tariff ($3 \times 8000$): **Rs. 24,000**
  - Folio services (Dining + Pickup): **Rs. 4,700**
  - Gross subtotal: **Rs. 28,700**
  - Less 10% Discount: **-Rs. 2,870**
  - Plus 18% GST: **+Rs. 4,649**
  - **Grand Total Payable: Rs. 30,479**
  - Room release notification and live inventory update.

---

## SLIDE 10: Conclusion & Learning Outcomes
- Successfully engineered an end-to-end compiler using the classic **Lex & Yacc** toolchain.
- Key Accomplishments:
  - 16 custom keywords with 0 shift/reduce conflicts.
  - Abstract Syntax Tree (AST) evaluation engine.
  - Real-time room inventory management and overbooking protection.
  - Practical compiler engineering principles applied to real-world business automation.

*Speaker Note:*
"Thank you. We are now happy to answer your questions and perform the live demonstration."
