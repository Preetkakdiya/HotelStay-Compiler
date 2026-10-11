# MINI PROJECT REPORT
## COMPILER DESIGN LABORATORY

---

### **TITLE:** HotelStay: A Domain-Specific Language for Luxury Hotel Reservation and Billing Automation using Lex and Yacc
**Subject:** Compiler Design  
**Tools Used:** LEX (Flex 2.5+) & YACC (Bison 2.4+) / GCC  
**Target Extension:** `.stay`  

---

## 1. ABSTRACT
Modern software architectures rely extensively on Domain-Specific Languages (DSLs) to express domain logic concisely and intuitively compared to general-purpose programming languages. In this mini project, we design and implement **HotelStay**, a declarative domain-specific language compiler built using the classical Lex (Flex) and Yacc (Bison) toolchain. 

HotelStay provides high-level grammar and custom keywords tailored for hotel room reservations, room category tariff allocation, extended stay calculations with arithmetic expression evaluation, folio service and amenity billing, promotional discounts, dynamic tax configuration, and occupancy status tracking. The compiler executes in two synchronous phases: Lexical Analysis (tokenization, string literals, numbers, and identifiers) and Syntax/Semantic Analysis (LALR(1) shift-reduce parsing, Context-Free Grammar evaluation, Symbol Table tracking, and error diagnostic reporting).

---

## 2. OBJECTIVES & SCOPE
- To understand the working mechanism of compiler front-end tools: **Lex (Flex)** and **Yacc (Bison)**.
- To design a custom Lexical Analyzer that scans source code from `.stay` files and outputs valid tokens.
- To formulate an unambiguous Context-Free Grammar (CFG) for parsing domain statements and mathematical expressions.
- To design an in-memory **Symbol Table** structure tracking guest records, room categories, nightly rates, amenities folio, and checkout state.
- To implement semantic analysis routines that catch duplicate declarations, operations on undeclared entities, zero/negative durations, and illegal discount values.
- To produce an automated itemized tax invoice upon checkout execution.

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
|    - Strips whitespace & comments                     |
|    - Matches regex patterns                           |
|    - Emits Tokens & assigns yylval                    |
+-------------------------------------------------------+
                           | Tokens (GUEST, ROOM, etc.)
                           v
+-------------------------------------------------------+
|             Parser (Bison - HotelStay.y)              |
|    - LALR(1) Parsing Table                            |
|    - Shift / Reduce CFG Actions                       |
|    - Operator Precedence & Associativity              |
+-------------------------------------------------------+
                           |
           +---------------+---------------+
           |                               |
           v                               v
+-----------------------+       +-----------------------+
|  Symbol Table Manager |       |    Semantic Engine    |
| - Guest folio lookup  |       | - Rate calculation    |
| - Duplicate check     |       | - Discount & Tax math |
| - State maintenance   |       | - Diagnostic error log|
+-----------------------+       +-----------------------+
           |                               |
           +---------------+---------------+
                           |
                           v
+-------------------------------------------------------+
|     Execution Output: Occupancy Logs & Invoices       |
+-------------------------------------------------------+
```

---

## 4. LANGUAGE SPECIFICATION

### 4.1 Custom Keywords (10 Keywords)
| Keyword | Category | Functionality |
| :--- | :--- | :--- |
| `GUEST` | Declaration | Enters new guest into the symbol table |
| `ROOM` | Configuration | Sets room category and default nightly tariff |
| `NIGHTS` | Configuration | Sets duration of stay; evaluates arithmetic expressions |
| `AMENITY` | Folio Charge | Adds auxiliary services (Buffet, Dining, Cab, etc.) with cost |
| `DISCOUNT` | Pricing Rule | Applies percentage deduction on gross bill (0% to 100%) |
| `TAX` | Pricing Rule | Configures GST/VAT percentage |
| `CHECKOUT` | Execution | Finalizes billing, applies deductions/taxes, prints tax invoice |
| `STATUS` | Utility | Displays comprehensive occupancy table |
| `PRINT` | I/O | Prints strings or expression results |
| `RESET` | Management | Removes guest entry from active memory |

### 4.2 Room Categories
- `STANDARD` (Base tariff: Rs. 2,500/night)
- `DELUXE` (Base tariff: Rs. 4,500/night)
- `SUITE` (Base tariff: Rs. 8,000/night)
- `PRESIDENTIAL` (Base tariff: Rs. 18,000/night)
- Custom rooms can also be assigned using: `ROOM <guest> <CustomType> <CustomRate>;`

---

## 5. FORMAL GRAMMAR (BNF / CFG RULES)

```
<program>            ::= <statements>
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

