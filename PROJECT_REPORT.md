# MINI PROJECT REPORT
## COMPILER DESIGN LABORATORY

---

### **TITLE:** HotelStay: A Domain-Specific Language for Luxury Hotel Reservation and Billing Automation using Lex and Yacc
**Subject:** Compiler Design  
**Tools Used:** LEX (Flex 2.6.4), YACC (Bison 3.8.2), GCC 13.2.0  
**Target File Extension:** `.stay`  
**GitHub Repository:** [https://github.com/preetkakdiya/HotelStay-Compiler](https://github.com/preetkakdiya/HotelStay-Compiler)

---

## 1. ABSTRACT
Modern software architectures rely extensively on Domain-Specific Languages (DSLs) to express complex domain operations concisely compared to general-purpose programming languages. In this mini project, we design and implement **HotelStay**, a hybrid domain-specific and algorithmic programming language compiler built using the classical Lex (Flex) and Yacc (Bison) toolchain in C.

HotelStay provides high-level grammar and custom keywords tailored for hotel room reservations, room category tariff allocation, extended stay calculations with arithmetic expression evaluation, folio service and amenity billing, promotional discounts, dynamic tax configuration, live room inventory management, and algorithmic control flow (variables, conditionals, and loops). The compiler parses source code through a complete front-end pipeline: Lexical Analysis, LALR(1) Context-Free Grammar Parsing with an Abstract Syntax Tree (AST), dual Symbol Table management, and semantic verification.

---

## 2. OBJECTIVES & SCOPE
- To understand the practical implementation of compiler front-end tools: **Lex (Flex)** and **Yacc (Bison)**.
- To design a custom Lexical Analyzer that scans source code from `.stay` files and outputs valid tokens.
- To formulate an unambiguous Context-Free Grammar (CFG) for parsing domain operations, mathematical expressions, conditionals, and loops with **zero shift/reduce conflicts**.
- To construct an in-memory **Abstract Syntax Tree (AST)** for syntax-directed evaluation.
- To maintain dual **Symbol Tables** tracking guest folios (up to 50 guests) and user-declared integer variables (up to 100 variables).
- To implement live room inventory tracking across 4 tiers (Standard: 20, Deluxe: 15, Suite: 10, Presidential: 5 = 50 Total Rooms) with overbooking protection and dynamic room release.
- To enforce semantic constraints catching duplicate declarations, operations on undeclared entities, non-positive durations, out-of-range discounts, and division by zero.
- To produce automated itemized tax invoices upon checkout execution.

---

## 3. COMPILER ARCHITECTURE & WORKFLOW

```
+-------------------------------------------------------+
|             Source Code Script (*.stay)               |
+-------------------------------------------------------+
                           |
                           v
+-------------------------------------------------------+
|           Lexical Analyzer (Flex - HotelStay.l)       |
|    - Strips whitespace & comments (// and /* */)      |
|    - Recognizes 16 Keywords, Room Types, Identifiers  |
|    - Emits Tokens & assigns semantic values (yylval)  |
+-------------------------------------------------------+
                           | Tokens (GUEST, ROOM, etc.)
                           v
+-------------------------------------------------------+
|             Parser (Bison - HotelStay.y)              |
|    - LALR(1) Parsing Table (0 conflicts)              |
|    - Shift / Reduce CFG Actions                       |
|    - Operator Precedence & Associativity              |
|    - Abstract Syntax Tree (AST) Construction          |
+-------------------------------------------------------+
                           |
           +---------------+---------------+
           |                               |
           v                               v
+-----------------------+       +-----------------------+
| Dual Symbol Tables    |       | Semantic Engine & AST |
| - Guest Folio Records |       | - Inventory Tracking  |
| - Variables (MAKE)    |       | - Conditionals (WHEN) |
| - Case-Insensitive    |       | - Loops (REPEAT)      |
|   Lookup (strcasecmp) |       | - Diagnostic Errors   |
+-----------------------+       +-----------------------+
           |                               |
           +---------------+---------------+
                           |
                           v
+-------------------------------------------------------+
|     Execution Output: Occupancy Logs & Tax Invoices   |
+-------------------------------------------------------+
```

---

## 4. LANGUAGE SPECIFICATION

### 4.1 Custom Keywords (16 Keywords)
| Keyword | Category | Functionality |
| :--- | :--- | :--- |
| `GUEST` | Declaration | Enters new guest into the symbol table |
| `ROOM` | Allocation | Sets room category and default nightly tariff |
| `NIGHTS` | Duration | Sets stay duration; evaluates arithmetic expressions |
| `AMENITY` | Folio Charge | Adds auxiliary services (Dining, Airport Pickup, Buffet, Laundry, Gym) |
| `DISCOUNT` | Pricing Rule | Applies percentage deduction on gross bill (0% to 100%) |
| `TAX` | Pricing Rule | Configures GST/VAT percentage (default: 12%) |
| `CHECKOUT` | Execution | Finalizes billing, applies deductions/taxes, releases room, prints invoice |
| `STATUS` | Utility | Displays comprehensive inventory & occupancy table |
| `PRINT` | I/O | Prints strings or expression results |
| `RESET` | Management | Removes guest entry from active memory |
| `MAKE` | Variables | Declares a custom integer variable in symbol table |
| `TAKE` | Input | Accepts interactive integer input from user (`scanf`) |
| `WHEN` | Condition | Conditional branching (`if` statement) |
| `OTHERWISE` | Condition | Alternative conditional branch (`else` statement) |
| `REPEAT` | Loop | Iterative loop construct (`while` loop) |
| `STOP` | Control | Terminates program execution cleanly |

### 4.2 Room Categories & Inventory Caps
- **`STANDARD`**: 20 Rooms (Rs. 2,500/night)
- **`DELUXE`**: 15 Rooms (Rs. 4,500/night)
- **`SUITE`**: 10 Rooms (Rs. 8,000/night)
- **`PRESIDENTIAL`**: 5 Rooms (Rs. 18,000/night)
- **Total Hotel Capacity**: 50 Rooms

---

## 5. FORMAL GRAMMAR (CONTEXT-FREE GRAMMAR)

```
<program>            ::= <statements> | %empty
<statements>         ::= <statements> <statement> | <statement>
<statement>          ::= <guest_decl>
                       | <room_assignment>
                       | <nights_assignment>
                       | <amenity_addition>
                       | <discount_assignment>
                       | <tax_assignment>
                       | <checkout_stmt>
                       | <status_stmt>
                       | <print_stmt>
                       | <reset_stmt>
                       | <make_stmt>
                       | <take_stmt>
                       | <assign_stmt>
                       | <when_stmt>
                       | <repeat_stmt>
                       | <stop_stmt>
                       | <statement_block>

<statement_block>    ::= '{' <statements> '}' | '{' '}'
<guest_decl>         ::= GUEST ID ';'
<room_assignment>    ::= ROOM ID STANDARD ';'
                       | ROOM ID DELUXE ';'
                       | ROOM ID SUITE ';'
                       | ROOM ID PRESIDENTIAL ';'
                       | ROOM ID ID <expression> ';'
<nights_assignment>  ::= NIGHTS ID <expression> ';'
<amenity_addition>   ::= AMENITY ID ID <expression> ';'
                       | AMENITY ID STRING <expression> ';'
<discount_assignment>::= DISCOUNT ID <expression> ';'
<tax_assignment>     ::= TAX ID <expression> ';'
<checkout_stmt>      ::= CHECKOUT ID ';'
<status_stmt>        ::= STATUS ';'
<print_stmt>         ::= PRINT STRING ';' | PRINT <expression> ';'
<reset_stmt>         ::= RESET ID ';'
<make_stmt>          ::= MAKE ID ';'
<take_stmt>          ::= TAKE ID ';'
<assign_stmt>        ::= ID '=' <expression> ';'
<when_stmt>          ::= WHEN '(' <expression> ')' <statement>
                       | WHEN '(' <expression> ')' <statement> OTHERWISE <statement>
<repeat_stmt>        ::= REPEAT '(' <expression> ')' <statement>
<stop_stmt>          ::= STOP ';'

<expression>         ::= <expression> EQ <term>
                       | <expression> NEQ <term>
                       | <expression> '<' <term>
                       | <expression> '>' <term>
                       | <expression> LTE <term>
                       | <expression> GTE <term>
                       | <expression> '+' <term>
                       | <expression> '-' <term>
                       | <term>
<term>               ::= <term> '*' <factor>
                       | <term> '/' <factor>
                       | <factor>
<factor>             ::= NUMBER
                       | ID
                       | '(' <expression> ')'
```

---

## 6. IMPLEMENTATION DETAILS

### 6.1 Lexical Analyzer Specification (`HotelStay.l`)
- Recognizes all 16 keywords, room tiers, operators, numbers, string literals, and identifiers.
- Manages single-line (`//.*`) and multi-line (`/* ... */`) comments.
- Returns semantic values through `yylval.num` and `yylval.str`.

### 6.2 Parser & AST Engine (`HotelStay.y`)
- Evaluates precedence via `%left` and resolves `if-else` dangling ambiguity using `%nonassoc LOWER_THAN_OTHERWISE` and `%nonassoc OTHERWISE`.
- Constructs an in-memory Abstract Syntax Tree (`StmtNode`, `ExprNode`) during parsing.
- Executes statements via `executeStmtList()` with guard counters against infinite loops.
- Provides case-insensitive lookup via `strcasecmp()`.

---

## 7. TEST CASES & EXECUTION RESULTS

### 7.1 Main Demonstration Program (`booking.stay`)
```stay
PRINT "Welcome to Grand Horizon Hotel - Automated Booking Engine";

// 1. Registering Guests
GUEST Aarav;
GUEST Priya;
GUEST Rohan;
GUEST Sarthak;

// 2. Room Allocations
ROOM Aarav SUITE;
ROOM Priya DELUXE;
ROOM Rohan STANDARD;
ROOM Sarthak PRESIDENTIAL;

// 3. Stay Duration (in nights)
NIGHTS Aarav 3;
NIGHTS Priya (2 + 1); // Arithmetic expression supported!
NIGHTS Rohan 2;
NIGHTS Sarthak 2;

// 4. Folio Services & Amenities (Cost in Rs.)
AMENITY Aarav DINING 3200;
AMENITY Aarav "Airport Luxury Pickup" 1500;

AMENITY Priya CAB 600;
AMENITY Priya "Breakfast Buffet" 850;

AMENITY Rohan LAUNDRY 350;

AMENITY Sarthak Dining 3500;
AMENITY Sarthak "Airport Luxury Pickup" 1500;

// 5. Promotional Discounts & Custom Tax Configurations
DISCOUNT Aarav 10;   // 10% Loyalty Discount
TAX Aarav 18;        // 18% Luxury GST

DISCOUNT Priya 5;    // 5% Early Bird Discount
TAX Priya 12;        // 12% Standard GST

DISCOUNT Sarthak 15; // 15% Loyalty Discount
TAX Sarthak 18;      // 18% Luxury GST

// 6. Check Current Hotel Occupancy
STATUS;

// 7. Check-out and Invoice Generation
CHECKOUT Aarav;
CHECKOUT Priya;
CHECKOUT Sarthak;

// 8. Re-check Hotel Status
STATUS;
```

### 7.2 Generated Execution Output
```text
============================================================
       HOTELSTAY DOMAIN-SPECIFIC LANGUAGE (LEX & YACC)      
============================================================

[HotelStay] Executing input script: 'booking.stay'

[NOTICE] Welcome to Grand Horizon Hotel - Automated Booking Engine
[Registration] Guest 'Aarav' checked in. Ready for room assignment.
[Registration] Guest 'Priya' checked in. Ready for room assignment.
[Registration] Guest 'Rohan' checked in. Ready for room assignment.
[Registration] Guest 'Sarthak' checked in. Ready for room assignment.
[Room Allocated] Aarav allocated 'SUITE' Room @ Rs.8000/night. (9 SUITE room(s) still available)
[Room Allocated] Priya allocated 'DELUXE' Room @ Rs.4500/night. (14 DELUXE room(s) still available)
[Room Allocated] Rohan allocated 'STANDARD' Room @ Rs.2500/night. (19 STANDARD room(s) still available)
[Room Allocated] Sarthak allocated 'PRESIDENTIAL' Room @ Rs.18000/night. (4 PRESIDENTIAL room(s) still available)
[Stay Updated] Aarav stay duration updated to 3 night(s).
[Stay Updated] Priya stay duration updated to 3 night(s).
[Stay Updated] Rohan stay duration updated to 2 night(s).
[Stay Updated] Sarthak stay duration updated to 2 night(s).
[Amenity Added] Added 'DINING' (Rs.3200) to Aarav's folio.
[Amenity Added] Added 'Airport Luxury Pickup' (Rs.1500) to Aarav's folio.
[Amenity Added] Added 'CAB' (Rs.600) to Priya's folio.
[Amenity Added] Added 'Breakfast Buffet' (Rs.850) to Priya's folio.
[Amenity Added] Added 'LAUNDRY' (Rs.350) to Rohan's folio.
[Amenity Added] Added 'Dining' (Rs.3500) to Sarthak's folio.
[Amenity Added] Added 'Airport Luxury Pickup' (Rs.1500) to Sarthak's folio.
[Discount Applied] 10% promotional discount applied for Aarav.
[Tax Configured] 18% tax rate set for Aarav.
[Discount Applied] 5% promotional discount applied for Priya.
[Tax Configured] 12% tax rate set for Priya.
[Discount Applied] 15% promotional discount applied for Sarthak.
[Tax Configured] 18% tax rate set for Sarthak.

+==========================================================+
|          HOTEL GRAND HORIZON - ROOM AVAILABILITY         |
+==========================================================+
  Room Category    Total    Occupied   Available   
  --------------------------------------------------------
  STANDARD         20       1          19          
  DELUXE           15       1          14          
  SUITE            10       1          9           
  PRESIDENTIAL     5        1          4           
  --------------------------------------------------------
  TOTAL HOTEL      50       4          46          
+==========================================================+

+----------------------------------------------------------+
|                 ACTIVE GUEST ROSTER LOG                  |
+----------------------------------------------------------+
  Guest           Room           Nights   Status      
  --------------------------------------------------------
  Aarav           SUITE          3        Occupied    
  Priya           DELUXE         3        Occupied    
  Rohan           STANDARD       2        Occupied    
  Sarthak         PRESIDENTIAL   2        Occupied    
+----------------------------------------------------------+


+==========================================================+
|              HOTEL GRAND HORIZON - TAX INVOICE           |
+==========================================================+
  Guest Name    : Aarav                                 
  Room Type     : SUITE           (Rs.8000 / night)
  Duration      : 3 night(s)
------------------------------------------------------------
  Room Charges  : 3 nights x Rs.8000 = Rs.24000     
  Additional Amenities & Folio Services:
    * DINING                    : Rs.3200
    * Airport Luxury Pickup     : Rs.1500
  Amenities Total Charges      : Rs.4700      
------------------------------------------------------------
  Gross Subtotal                : Rs.28700     
  Discount (10%)                : -Rs.2870      
  Net Taxable Amount            : Rs.25830     
  Taxes / GST (18%)             : +Rs.4649      
+==========================================================+
  GRAND TOTAL PAYABLE           : Rs.30479     
+==========================================================+
  Thank you for choosing Hotel Grand Horizon! Have a safe trip!

[Room Released] SUITE room released! Total SUITE rooms now available: 10


+==========================================================+
|              HOTEL GRAND HORIZON - TAX INVOICE           |
+==========================================================+
  Guest Name    : Priya                                 
  Room Type     : DELUXE          (Rs.4500 / night)
  Duration      : 3 night(s)
------------------------------------------------------------
  Room Charges  : 3 nights x Rs.4500 = Rs.13500     
  Additional Amenities & Folio Services:
    * CAB                       : Rs.600
    * Breakfast Buffet          : Rs.850
  Amenities Total Charges      : Rs.1450      
------------------------------------------------------------
  Gross Subtotal                : Rs.14950     
  Discount ( 5%)                : -Rs.747       
  Net Taxable Amount            : Rs.14203     
  Taxes / GST (12%)             : +Rs.1704      
+==========================================================+
  GRAND TOTAL PAYABLE           : Rs.15907     
+==========================================================+
  Thank you for choosing Hotel Grand Horizon! Have a safe trip!

[Room Released] DELUXE room released! Total DELUXE rooms now available: 15


+==========================================================+
|              HOTEL GRAND HORIZON - TAX INVOICE           |
+==========================================================+
  Guest Name    : Sarthak                               
  Room Type     : PRESIDENTIAL    (Rs.18000 / night)
  Duration      : 2 night(s)
------------------------------------------------------------
  Room Charges  : 2 nights x Rs.18000 = Rs.36000     
  Additional Amenities & Folio Services:
    * Dining                    : Rs.3500
    * Airport Luxury Pickup     : Rs.1500
  Amenities Total Charges      : Rs.5000      
------------------------------------------------------------
  Gross Subtotal                : Rs.41000     
  Discount (15%)                : -Rs.6150      
  Net Taxable Amount            : Rs.34850     
  Taxes / GST (18%)             : +Rs.6273      
+==========================================================+
  GRAND TOTAL PAYABLE           : Rs.41123     
+==========================================================+
  Thank you for choosing Hotel Grand Horizon! Have a safe trip!

[Room Released] PRESIDENTIAL room released! Total PRESIDENTIAL rooms now available: 5


+==========================================================+
|          HOTEL GRAND HORIZON - ROOM AVAILABILITY         |
+==========================================================+
  Room Category    Total    Occupied   Available   
  --------------------------------------------------------
  STANDARD         20       1          19          
  DELUXE           15       0          15          
  SUITE            10       0          10          
  PRESIDENTIAL     5        0          5           
  --------------------------------------------------------
  TOTAL HOTEL      50       1          49          
+==========================================================+

+----------------------------------------------------------+
|                 ACTIVE GUEST ROSTER LOG                  |
+----------------------------------------------------------+
  Guest           Room           Nights   Status      
  --------------------------------------------------------
  Aarav           SUITE          3        Checked Out 
  Priya           DELUXE         3        Checked Out 
  Rohan           STANDARD       2        Occupied    
  Sarthak         PRESIDENTIAL   2        Checked Out 
+----------------------------------------------------------+


============================================================
           HOTELSTAY COMPILATION & EXECUTION COMPLETE       
============================================================
```

### 7.3 Negative Error Test (`error_demo.stay`)
```text
[NOTICE] --- Commencing Robustness & Error Handling Tests ---
[Registration] Guest 'Simran' checked in. Ready for room assignment.
[Semantic Error] Guest 'Simran' is already registered in the system.
[Semantic Error] Cannot assign room. Guest 'UnknownGuest' not found.
[Semantic Error] Duration of stay must be at least 1 night. Provided: 0
[Semantic Error] Invalid discount percent: 150% (Must be between 0 and 100).
[Stay Updated] Simran stay duration updated to 2 night(s).
[Discount Applied] 15% promotional discount applied for Simran.
[INVOICE GENERATED FOR SIMRAN]
[Warning] Guest 'Simran' has already been checked out.
```

---

## 8. CONCLUSION
The **HotelStay** project successfully demonstrates practical end-to-end compiler design concepts using classical **Lex (Flex) and Yacc (Bison)** parser generation in C. The compiler bridges declarative business domain logic with full general-purpose programming constructs (conditionals, loops, and variables) via an Abstract Syntax Tree (AST). It provides real-time room inventory management, case-insensitive symbol lookups, comprehensive semantic validation, and automated tax invoicing. The project satisfies all university laboratory criteria, incorporating 16 custom keywords with zero grammar conflicts.
