# HotelStay — Domain-Specific Compiler Language

**Course**: Compiler Design Mini Project  
**Implementation Tools**: Lex / Flex (Lexical Analyzer) & Yacc / Bison (LALR Parser)  
**File Extension**: `.stay`  

---

## 📌 Project Overview
**HotelStay** is a custom domain-specific programming language developed using **Lex and Yacc**. It models an automated luxury hotel reservation, folio service management, and billing system.

The language performs:
1. **Lexical Analysis (`HotelStay.l`)**: Recognizes tokens such as keywords (`GUEST`, `ROOM`, `NIGHTS`, `AMENITY`, `DISCOUNT`, `TAX`, `CHECKOUT`, `STATUS`, `PRINT`, `RESET`), room categories, numbers, identifiers, operators, and comments.
2. **Syntax Analysis (`HotelStay.y`)**: Validates statements using Context-Free Grammar (CFG) and handles arithmetic precedence.
3. **Semantic Actions & Symbol Table**:
   - Manages a guest registry symbol table.
   - Prevents duplicate registrations.
   - Catches undeclared guest operations.
   - Validates stays (minimum 1 night) and discount limits (0–100%).
   - Calculates itemized room charges, folio service additions, promo discounts, GST/VAT taxes, and generates formatted invoices.

---

## 🔑 Custom Keywords (10 Keywords)
The faculty requires at least **7 custom keywords** for a 3-member group. **HotelStay includes 10 custom keywords**:

| Keyword | Description | Example |
| :--- | :--- | :--- |
| `GUEST` | Registers a new guest into the symbol table | `GUEST Aarav;` |
| `ROOM` | Assigns room category (`STANDARD`, `DELUXE`, `SUITE`, `PRESIDENTIAL`) | `ROOM Aarav SUITE;` |
| `NIGHTS` | Sets stay duration in nights (supports arithmetic expressions) | `NIGHTS Aarav 3;` |
| `AMENITY` | Adds amenity/folio charges (Spa, Dining, Cab, etc.) | `AMENITY Aarav SPA 2500;` |
| `DISCOUNT` | Applies percentage promotional discount | `DISCOUNT Aarav 10;` |
| `TAX` | Configures GST/tax percentage (default: 12%) | `TAX Aarav 18;` |
| `CHECKOUT` | Computes final bill and generates a formatted tax invoice | `CHECKOUT Aarav;` |
| `STATUS` | Displays current hotel room occupancy log | `STATUS;` |
| `PRINT` | Prints text announcements or arithmetic results | `PRINT "Welcome to Grand Horizon";` |
| `RESET` | Removes guest record from active registry | `RESET Aarav;` |

---

## 🚀 How to Run the Project

### Option A: Running on JDoodle Online Compiler (Recommended by Faculty)
If Flex/Bison is not installed locally on your PC, use **JDoodle Online Flex Compiler**:
1. Open [https://www.jdoodle.com/online-flex-compiler](https://www.jdoodle.com/online-flex-compiler).
2. Follow the online multi-file setup or paste the Lex and Yacc source into the respective editor panes.
3. Provide `booking.stay` content into the **CommandLine Arguments** / **Stdin** box.
4. Click **Execute**.

---

### Option B: Running Locally on Windows (via CMD / MSYS2 / MinGW)

Ensure you have `bison` (or `win_bison`), `flex` (or `win_flex`), and `gcc` installed:

1. **Generate Parser C code and Header:**
   ```cmd
   bison -d HotelStay.y
   ```
   *Generates `HotelStay.tab.c` and `HotelStay.tab.h`.*

2. **Generate Lexer C code:**
   ```cmd
   flex HotelStay.l
   ```
   *Generates `lex.yy.c`.*

3. **Compile everything with GCC:**
   ```cmd
   gcc HotelStay.tab.c lex.yy.c -o HotelStay.exe
   ```

4. **Run the compiler with input `.stay` script:**
   ```cmd
   HotelStay.exe booking.stay
   ```
   *(Or double-click `build.bat` for automatic 1-click execution!)*

---

### Option C: Running on Linux / macOS / Ubuntu WSL
```bash
bison -d HotelStay.y
flex HotelStay.l
gcc HotelStay.tab.c lex.yy.c -o HotelStay
./HotelStay booking.stay
```

---

## 📂 Project Structure
- `HotelStay.l`: Flex lexical analyzer specification.
- `HotelStay.y`: Bison Yacc parser specification with grammar and symbol table functions.
- `booking.stay`: Primary test program showcasing all keywords, expressions, and receipts.
- `error_demo.stay`: Negative testing script showcasing semantic and syntax error trapping.
- `build.bat`: Windows compilation and execution script.
- `PROJECT_REPORT.md`: Complete laboratory project report for printout.
- `PPT_PRESENTATION.md`: Slide-by-slide seminar presentation deck.