<expression>         ::= <expression> '+' <term>
                       | <expression> '-' <term>
                       | <term>
<term>               ::= <term> '*' <factor>
                       | <term> '/' <factor>
                       | <factor>
<factor>             ::= NUMBER
                       | '(' <expression> ')'
```

---

## 6. IMPLEMENTATION DETAILS

### 6.1 Lexical Analyzer Specification (`HotelStay.l`)
- Contains regular expressions for keywords, identifiers (`[a-zA-Z_][a-zA-Z0-9_]*`), numbers (`[0-9]+`), string literals (`\"[^\"]*\"`), single-line comments (`//.*`), and multi-line comments.
- Passes semantic values via `yylval.num` and `yylval.str`.

### 6.2 Parser & Semantic Engine (`HotelStay.y`)
- Defines `%union` containing integer numbers and heap-allocated string pointers.
- Declares left associativity and precedence for arithmetic operations (`+`, `-`, `*`, `/`).
- Implements `GuestRecord` symbol table with array-based linear probing and boundary checks.
- Generates itemized ASCII tax invoice with gross subtotal, percentage discounts, net taxable calculation, and tax addition.

---

## 7. TEST CASES & EXECUTION RESULTS

### 7.1 Test Program (`booking.stay`)
```
PRINT "Welcome to Grand Horizon Hotel - Automated Booking Engine";

GUEST Aarav;
GUEST Priya;
GUEST Rohan;

ROOM Aarav SUITE;
ROOM Priya DELUXE;
ROOM Rohan STANDARD;

NIGHTS Aarav 3;
NIGHTS Priya (2 + 1);
NIGHTS Rohan 2;

AMENITY Aarav BUFFET 2500;
AMENITY Aarav DINING 3200;
AMENITY Aarav "Airport Luxury Pickup" 1500;

AMENITY Priya "Breakfast Buffet" 1800;
AMENITY Priya CAB 600;

AMENITY Rohan LAUNDRY 350;

DISCOUNT Aarav 10;
TAX Aarav 18;

DISCOUNT Priya 5;
TAX Priya 12;

STATUS;

CHECKOUT Aarav;
CHECKOUT Priya;

STATUS;
```

