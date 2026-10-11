# HotelStay — Domain-Specific Compiler Language

**Course**: Compiler Design Mini Project  
**Implementation Tools**: Lex / Flex (Lexical Analyzer) & Yacc / Bison (LALR Parser) + GCC  
**Target File Extension**: `.stay`  
**GitHub Repository**: [https://github.com/preetkakdiya/HotelStay-Compiler](https://github.com/preetkakdiya/HotelStay-Compiler)

---

## 📌 Project Overview
**HotelStay** is a hybrid Domain-Specific Language (DSL) and programming language compiler designed and implemented using **Lex (Flex)** and **Yacc (Bison)**. It provides high-level declarative syntax for luxury hotel room reservations, room category tariff allocation, extended stay calculations, folio service billing, automated promotional discounts, customizable GST/tax calculation, live inventory management, and algorithmic control flow (variables, conditionals, and loops).

The compiler executes through a complete front-end pipeline:
1. **Lexical Analysis (`HotelStay.l`)**: Scans input character stream, filters whitespaces/comments, identifies 16 custom keywords, tokens, string literals, and numbers.
2. **Syntax Analysis (`HotelStay.y`)**: LALR(1) parser verifies Context-Free Grammar (CFG), resolves shift/reduce conflicts (0 warnings/conflicts), and constructs an Abstract Syntax Tree (AST).
3. **Symbol Table & Semantic Engine**:
   - **Guest Folio Registry**: Case-insensitive guest lookup, room allocations, amenity charges list, discount/tax calculations.
   - **Variable Symbol Table**: Stores integer variables declared via `MAKE` and populated via `TAKE` or assignments.
   - **Room Inventory Tracker**: Enforces category caps (Standard: 20, Deluxe: 15, Suite: 10, Presidential: 5 = Total 50), rejects overbooking, and dynamically releases rooms upon checkout.
4. **Execution & Target Output**: Generates live occupancy tables and formatted, itemized tax invoices.

---

## 🔑 Custom Keywords (16 Keywords)
*(Faculty requirement: At least 7 keywords for a group of 3)*

| Keyword | Category | Functionality | Example |
| :--- | :--- | :--- | :--- |
| `GUEST` | Declaration | Registers a new guest in the symbol table | `GUEST Aarav;` |
| `ROOM` | Allocation | Assigns room category (`STANDARD`, `DELUXE`, `SUITE`, `PRESIDENTIAL`) | `ROOM Aarav SUITE;` |
| `NIGHTS` | Duration | Sets stay duration; evaluates arithmetic expressions | `NIGHTS Aarav (2 + 1);` |
| `AMENITY` | Billing | Adds folio services (Dining, Airport Pickup, Buffet, Laundry, Gym) | `AMENITY Aarav DINING 3200;` |
| `DISCOUNT` | Pricing | Applies promotional discount percentage (0% to 100%) | `DISCOUNT Aarav 10;` |
| `TAX` | Pricing | Configures customized GST/VAT percentage (default: 12%) | `TAX Aarav 18;` |
| `CHECKOUT` | Execution | Computes final bill and generates formatted tax invoice | `CHECKOUT Aarav;` |
| `STATUS` | Dashboard | Displays live hotel room inventory and occupancy table | `STATUS;` |
| `PRINT` | I/O | Prints strings or arithmetic calculation results | `PRINT "Welcome to Grand Horizon";` |
| `RESET` | Management | Removes guest record from active memory | `RESET Aarav;` |
| `MAKE` | Variables | Declares a custom integer variable | `MAKE stayDays;` |
| `TAKE` | Input | Accepts interactive integer input from user (`scanf`) | `TAKE stayDays;` |
| `WHEN` | Condition | Conditional branching (`if` statement) | `WHEN (stayDays >= 4) { ... }` |
| `OTHERWISE` | Condition | Alternative branch (`else` statement) | `OTHERWISE { ... }` |
| `REPEAT` | Loop | Iterative loop construct (`while` loop) | `REPEAT (counter <= 3) { ... }` |
| `STOP` | Control | Terminates program execution cleanly | `STOP;` |

---

## 🏨 Room Categories & Live Inventory Management
The hotel models a luxury property with an inventory of **50 total rooms**:

| Category | Nightly Tariff | Inventory Cap | Features |
| :--- | :--- | :---: | :--- |
| **`STANDARD`** | Rs. 2,500 | 20 Rooms | Queen Bed, City View, Free Wi-Fi |
| **`DELUXE`** | Rs. 4,500 | 15 Rooms | King Bed, Balcony, Pool View |
| **`SUITE`** | Rs. 8,000 | 10 Rooms | Master Bedroom, Living Lounge, Jacuzzi |
| **`PRESIDENTIAL`** | Rs. 18,000 | 5 Rooms | Top Floor Penthouse, Butler Service |

* **Live Remaining Count**: Every room allocation immediately displays available units (e.g. `9 SUITE room(s) still available`).
* **Overbooking Protection**: Rejects bookings exceeding inventory limits with an `[Availability Alert]`.
* **Automatic Room Release**: When a guest checks out, the room is released back to the available inventory pool.

---

## 🛡️ Semantic Error Trapping & Edge-Case Handling
The compiler includes robust diagnostic validation tested via [`error_demo.stay`](file:///d:/cd%20project%201/error_demo.stay):
- **Duplicate Registration**: Prevents registering an existing active guest.
- **Undeclared Guest**: Operations on non-existent guests are safely rejected.
- **Invalid Stay Duration**: Rejects stay duration $\le 0$.
- **Out-of-Range Discounts**: Rejects discounts $< 0\%$ or $> 100\%$.
- **Division by Zero**: Evaluated safely without crashing.
- **Double Checkout Prevention**: Flags attempts to re-bill checked-out guests.
- **Case-Insensitive Guest Lookup**: Supports flexible casing (`Rohan`, `ROHAN`, `rohan`).

---

## 🚀 How to Run

### Method 1: 1-Click Batch Execution (Recommended)
Double-click [`build.bat`](file:///d:/cd%20project%201/build.bat) or run in terminal:
```cmd
build.bat
```
*(Automatically terminates any open process, compiles Bison/Flex/GCC with 0 warnings, and runs `booking.stay`)*

### Method 2: Manual Terminal Execution
```powershell
.\HotelStay.exe booking.stay
.\HotelStay.exe error_demo.stay
```

---

## 📂 Project Repository Structure
- `HotelStay.l`: Lexical analyzer specification (Flex).
- `HotelStay.y`: Parser, AST generator, grammar rules, and symbol table (Bison).
- `booking.stay`: Main demonstration program.
- `error_demo.stay`: Negative validation and error testing script.
- `build.bat`: Automated build and runner script for Windows.
- `PROJECT_REPORT.md`: Comprehensive laboratory project report for printout.
- `PPT_PRESENTATION.md`: 10-slide seminar presentation deck with speaker notes.