### 7.2 Generated Output
```
============================================================
       HOTELSTAY DOMAIN-SPECIFIC LANGUAGE (LEX & YACC)      
============================================================

[HotelStay] Executing input script: 'booking.stay'

[NOTICE] Welcome to Grand Horizon Hotel - Automated Booking Engine
[Registration] Guest 'Aarav' successfully registered (Default: STANDARD Room @ Rs.2500/night).
[Registration] Guest 'Priya' successfully registered (Default: STANDARD Room @ Rs.2500/night).
[Registration] Guest 'Rohan' successfully registered (Default: STANDARD Room @ Rs.2500/night).
[Room Allocated] Aarav allocated 'SUITE' Room @ Rs.8000/night.
[Room Allocated] Priya allocated 'DELUXE' Room @ Rs.4500/night.
[Room Allocated] Rohan allocated 'STANDARD' Room @ Rs.2500/night.
[Stay Updated] Aarav stay duration updated to 3 night(s).
[Stay Updated] Priya stay duration updated to 3 night(s).
[Stay Updated] Rohan stay duration updated to 2 night(s).
[Amenity Added] Added 'BUFFET' (Rs.2500) to Aarav's folio.
[Amenity Added] Added 'DINING' (Rs.3200) to Aarav's folio.
[Amenity Added] Added 'Airport Luxury Pickup' (Rs.1500) to Aarav's folio.
[Amenity Added] Added 'Breakfast Buffet' (Rs.1800) to Priya's folio.
[Amenity Added] Added 'CAB' (Rs.600) to Priya's folio.
[Amenity Added] Added 'LAUNDRY' (Rs.350) to Rohan's folio.
[Discount Applied] 10% promotional discount applied for Aarav.
[Tax Configured] 18% tax rate set for Aarav.
[Discount Applied] 5% promotional discount applied for Priya.
[Tax Configured] 12% tax rate set for Priya.

+==========================================================+
|          HOTEL GRAND HORIZON - ROOM AVAILABILITY         |
+==========================================================+
  Room Category    Total    Occupied   Available   
  --------------------------------------------------------
  STANDARD         20       1          19          
  DELUXE           15       1          14          
  SUITE            10       1          9           
  PRESIDENTIAL     5        0          5           
  --------------------------------------------------------
  TOTAL HOTEL      50       3          47          
+==========================================================+

+----------------------------------------------------------+
|                 ACTIVE GUEST ROSTER LOG                  |
+----------------------------------------------------------+
  Guest           Room           Nights   Status      
  --------------------------------------------------------
  Aarav           SUITE          3        Occupied    
  Priya           DELUXE         3        Occupied    
  Rohan           STANDARD       2        Occupied    
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
    * BUFFET                    : Rs.2500
    * DINING                    : Rs.3200
    * Airport Luxury Pickup     : Rs.1500
  Amenities Total Charges      : Rs.7200      
------------------------------------------------------------
  Gross Subtotal                : Rs.31200     
  Discount (10%)                : -Rs.3120      
  Net Taxable Amount            : Rs.28080     
  Taxes / GST (18%)             : +Rs.5054      
+==========================================================+
  GRAND TOTAL PAYABLE           : Rs.33134     
+==========================================================+
  Thank you for choosing Hotel Grand Horizon! Have a safe trip!


+==========================================================+
|              HOTEL GRAND HORIZON - TAX INVOICE           |
+==========================================================+
  Guest Name    : Priya                                 
  Room Type     : DELUXE          (Rs.4500 / night)
  Duration      : 3 night(s)
------------------------------------------------------------
  Room Charges  : 3 nights x Rs.4500 = Rs.13500     
  Additional Amenities & Folio Services:
    * Breakfast Buffet          : Rs.1800
    * CAB                       : Rs.600
  Amenities Total Charges      : Rs.2400      
------------------------------------------------------------
  Gross Subtotal                : Rs.15900     
  Discount ( 5%)                : -Rs.795       
  Net Taxable Amount            : Rs.15105     
  Taxes / GST (12%)             : +Rs.1812      
+==========================================================+
  GRAND TOTAL PAYABLE           : Rs.16917     
+==========================================================+
  Thank you for choosing Hotel Grand Horizon! Have a safe trip!


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
+----------------------------------------------------------+


============================================================
           HOTELSTAY COMPILATION & EXECUTION COMPLETE       
============================================================
```

### 7.3 Negative Error Test (`error_demo.stay`)
Output verifies robust semantic validation:
```
[NOTICE] --- Commencing Robustness & Error Handling Tests ---
[Registration] Guest 'Simran' successfully registered (Default: STANDARD Room @ Rs.2500/night).
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
The **HotelStay** DSL project successfully demonstrates practical end-to-end compiler design concepts using classical **Lex and Yacc** parser generation. The compiler parses human-readable hotel domain statements into automated billing records while enforcing semantic constraints, symbol table state tracking, and mathematical expression evaluation. The project thoroughly satisfies all university laboratory criteria, incorporating 10 custom keywords, error diagnostics, and formatted reporting.
